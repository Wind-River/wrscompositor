/**
 * Copyright (C) 2013, LG Electronics
 *
 * This file is part of GENIVI Project web-api-vehicle.
 *
 * Contributions are licensed to the GENIVI Alliance under one or more
 * Contribution License Agreements.
 *
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Chunsung Jung <chunsung.jung@lge.com>
 *
 * @file mainwindow.cpp
 * For further information, see http://www.genivi.org/
 */

#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "def-socket.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    networkSettings();
    logInit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Network Settings
void MainWindow::networkSettings()
{
    // Read the IP address and Port
    QFile cfgFile(QCoreApplication::applicationDirPath() + '/' + CONFIG_FILE);
    if (!cfgFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(0,"[Error] network.cfg", cfgFile.errorString());
        exit(-1);
    }

    QString str;
    QTextStream in(&cfgFile);
    do
    {
        str = in.readLine();
        if (str.at(0) != '/')
        {
            if (str.contains("VNA_IP"))
            {
                m_ipAddress = str.remove(0, str.indexOf("=") + 1);
            }
            else if (str.contains("VNA_PORT"))
            {
                m_portNumberSend = str.remove(0, str.indexOf("=") + 1).toInt();
            }
            else if (str.contains("VDS_PORT"))
            {
                m_portNumberRcv = str.remove(0, str.indexOf("=") + 1).toInt();
            }
        }
    } while (!str.isNull());
    cfgFile.close();

    // Generate the UDP client & server
    udpClient = new UdpClient(m_ipAddress, m_portNumberSend);
    udpServer = new UdpServer(m_portNumberRcv);
    connect(udpServer->udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

    // Generate the message data
    msgData = new char[BUF_SIZE];
}

// Gengerate the log model
void MainWindow::logInit()
{
    logHeaderText[0] = tr("Status");
    logHeaderText[1] = tr("Message");
    logHeaderText[2] = tr("Value");
    logHeaderText[3] = tr("Time");

    m_itemModel = new QStandardItemModel(0, COLUMN_SIZE_LOG);
    for(int i = 0; i < COLUMN_SIZE_LOG; i++)
	{
        m_itemModel->setHeaderData(i, Qt::Horizontal, logHeaderText[i]);
	}

    m_proxyModel = new QSortFilterProxyModel;
    m_proxyModel->setDynamicSortFilter(true);
    m_proxyModel->setSourceModel(m_itemModel);

    ui->treeView->setModel(m_proxyModel);
    ui->treeView->setRootIsDecorated(false);
    ui->treeView->setSortingEnabled(true);
    ui->treeView->sortByColumn(3, Qt::AscendingOrder);
    ui->treeView->setEditTriggers(NULL);
}

void MainWindow::addLog(QAbstractItemModel *itemModel, const QString &status, const QString &msg, const QString &value, const QDateTime &dateTime)
{
    int index = itemModel->rowCount();
    itemModel->insertRow(index);
    itemModel->setData(itemModel->index(index, 0), status);
    itemModel->setData(itemModel->index(index, 1), msg);
    itemModel->setData(itemModel->index(index, 2), value);
    itemModel->setData(itemModel->index(index, 3), dateTime);

    ui->treeView->scrollToBottom();
    for (int i = 0; i < COLUMN_SIZE_LOG ; i++)
	{
        ui->treeView->resizeColumnToContents(i);
	}
}

void MainWindow::processPendingDatagrams()
{
    while (udpServer->udpSocket->hasPendingDatagrams()) 
	{
        QByteArray datagram;
        datagram.resize(udpServer->udpSocket->pendingDatagramSize());
        udpServer->udpSocket->readDatagram(datagram.data(), datagram.size());
        m_msgID = datagram.left(3).toInt();
        m_index = datagram.mid(3).toInt();
        switch (m_msgID)
        {
        case genivi::wavd::socket::VEHICLEINFO_WMI:
        {
            addLog(m_itemModel, "Receive", ui->label_A1->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::VEHICLEINFO_VIN:
        {
            addLog(m_itemModel, "Receive", ui->label_A2->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::VEHICLEINFO_VEHICLETYPE:
        {
            ui->comboBox_V1->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_V1->text(), ui->comboBox_V1->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::VEHICLEINFO_DOORTYPE_1STROW:
        {
            addLog(m_itemModel, "Receive", ui->label_V2->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::VEHICLEINFO_DOORTYPE_2NDROW:
        {
            addLog(m_itemModel, "Receive", ui->label_V3->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::VEHICLEINFO_DOORTYPE_3RDROW:
        {
            addLog(m_itemModel, "Receive", ui->label_V4->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::VEHICLEINFO_FUELTYPE:
        {
            ui->comboBox_V5->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_V5->text(), ui->comboBox_V5->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::VEHICLEINFO_TRANSMISSIONGEARTYPE:
        {
            ui->comboBox_V6->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_V6->text(), ui->comboBox_V6->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::VEHICLEINFO_WHEELINFO_RADIUS:
        {
            addLog(m_itemModel, "Receive", ui->label_V7->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::VEHICLEINFO_WHEELINFO_TRACK:
        {
            addLog(m_itemModel, "Receive", ui->label_V8->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::MAINTENANCE_ODOMETER:
        {
            addLog(m_itemModel, "Receive", ui->label_M1->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::MAINTENANCE_TRANSMISSIONOIL_LIFELEVEL:
        {
            ui->comboBox_M2->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_M2->text(), ui->comboBox_M2->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::MAINTENANCE_TRANSMISSIONOIL_TEMP:
        {
            addLog(m_itemModel, "Receive", ui->label_M3->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::MAINTENANCE_BRAKEFLUIDLEVEL:
        {
            ui->comboBox_M4->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_M4->text(), ui->comboBox_M4->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::MAINTENANCE_WASHERFLUIDLEVEL:
        {
            ui->comboBox_M5->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_M5->text(), ui->comboBox_M5->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::MAINTENANCE_MALFUNCTIONINDICATORLAMP:
        {
            ui->comboBox_M6->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_M6->text(), ui->comboBox_M6->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::MAINTENANCE_BATTERY_VOLTAGE:
        {
            addLog(m_itemModel, "Receive", ui->label_M7->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::MAINTENANCE_BATTERY_CURRENT:
        {
            addLog(m_itemModel, "Receive", ui->label_M8->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::MAINTENANCE_TIREPRESSURE_FRONTLEFT:
        {
            addLog(m_itemModel, "Receive", ui->label_M9->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::MAINTENANCE_TIREPRESSURE_FRONTRIGHT:
        {
            addLog(m_itemModel, "Receive", ui->label_M10->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::MAINTENANCE_TIREPRESSURE_REARLEFT:
        {
            addLog(m_itemModel, "Receive", ui->label_M11->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::MAINTENANCE_TIREPRESSURE_REARRIGHT:
        {
            addLog(m_itemModel, "Receive", ui->label_M12->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::MAINTENANCE_TIREPRESSURESTATUS_FRONTLEFT:
        {
            ui->comboBox_M13->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_M13->text(), ui->comboBox_M13->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::MAINTENANCE_TIREPRESSURESTATUS_FRONTRIGHT:
        {
            ui->comboBox_M14->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_M14->text(), ui->comboBox_M14->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::MAINTENANCE_TIREPRESSURESTATUS_REARLEFT:
        {
            ui->comboBox_M15->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_M15->text(), ui->comboBox_M15->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::MAINTENANCE_TIREPRESSURESTATUS_REARRIGHT:
        {
            ui->comboBox_M16->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_M16->text(), ui->comboBox_M16->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_VEHICLEPOWERMODE:
        {
            ui->comboBox_R1->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_R1->text(), ui->comboBox_R1->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_SPEEDOMETER:
        {
            addLog(m_itemModel, "Receive", ui->label_R2->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_ENGINESPEED:
        {
            addLog(m_itemModel, "Receive", ui->label_R3->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_TRIPMETER_1_MILEAGE:
        {
            addLog(m_itemModel, "Receive", ui->label_R4->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_TRIPMETER_2_MILEAGE:
        {
            addLog(m_itemModel, "Receive", ui->label_R5->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_TRIPMETER_1_AVERAGESPEED:
        {
            addLog(m_itemModel, "Receive", ui->label_R6->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_TRIPMETER_2_AVERAGESPEED:
        {
            addLog(m_itemModel, "Receive", ui->label_R7->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_TRIPMETER_1_FUELCONSUMPTION:
        {
            addLog(m_itemModel, "Receive", ui->label_R8->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_TRIPMETER_2_FUELCONSUMPTION:
        {
            addLog(m_itemModel, "Receive", ui->label_R9->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_TRANSMISSIONGEARSTATUS:
        {
            if (m_index == 32)
			{
                m_index = 11;
			}
            else if (m_index == 64)
			{
                m_index = 12;
			}
            else if (m_index == 128)
			{
                m_index = 13;
			}
            ui->comboBox_R10->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_R10->text(), ui->comboBox_R10->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_CRUISECONTROL_STATUS:
        {
            ui->comboBox_R11->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_R11->text(), ui->comboBox_R11->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_CRUISECONTROL_SPEED:
        {
            addLog(m_itemModel, "Receive", ui->label_R12->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_WHEELBRAKE:
        {
            ui->comboBox_R13->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_R13->text(), ui->comboBox_R13->currentText(), QDateTime::currentDateTime());
            break;
        }
        case  genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_HEAD:
        {
            ui->comboBox_R14->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_R14->text(), ui->comboBox_R14->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_HIGHBEAM:
        {
            ui->comboBox_R15->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_R15->text(), ui->comboBox_R15->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_TURNLEFT:
        {
            ui->comboBox_R16->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_R16->text(), ui->comboBox_R16->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_TURNRIGHT:
        {
            ui->comboBox_R17->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_R17->text(), ui->comboBox_R17->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_BRAKE:
        {
            ui->comboBox_R18->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_R18->text(), ui->comboBox_R18->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_FOGFRONT:
        {
            ui->comboBox_R19->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_R19->text(), ui->comboBox_R19->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_FOGREAR:
        {
            ui->comboBox_R20->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_R20->text(), ui->comboBox_R20->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_HAZARD:
        {
            ui->comboBox_R21->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_R21->text(), ui->comboBox_R21->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_PARKING:
        {
            ui->comboBox_R22->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_R22->text(), ui->comboBox_R22->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_INTERIORLIGHTSSTATUS_DRIVER:
        {
            ui->comboBox_R23->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_R23->text(), ui->comboBox_R23->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_INTERIORLIGHTSSTATUS_PASSENGER:
        {
            ui->comboBox_R24->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_R24->text(), ui->comboBox_R24->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_INTERIORLIGHTSSTATUS_CENTER:
        {
            ui->comboBox_R25->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_R25->text(), ui->comboBox_R25->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_AUTOMATICHEADLIGHTS:
        {
            ui->comboBox_R26->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_R26->text(), ui->comboBox_R26->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_DYNAMICHIGHBEAM:
        {
            ui->comboBox_R27->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_R27->text(), ui->comboBox_R27->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_HORN:
        {
            ui->comboBox_R28->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_R28->text(), ui->comboBox_R28->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_CHIME:
        {
            ui->comboBox_R29->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_R29->text(), ui->comboBox_R29->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_FUEL:
        {
            addLog(m_itemModel, "Receive", ui->label_R30->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_ESTIMATEDRANGE:
        {
            addLog(m_itemModel, "Receive", ui->label_R31->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_ENGINEOIL_REMAINING:
        {
            addLog(m_itemModel, "Receive", ui->label_R32->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_ENGINEOIL_CHANGE:
        {
            ui->comboBox_R33->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_R33->text(), ui->comboBox_R33->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_ENGINEOIL_TEMP:
        {
            addLog(m_itemModel, "Receive", ui->label_R34->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_ENGINECOOLANT_LEVEL:
        {
            ui->comboBox_R35->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_R35->text(), ui->comboBox_R35->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_ENGINECOOLANT_TEMP:
        {
            addLog(m_itemModel, "Receive", ui->label_R36->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::RUNNINGSTATUS_STEERINGWHEELANGLE:
        {
            addLog(m_itemModel, "Receive", ui->label_R37->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_KEYID:
        {
            addLog(m_itemModel, "Receive", ui->label_P1->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_LANGUAGE:
        {
            ui->comboBox_P2->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_P2->text(), ui->comboBox_P2->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_MEASUREMENTSYSTEM:
        {
            ui->comboBox_P3->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_P3->text(), ui->comboBox_P3->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_FUEL:
        {
            addLog(m_itemModel, "Receive", ui->label_P4->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_DISTANCE:
        {
            addLog(m_itemModel, "Receive", ui->label_P5->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_SPEED:
        {
            addLog(m_itemModel, "Receive", ui->label_P6->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_CONSUMPTION:
        {
            addLog(m_itemModel, "Receive", ui->label_P7->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_MIRROR_DRIVER:
        {
            addLog(m_itemModel, "Receive", ui->label_P8->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_MIRROR_PASSENGER:
        {
            addLog(m_itemModel, "Receive", ui->label_P9->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_MIRROR_INSIDE:
        {
            addLog(m_itemModel, "Receive", ui->label_P10->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_STEERINGWHEELPOSITION_SLIDE:
        {
            addLog(m_itemModel, "Receive", ui->label_P11->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_STEERINGWHEELPOSITION_TILT:
        {
            addLog(m_itemModel, "Receive", ui->label_P12->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_DRIVINGMODE:
        {
            ui->comboBox_P13->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_P13->text(), ui->comboBox_P13->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_RECLINESEATBACK:
        {
            addLog(m_itemModel, "Receive", ui->label_P14->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_SLIDE:
        {
            addLog(m_itemModel, "Receive", ui->label_P15->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_CUSHIONHEIGHT:
        {
            addLog(m_itemModel, "Receive", ui->label_P16->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_HEADREST:
        {
            addLog(m_itemModel, "Receive", ui->label_P17->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_BACKCUSHION:
        {
            addLog(m_itemModel, "Receive", ui->label_P18->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_SIDECUSHION:
        {
            addLog(m_itemModel, "Receive", ui->label_P19->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_RECLINESEATBACK:
        {
            addLog(m_itemModel, "Receive", ui->label_P20->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_SLIDE:
        {
            addLog(m_itemModel, "Receive", ui->label_P21->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_CUSHIONHEIGHT:
        {
            addLog(m_itemModel, "Receive", ui->label_P22->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_HEADREST:
        {
            addLog(m_itemModel, "Receive", ui->label_P23->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_BACKCUSHION:
        {
            addLog(m_itemModel, "Receive", ui->label_P24->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_SIDECUSHION:
        {
            addLog(m_itemModel, "Receive", ui->label_P25->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_DASHBOARDILLUMINATION:
        {
            addLog(m_itemModel, "Receive", ui->label_P26->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PERSONALIZATION_GENERATEDVEHICLESOUNDMODE:
        {
            ui->comboBox_P27->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_P27->text(), ui->comboBox_P27->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_ANTILOCKBRAKINGSYSTEM:
        {
            ui->comboBox_D1->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_D1->text(), ui->comboBox_D1->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_TRACTIONCONTROLSYSTEM:
        {
            ui->comboBox_D2->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_D2->text(), ui->comboBox_D2->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_ELECTRONICSTABILITYCONTROL:
        {
            ui->comboBox_D3->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_D3->text(), ui->comboBox_D3->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_VEHICLETOPSPEEDLIMIT:
        {
            addLog(m_itemModel, "Receive", ui->label_D4->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_AIRBAGSTATUS_DRIVER:
        {
            ui->comboBox_D5->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_D5->text(), ui->comboBox_D5->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_AIRBAGSTATUS_PASSENGER:
        {
            ui->comboBox_D6->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_D6->text(), ui->comboBox_D6->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_AIRBAGSTATUS_SIDE:
        {
            ui->comboBox_D7->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_D7->text(), ui->comboBox_D7->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_DOOROPENSTATUS_DRIVER:
        {
            ui->comboBox_D8->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_D8->text(), ui->comboBox_D8->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_DOOROPENSTATUS_PASSENGER:
        {
            ui->comboBox_D9->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_D9->text(), ui->comboBox_D9->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_DOOROPENSTATUS_REARLEFT:
        {
            ui->comboBox_D10->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_D10->text(), ui->comboBox_D10->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_DOOROPENSTATUS_REARRIGHT:
        {
            ui->comboBox_D11->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_D11->text(), ui->comboBox_D11->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_DOOROPENSTATUS_TRUNK:
        {
            ui->comboBox_D12->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_D12->text(), ui->comboBox_D12->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_DOOROPENSTATUS_FUELFILTERCAP:
        {
            ui->comboBox_D13->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_D13->text(), ui->comboBox_D13->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_DOOROPENSTATUS_HOOD:
        {
            ui->comboBox_D14->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_D14->text(), ui->comboBox_D14->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_DOORLOCKSTATUS_DRIVER:
        {
            ui->comboBox_D15->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_D15->text(), ui->comboBox_D15->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_DOORLOCKSTATUS_PASSENGER:
        {
            ui->comboBox_D16->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_D16->text(), ui->comboBox_D16->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_DOORLOCKSTATUS_REARLEFT:
        {
            ui->comboBox_D17->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_D17->text(), ui->comboBox_D17->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_DOORLOCKSTATUS_REARRIGHT:
        {
            ui->comboBox_D18->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_D18->text(), ui->comboBox_D18->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_CHILDSAFETYLOCK:
        {
            ui->comboBox_D19->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_D19->text(), ui->comboBox_D19->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_OCCUPANTSSTATUS_DRIVER:
        {
            ui->comboBox_D20->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_D20->text(), ui->comboBox_D20->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_OCCUPANTSSTATUS_PASSENGER:
        {
            ui->comboBox_D21->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_D21->text(), ui->comboBox_D21->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_OCCUPANTSSTATUS_REARLEFT:
        {
            ui->comboBox_D22->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_D22->text(), ui->comboBox_D22->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_OCCUPANTSSTATUS_REARRIGHT:
        {
            ui->comboBox_D23->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_D23->text(), ui->comboBox_D23->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_SEATBELT_DRIVER:
        {
            ui->comboBox_D24->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_D24->text(), ui->comboBox_D24->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_SEATBELT_PASSENGER:
        {
            ui->comboBox_D25->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_D25->text(), ui->comboBox_D25->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_SEATBELT_REARLEFT:
        {
            ui->comboBox_D26->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_D26->text(), ui->comboBox_D26->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_SEATBELT_REARRIGHT:
        {
            ui->comboBox_D27->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_D27->text(), ui->comboBox_D27->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_WINDOWLOCK_DRIVER:
        {
            ui->comboBox_D28->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_D28->text(), ui->comboBox_D28->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_WINDOWLOCK_PASSENGER:
        {
            ui->comboBox_D29->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_D29->text(), ui->comboBox_D29->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_WINDOWLOCK_REARLEFT:
        {
            ui->comboBox_D30->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_D30->text(), ui->comboBox_D30->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_WINDOWLOCK_REARRIGHT:
        {
            ui->comboBox_D31->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_D31->text(), ui->comboBox_D31->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_SENSORSTATUS:
        {
            ui->comboBox_D32->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_D32->text(), ui->comboBox_D32->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTCENTER:
        {
            addLog(m_itemModel, "Receive", ui->label_D33->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_REARCENTER:
        {
            addLog(m_itemModel, "Receive", ui->label_D34->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTLEFT:
        {
            addLog(m_itemModel, "Receive", ui->label_D35->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTRIGHT:
        {
            addLog(m_itemModel, "Receive", ui->label_D36->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLELEFT:
        {
            addLog(m_itemModel, "Receive", ui->label_D37->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLERIGHT:
        {
            addLog(m_itemModel, "Receive", ui->label_D38->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_REARLEFT:
        {
            addLog(m_itemModel, "Receive", ui->label_D39->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_REARRIGHT:
        {
            addLog(m_itemModel, "Receive", ui->label_D40->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_FRONTCOLLISIONDETECTION_STATUS:
        {
            ui->comboBox_D41->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_D41->text(), ui->comboBox_D41->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_FRONTCOLLISIONDETECTION_DISTANCE:
        {
            addLog(m_itemModel, "Receive", ui->label_D42->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::DRIVINGSAFETY_FRONTCOLLISIONDETECTION_TIME:
        {
            addLog(m_itemModel, "Receive", ui->label_D43->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_INTERIORTEMP:
        {
            addLog(m_itemModel, "Receive", ui->label_C1->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_EXTERIORTEMP:
        {
            addLog(m_itemModel, "Receive", ui->label_C2->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_EXTERIORBRIGHTNESS:
        {
            addLog(m_itemModel, "Receive", ui->label_C3->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_RAINSENSOR:
        {
            ui->comboBox_C4->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_C4->text(), ui->comboBox_C4->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_WINDSHIELDWIPER:
        {
            if (m_index == 10)
			{
                m_index = 6;
			}
            ui->comboBox_C5->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_C5->text(), ui->comboBox_C5->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_REARWIPER:
        {
            if (m_index == 10)
			{
                m_index = 6;
			}
            ui->comboBox_C6->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_C6->text(), ui->comboBox_C6->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_HVACFAN_DIRECTION:
        {
            ui->comboBox_C7->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_C7->text(), ui->comboBox_C7->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_HVACFAN_SPEED:
        {
            addLog(m_itemModel, "Receive", ui->label_C8->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_HVACFAN_TARGETTEMP:
        {
            addLog(m_itemModel, "Receive", ui->label_C9->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_AIRCONDITIONING:
        {
            ui->comboBox_C10->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_C10->text(), ui->comboBox_C10->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_AIRRECIRCULATION:
        {
            ui->comboBox_C11->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_C11->text(), ui->comboBox_C11->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_HEATER:
        {
            ui->comboBox_C12->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_C12->text(), ui->comboBox_C12->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_DEFROST_WINDSHIELD:
        {
            ui->comboBox_C13->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_C13->text(), ui->comboBox_C13->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_DEFROST_REARWINDOW:
        {
            ui->comboBox_C14->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_C14->text(), ui->comboBox_C14->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_DEFROST_SIDEMIRRORS:
        {
            ui->comboBox_C15->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_C15->text(), ui->comboBox_C15->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_STEERINGWHEELHEATER:
        {
            ui->comboBox_C16->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_C16->text(), ui->comboBox_C16->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_SEATHEATER:
        {
            ui->comboBox_C17->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_C17->text(), ui->comboBox_C17->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_SEATCOOLER:
        {
            ui->comboBox_C18->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_C18->text(), ui->comboBox_C18->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_WINDOW_DRIVER:
        {
            addLog(m_itemModel, "Receive", ui->label_C19->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_WINDOW_PASSENGER:
        {
            addLog(m_itemModel, "Receive", ui->label_C20->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_WINDOW_REARLEFT:
        {
            addLog(m_itemModel, "Receive", ui->label_C21->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_WINDOW_REARRIGHT:
        {
            addLog(m_itemModel, "Receive", ui->label_C22->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_SUNROOF_OPENNESS:
        {
            addLog(m_itemModel, "Receive", ui->label_C23->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_SUNROOF_TILT:
        {
            addLog(m_itemModel, "Receive", ui->label_C24->text(), datagram.mid(3).data(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::CLIMATEENVIRONMENT_CONVERTIBLEROOF:
        {
            ui->comboBox_C25->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_C25->text(), ui->comboBox_C25->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::VISIONSYSTEM_LANEDEPARTUREDETECTIONSTATUS:
        {
            ui->comboBox_VS1->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_VS1->text(), ui->comboBox_VS1->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::VISIONSYSTEM_LANEDEPARTED:
        {
            ui->comboBox_VS2->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_VS2->text(), ui->comboBox_VS2->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PARKING_SECURITYALERT:
        {
            ui->comboBox_PA1->setCurrentIndex(m_index - 1);
            addLog(m_itemModel, "Receive", ui->label_PA1->text(), ui->comboBox_PA1->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PARKING_PARKINGBRAKE:
        {
            ui->comboBox_PA2->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_PA2->text(), ui->comboBox_PA2->currentText(), QDateTime::currentDateTime());
            break;
        }
        case genivi::wavd::socket::PARKING_PARKINGLIGHTS:
        {
            ui->comboBox_PA3->setCurrentIndex(m_index);
            addLog(m_itemModel, "Receive", ui->label_PA3->text(), ui->comboBox_PA3->currentText(), QDateTime::currentDateTime());
            break;
        }
        }
    }
}

// APP INTEROPERABILITY
void MainWindow::on_pushButton_A1_clicked()
{
    QByteArray byteArray = ui->lineEdit_A1->text().toLatin1();
    sprintf(msgData, "%03d%s", genivi::wavd::socket::VEHICLEINFO_WMI, byteArray.data());
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_A1->text(), ui->lineEdit_A1->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_A2_clicked()
{
    QByteArray byteArray = ui->lineEdit_A2->text().toLatin1();
    sprintf(msgData, "%03d%s", genivi::wavd::socket::VEHICLEINFO_VIN, byteArray.data());
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_A2->text(), ui->lineEdit_A2->text(), QDateTime::currentDateTime());
}

// Vehicle Info
void MainWindow::on_pushButton_V1_clicked()
{
    unsigned short index = ui->comboBox_V1->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::VEHICLEINFO_VEHICLETYPE, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_V1->text(), ui->comboBox_V1->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_V2_clicked()
{
    unsigned short index = ui->lineEdit_V2->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::VEHICLEINFO_DOORTYPE_1STROW, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_V2->text(), ui->lineEdit_V2->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_V3_clicked()
{
    unsigned short index = ui->lineEdit_V3->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::VEHICLEINFO_DOORTYPE_2NDROW, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_V3->text(), ui->lineEdit_V3->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_V4_clicked()
{
    unsigned short index = ui->lineEdit_V4->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::VEHICLEINFO_DOORTYPE_3RDROW, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_V4->text(), ui->lineEdit_V4->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_V5_clicked()
{
    unsigned short index = ui->comboBox_V5->currentIndex();
    quint8 gearType = 0x00;

    switch (index)
    {
    case 0:
    {
        gearType = 0x01;
        break;
    }
    case 1:
    {
        gearType = 0x02;
        break;
    }
    case 2:
    {
        gearType = 0x03;
        break;
    }
    case 3:
    {
        gearType = 0x04;
        break;
    }
    case 4:
    {
        gearType = 0x05;
        break;
    }
    case 5:
    {
        gearType = 0x06;
        break;
    }
    case 6:
    {
        gearType = 0x07;
        break;
    }
    case 7:
    {
        gearType = 0x08;
        break;
    }
    case 8:
    {
        gearType = 0x09;
        break;
    }
    case 9:
    {
        gearType = 0x0a;
        break;
    }
    case 10:
    {
        gearType = 0x0b;
        break;
    }
    case 11:
    {
        gearType = 0x0c;
        break;
    }
    case 12:
    {
        gearType = 0x0d;
        break;
    }
    case 13:
    {
        gearType = 0x0e;
        break;
    }
    case 14:
    {
        gearType = 0x0f;
        break;
    }
    case 15:
    {
        gearType = 0x10;
        break;
    }
    case 16:
    {
        gearType = 0x11;
        break;
    }
    case 17:
    {
        gearType = 0x12;
        break;
    }
    case 18:
    {
        gearType = 0x13;
        break;
    }
    case 19:
    {
        gearType = 0x14;
        break;
    }
    case 20:
    {
        gearType = 0x15;
        break;
    }
    case 21:
    {
        gearType = 0x16;
        break;
    }
    }
    sprintf(msgData, "%03d%d", genivi::wavd::socket::VEHICLEINFO_FUELTYPE, gearType);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_V5->text(), ui->comboBox_V5->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_V6_clicked()
{
    int index = ui->comboBox_V6->currentIndex() + 1;
    sprintf(msgData, "%03d%x", genivi::wavd::socket::VEHICLEINFO_TRANSMISSIONGEARTYPE, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_V6->text(), ui->comboBox_V6->currentText(), QDateTime::currentDateTime());
}

void MainWindow::on_pushButton_V7_clicked()
{
    double index = ui->lineEdit_V7->text().toDouble();
    sprintf(msgData, "%03d%f", genivi::wavd::socket::VEHICLEINFO_WHEELINFO_RADIUS, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_V7->text(), ui->lineEdit_V7->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_V8_clicked()
{
    double index = ui->lineEdit_V8->text().toDouble();
    sprintf(msgData, "%03d%f", genivi::wavd::socket::VEHICLEINFO_WHEELINFO_TRACK, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_V8->text(), ui->lineEdit_V8->text(), QDateTime::currentDateTime());
}

// Maintenance
void MainWindow::on_pushButton_M1_clicked()
{
    unsigned long index = ui->lineEdit_M1->text().toULong();
    sprintf(msgData, "%03d%lu", genivi::wavd::socket::MAINTENANCE_ODOMETER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_M1->text(), ui->lineEdit_M1->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_M2_clicked()
{
    int index = ui->comboBox_M2->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::MAINTENANCE_TRANSMISSIONOIL_LIFELEVEL, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_M2->text(), ui->comboBox_M2->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_M3_clicked()
{
    short index = ui->lineEdit_M3->text().toShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::MAINTENANCE_TRANSMISSIONOIL_TEMP, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_M3->text(), ui->lineEdit_M3->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_M4_clicked()
{
    int index = ui->comboBox_M4->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::MAINTENANCE_BRAKEFLUIDLEVEL, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_M4->text(), ui->comboBox_M4->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_M5_clicked()
{
    int index = ui->comboBox_M5->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::MAINTENANCE_WASHERFLUIDLEVEL, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_M5->text(), ui->comboBox_M5->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_M6_clicked()
{
    int index = ui->comboBox_M6->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::MAINTENANCE_MALFUNCTIONINDICATORLAMP, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_M6->text(), ui->comboBox_M6->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_M7_clicked()
{
    unsigned short index = ui->lineEdit_M7->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::MAINTENANCE_BATTERY_VOLTAGE, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_M7->text(), ui->lineEdit_M7->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_M8_clicked()
{
    unsigned short index = ui->lineEdit_M8->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::MAINTENANCE_BATTERY_CURRENT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_M8->text(), ui->lineEdit_M8->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_M9_clicked()
{
    unsigned short index = ui->lineEdit_M9->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::MAINTENANCE_TIREPRESSURE_FRONTLEFT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_M9->text(), ui->lineEdit_M9->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_M10_clicked()
{
    unsigned short index = ui->lineEdit_M10->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::MAINTENANCE_TIREPRESSURE_FRONTRIGHT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_M10->text(), ui->lineEdit_M10->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_M11_clicked()
{
    unsigned short index = ui->lineEdit_M11->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::MAINTENANCE_TIREPRESSURE_REARLEFT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_M11->text(), ui->lineEdit_M11->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_M12_clicked()
{
    unsigned short index = ui->lineEdit_M12->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::MAINTENANCE_TIREPRESSURE_REARRIGHT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_M12->text(), ui->lineEdit_M12->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_M13_clicked()
{
    int index = ui->comboBox_M13->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::MAINTENANCE_TIREPRESSURESTATUS_FRONTLEFT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_M13->text(), ui->comboBox_M13->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_M14_clicked()
{
    int index = ui->comboBox_M14->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::MAINTENANCE_TIREPRESSURESTATUS_FRONTRIGHT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_M14->text(), ui->comboBox_M14->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_M15_clicked()
{
    int index = ui->comboBox_M15->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::MAINTENANCE_TIREPRESSURESTATUS_REARLEFT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_M15->text(), ui->comboBox_M15->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_M16_clicked()
{
    int index = ui->comboBox_M16->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::MAINTENANCE_TIREPRESSURESTATUS_REARRIGHT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_M16->text(), ui->comboBox_M16->currentText(), QDateTime::currentDateTime());
}

// Running Status
void MainWindow::on_pushButton_R1_clicked()
{
    int index = ui->comboBox_R1->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_VEHICLEPOWERMODE, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R1->text(), ui->comboBox_R1->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R2_clicked()
{
    unsigned short index = ui->lineEdit_R2->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_SPEEDOMETER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R2->text(), ui->lineEdit_R2->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R3_clicked()
{
    unsigned short index = ui->lineEdit_R3->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_ENGINESPEED, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R3->text(), ui->lineEdit_R3->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R4_clicked()
{
    unsigned long index = ui->lineEdit_R4->text().toULong();
    sprintf(msgData, "%03d%lu", genivi::wavd::socket::RUNNINGSTATUS_TRIPMETER_1_MILEAGE, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R4->text(), ui->lineEdit_R4->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R5_clicked()
{
    unsigned long index = ui->lineEdit_R5->text().toULong();
    sprintf(msgData, "%03d%lu", genivi::wavd::socket::RUNNINGSTATUS_TRIPMETER_2_MILEAGE, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R5->text(), ui->lineEdit_R5->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R6_clicked()
{
    unsigned short index = ui->lineEdit_R6->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_TRIPMETER_1_AVERAGESPEED, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R6->text(), ui->lineEdit_R6->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R7_clicked()
{
    unsigned short index = ui->lineEdit_R7->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_TRIPMETER_2_AVERAGESPEED, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R7->text(), ui->lineEdit_R7->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R8_clicked()
{
    unsigned long index = ui->lineEdit_R8->text().toULong();
    sprintf(msgData, "%03d%lu", genivi::wavd::socket::RUNNINGSTATUS_TRIPMETER_1_FUELCONSUMPTION, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R8->text(), ui->lineEdit_R8->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R9_clicked()
{
    unsigned long index = ui->lineEdit_R9->text().toULong();
    sprintf(msgData, "%03d%lu", genivi::wavd::socket::RUNNINGSTATUS_TRIPMETER_2_FUELCONSUMPTION, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R9->text(), ui->lineEdit_R9->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R10_clicked()
{
    int index = ui->comboBox_R10->currentIndex();

    if (index == 21)
	{
        index = 32;
	}
    else if (index == 22)
	{
        index = 64;
	}
    else if (index == 23)
	{
        index = 128;
	}
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_TRANSMISSIONGEARSTATUS, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R10->text(), ui->comboBox_R10->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R11_clicked()
{
    int index = ui->comboBox_R11->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_CRUISECONTROL_STATUS, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R11->text(), ui->comboBox_R11->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R12_clicked()
{
    unsigned short index = ui->lineEdit_R12->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_CRUISECONTROL_SPEED, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R12->text(), ui->lineEdit_R12->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R13_clicked()
{
    int index = ui->comboBox_R13->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_WHEELBRAKE, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R13->text(), ui->comboBox_R13->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R14_clicked()
{
    int index = ui->comboBox_R14->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_HEAD, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R14->text(), ui->comboBox_R14->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R15_clicked()
{
    int index = ui->comboBox_R15->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_HIGHBEAM, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R15->text(), ui->comboBox_R15->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R16_clicked()
{
    int index = ui->comboBox_R16->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_TURNLEFT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R16->text(), ui->comboBox_R16->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R17_clicked()
{
    int index = ui->comboBox_R17->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_TURNRIGHT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R17->text(), ui->comboBox_R17->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R18_clicked()
{
    int index = ui->comboBox_R18->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_BRAKE, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R18->text(), ui->comboBox_R18->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R19_clicked()
{
    int index = ui->comboBox_R19->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_FOGFRONT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R19->text(), ui->comboBox_R19->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R20_clicked()
{
    int index = ui->comboBox_R20->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_FOGREAR, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R20->text(), ui->comboBox_R20->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R21_clicked()
{
    int index = ui->comboBox_R21->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_HAZARD, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R21->text(), ui->comboBox_R21->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R22_clicked()
{
    int index = ui->comboBox_R22->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_LIGHTSSTATUS_PARKING, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R22->text(), ui->comboBox_R22->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R23_clicked()
{
    int index = ui->comboBox_R23->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_INTERIORLIGHTSSTATUS_DRIVER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R23->text(), ui->comboBox_R23->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R24_clicked()
{
    int index = ui->comboBox_R24->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_INTERIORLIGHTSSTATUS_PASSENGER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R24->text(), ui->comboBox_R24->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R25_clicked()
{
    int index = ui->comboBox_R25->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_INTERIORLIGHTSSTATUS_CENTER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R25->text(), ui->comboBox_R25->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R26_clicked()
{
    int index = ui->comboBox_R26->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_AUTOMATICHEADLIGHTS, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R26->text(), ui->comboBox_R26->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R27_clicked()
{
    int index = ui->comboBox_R27->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_DYNAMICHIGHBEAM, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R27->text(), ui->comboBox_R27->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R28_clicked()
{
    int index = ui->comboBox_R28->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_HORN, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R28->text(), ui->comboBox_R28->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R29_clicked()
{
    int index = ui->comboBox_R29->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_CHIME, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R29->text(), ui->comboBox_R29->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R30_clicked()
{
    unsigned short index = ui->lineEdit_R30->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_FUEL, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R30->text(), ui->lineEdit_R30->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R31_clicked()
{
    unsigned long index = ui->lineEdit_R31->text().toULong();
    sprintf(msgData, "%03d%lu", genivi::wavd::socket::RUNNINGSTATUS_ESTIMATEDRANGE, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R31->text(), ui->lineEdit_R31->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R32_clicked()
{
    unsigned short index = ui->lineEdit_R32->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_ENGINEOIL_REMAINING, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R32->text(), ui->lineEdit_R32->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R33_clicked()
{
    int index = ui->comboBox_R33->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_ENGINEOIL_CHANGE, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R33->text(), ui->comboBox_R33->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R34_clicked()
{
    short index = ui->lineEdit_R34->text().toShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_ENGINEOIL_TEMP, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R34->text(), ui->lineEdit_R34->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R35_clicked()
{
    int index = ui->comboBox_R35->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_ENGINECOOLANT_LEVEL, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R35->text(), ui->comboBox_R35->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R36_clicked()
{
    short index = ui->lineEdit_R36->text().toShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_ENGINECOOLANT_TEMP, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R36->text(), ui->lineEdit_R36->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_R37_clicked()
{
    short index = ui->lineEdit_R37->text().toShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::RUNNINGSTATUS_STEERINGWHEELANGLE, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_R37->text(), ui->lineEdit_R37->text(), QDateTime::currentDateTime());
}

// Personalization
void MainWindow::on_pushButton_P1_clicked()
{
    QByteArray byteArray = ui->lineEdit_P1->text().toLatin1();
    sprintf(msgData, "%03d%s", genivi::wavd::socket::PERSONALIZATION_KEYID, byteArray.data());
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P1->text(), ui->lineEdit_P1->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P2_clicked()
{
    int index = ui->comboBox_P2->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PERSONALIZATION_LANGUAGE, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P2->text(), ui->comboBox_P2->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P3_clicked()
{
    int index = ui->comboBox_P3->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PERSONALIZATION_MEASUREMENTSYSTEM, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P3->text(), ui->comboBox_P3->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P4_clicked()
{
    QByteArray byteArray = ui->lineEdit_P4->text().toLatin1();
    sprintf(msgData, "%03d%s", genivi::wavd::socket::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_FUEL, byteArray.data());
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P4->text(), ui->lineEdit_P4->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P5_clicked()
{
    QByteArray byteArray = ui->lineEdit_P5->text().toLatin1();
    sprintf(msgData, "%03d%s", genivi::wavd::socket::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_DISTANCE, byteArray.data());
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P5->text(), ui->lineEdit_P5->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P6_clicked()
{
    QByteArray byteArray = ui->lineEdit_P6->text().toLatin1();
    sprintf(msgData, "%03d%s", genivi::wavd::socket::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_SPEED, byteArray.data());
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P6->text(), ui->lineEdit_P6->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P7_clicked()
{
    QByteArray byteArray = ui->lineEdit_P7->text().toLatin1();
    sprintf(msgData, "%03d%s", genivi::wavd::socket::PERSONALIZATION_MEASUREMENTSYSTEMSTRING_CONSUMPTION, byteArray.data());
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P7->text(), ui->lineEdit_P7->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P8_clicked()
{
    unsigned short index = ui->lineEdit_P8->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PERSONALIZATION_MIRROR_DRIVER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P8->text(), ui->lineEdit_P8->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P9_clicked()
{
    unsigned short index = ui->lineEdit_P9->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PERSONALIZATION_MIRROR_PASSENGER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P9->text(), ui->lineEdit_P9->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P10_clicked()
{
    unsigned short index = ui->lineEdit_P10->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PERSONALIZATION_MIRROR_INSIDE, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P10->text(), ui->lineEdit_P10->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P11_clicked()
{
    unsigned short index = ui->lineEdit_P11->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PERSONALIZATION_STEERINGWHEELPOSITION_SLIDE, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P11->text(), ui->lineEdit_P11->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P12_clicked()
{
    unsigned short index = ui->lineEdit_P12->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PERSONALIZATION_STEERINGWHEELPOSITION_TILT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P12->text(), ui->lineEdit_P12->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P13_clicked()
{
    int index = ui->comboBox_P13->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PERSONALIZATION_DRIVINGMODE, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P13->text(), ui->comboBox_P13->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P14_clicked()
{
    unsigned short index = ui->lineEdit_P14->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_RECLINESEATBACK, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P14->text(), ui->lineEdit_P14->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P15_clicked()
{
    unsigned short index = ui->lineEdit_P15->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_SLIDE, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P15->text(), ui->lineEdit_P15->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P16_clicked()
{
    unsigned short index = ui->lineEdit_P16->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_CUSHIONHEIGHT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P16->text(), ui->lineEdit_P16->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P17_clicked()
{
    unsigned short index = ui->lineEdit_P17->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_HEADREST, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P17->text(), ui->lineEdit_P17->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P18_clicked()
{
    unsigned short index = ui->lineEdit_P18->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_BACKCUSHION, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P18->text(), ui->lineEdit_P18->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P19_clicked()
{
    unsigned short index = ui->lineEdit_P19->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PERSONALIZATION_DRIVERSEATPOSITION_SIDECUSHION, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P19->text(), ui->lineEdit_P19->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P20_clicked()
{
    unsigned short index = ui->lineEdit_P20->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_RECLINESEATBACK, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P20->text(), ui->lineEdit_P20->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P21_clicked()
{
    unsigned short index = ui->lineEdit_P21->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_SLIDE, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P21->text(), ui->lineEdit_P21->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P22_clicked()
{
    unsigned short index = ui->lineEdit_P22->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_CUSHIONHEIGHT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P22->text(), ui->lineEdit_P22->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P23_clicked()
{
    unsigned short index = ui->lineEdit_P23->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_HEADREST, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P23->text(), ui->lineEdit_P23->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P24_clicked()
{
    unsigned short index = ui->lineEdit_P24->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_BACKCUSHION, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P24->text(), ui->lineEdit_P24->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P25_clicked()
{
    unsigned short index = ui->lineEdit_P25->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PERSONALIZATION_PASSENGERSEATPOSITION_SIDECUSHION, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P25->text(), ui->lineEdit_P25->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P26_clicked()
{
    unsigned short index = ui->lineEdit_P26->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PERSONALIZATION_DASHBOARDILLUMINATION, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P26->text(), ui->lineEdit_P26->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_P27_clicked()
{
    int index = ui->comboBox_P27->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PERSONALIZATION_GENERATEDVEHICLESOUNDMODE, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_P27->text(), ui->comboBox_P27->currentText(), QDateTime::currentDateTime());
}

// Driving Safety
void MainWindow::on_pushButton_D1_clicked()
{
    int index = ui->comboBox_D1->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_ANTILOCKBRAKINGSYSTEM, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D1->text(), ui->comboBox_D1->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D2_clicked()
{
    int index = ui->comboBox_D2->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_TRACTIONCONTROLSYSTEM, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D2->text(), ui->comboBox_D2->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D3_clicked()
{
    int index = ui->comboBox_D3->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_ELECTRONICSTABILITYCONTROL, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D3->text(), ui->comboBox_D3->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D4_clicked()
{
    unsigned short index = ui->lineEdit_D4->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_VEHICLETOPSPEEDLIMIT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D4->text(), ui->lineEdit_D4->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D5_clicked()
{
    int index = ui->comboBox_D5->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_AIRBAGSTATUS_DRIVER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D5->text(), ui->comboBox_D5->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D6_clicked()
{
    int index = ui->comboBox_D6->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_AIRBAGSTATUS_PASSENGER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D6->text(), ui->comboBox_D6->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D7_clicked()
{
    int index = ui->comboBox_D7->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_AIRBAGSTATUS_SIDE, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D7->text(), ui->comboBox_D7->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D8_clicked()
{
    int index = ui->comboBox_D8->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_DOOROPENSTATUS_DRIVER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D8->text(), ui->comboBox_D8->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D9_clicked()
{
    int index = ui->comboBox_D9->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_DOOROPENSTATUS_PASSENGER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D9->text(), ui->comboBox_D9->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D10_clicked()
{
    int index = ui->comboBox_D10->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_DOOROPENSTATUS_REARLEFT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D10->text(), ui->comboBox_D10->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D11_clicked()
{
    int index = ui->comboBox_D11->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_DOOROPENSTATUS_REARRIGHT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D11->text(), ui->comboBox_D11->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D12_clicked()
{
    int index = ui->comboBox_D12->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_DOOROPENSTATUS_TRUNK, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D12->text(), ui->comboBox_D12->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D13_clicked()
{
    int index = ui->comboBox_D13->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_DOOROPENSTATUS_FUELFILTERCAP, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D13->text(), ui->comboBox_D13->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D14_clicked()
{
    int index = ui->comboBox_D14->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_DOOROPENSTATUS_HOOD, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D14->text(), ui->comboBox_D14->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D15_clicked()
{
    int index = ui->comboBox_D15->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_DOORLOCKSTATUS_DRIVER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D15->text(), ui->comboBox_D15->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D16_clicked()
{
    int index = ui->comboBox_D16->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_DOORLOCKSTATUS_PASSENGER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D16->text(), ui->comboBox_D16->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D17_clicked()
{
    int index = ui->comboBox_D17->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_DOORLOCKSTATUS_REARLEFT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D17->text(), ui->comboBox_D17->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D18_clicked()
{
    int index = ui->comboBox_D18->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_DOORLOCKSTATUS_REARRIGHT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D18->text(), ui->comboBox_D18->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D19_clicked()
{
    int index = ui->comboBox_D19->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_CHILDSAFETYLOCK, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D19->text(), ui->comboBox_D19->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D20_clicked()
{
    int index = ui->comboBox_D20->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_OCCUPANTSSTATUS_DRIVER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D20->text(), ui->comboBox_D20->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D21_clicked()
{
    int index = ui->comboBox_D21->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_OCCUPANTSSTATUS_PASSENGER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D21->text(), ui->comboBox_D21->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D22_clicked()
{
    int index = ui->comboBox_D22->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_OCCUPANTSSTATUS_REARLEFT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D22->text(), ui->comboBox_D22->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D23_clicked()
{
    int index = ui->comboBox_D23->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_OCCUPANTSSTATUS_REARRIGHT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D23->text(), ui->comboBox_D23->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D24_clicked()
{
    int index = ui->comboBox_D24->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_SEATBELT_DRIVER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D24->text(), ui->comboBox_D24->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D25_clicked()
{
    int index = ui->comboBox_D25->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_SEATBELT_PASSENGER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D25->text(), ui->comboBox_D25->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D26_clicked()
{
    int index = ui->comboBox_D26->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_SEATBELT_REARLEFT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D26->text(), ui->comboBox_D26->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D27_clicked()
{
    int index = ui->comboBox_D27->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_SEATBELT_REARRIGHT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D27->text(), ui->comboBox_D27->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D28_clicked()
{
    int index = ui->comboBox_D28->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_WINDOWLOCK_DRIVER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D28->text(), ui->comboBox_D28->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D29_clicked()
{
    int index = ui->comboBox_D29->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_WINDOWLOCK_PASSENGER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D29->text(), ui->comboBox_D29->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D30_clicked()
{
    int index = ui->comboBox_D30->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_WINDOWLOCK_REARLEFT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D30->text(), ui->comboBox_D30->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D31_clicked()
{
    int index = ui->comboBox_D31->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_WINDOWLOCK_REARRIGHT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D31->text(), ui->comboBox_D31->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D32_clicked()
{
    int index = ui->comboBox_D32->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_SENSORSTATUS, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D32->text(), ui->comboBox_D32->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D33_clicked()
{
    unsigned short index = ui->lineEdit_D33->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTCENTER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D33->text(), ui->lineEdit_D33->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D34_clicked()
{
    unsigned short index = ui->lineEdit_D34->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_REARCENTER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D34->text(), ui->lineEdit_D34->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D35_clicked()
{
    unsigned short index = ui->lineEdit_D35->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTLEFT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D35->text(), ui->lineEdit_D35->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D36_clicked()
{
    unsigned short index = ui->lineEdit_D36->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_FRONTRIGHT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D36->text(), ui->lineEdit_D36->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D37_clicked()
{
    unsigned short index = ui->lineEdit_D37->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLELEFT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D37->text(), ui->lineEdit_D37->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D38_clicked()
{
    unsigned short index = ui->lineEdit_D38->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_MIDDLERIGHT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D38->text(), ui->lineEdit_D38->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D39_clicked()
{
    unsigned short index = ui->lineEdit_D39->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_REARLEFT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D39->text(), ui->lineEdit_D39->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D40_clicked()
{
    unsigned short index = ui->lineEdit_D40->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_OBSTACLEDISTANCE_REARRIGHT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D40->text(), ui->lineEdit_D40->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D41_clicked()
{
    int index = ui->comboBox_D41->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::DRIVINGSAFETY_FRONTCOLLISIONDETECTION_STATUS, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D41->text(), ui->comboBox_D41->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D42_clicked()
{
    unsigned long index = ui->lineEdit_D42->text().toULong();
    sprintf(msgData, "%03d%lu", genivi::wavd::socket::DRIVINGSAFETY_FRONTCOLLISIONDETECTION_DISTANCE, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D42->text(), ui->lineEdit_D42->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_D43_clicked()
{
    unsigned long index = ui->lineEdit_D43->text().toULong();
    sprintf(msgData, "%03d%lu", genivi::wavd::socket::DRIVINGSAFETY_FRONTCOLLISIONDETECTION_TIME, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_D43->text(), ui->lineEdit_D43->text(), QDateTime::currentDateTime());
}

// Climate Environment
void MainWindow::on_pushButton_C1_clicked()
{
    short index = ui->lineEdit_C1->text().toShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_INTERIORTEMP, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C1->text(), ui->lineEdit_C1->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C2_clicked()
{
    short index = ui->lineEdit_C2->text().toShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_EXTERIORTEMP, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C2->text(), ui->lineEdit_C2->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C3_clicked()
{
    unsigned long index = ui->lineEdit_C3->text().toULong();
    sprintf(msgData, "%03d%lu", genivi::wavd::socket::CLIMATEENVIRONMENT_EXTERIORBRIGHTNESS, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C3->text(), ui->lineEdit_C3->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C4_clicked()
{
    int index = ui->comboBox_C4->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_RAINSENSOR, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C4->text(), ui->comboBox_C4->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C5_clicked()
{
    int index = ui->comboBox_C5->currentIndex();
    if (index == 6)
	{
        index = 10;
	}
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_WINDSHIELDWIPER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C5->text(), ui->comboBox_C5->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C6_clicked()
{
    int index = ui->comboBox_C6->currentIndex();
    if (index == 6)
	{
        index = 10;
	}
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_REARWIPER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C6->text(), ui->comboBox_C6->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C7_clicked()
{
    int index = ui->comboBox_C7->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_HVACFAN_DIRECTION, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C7->text(), ui->comboBox_C7->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C8_clicked()
{
    unsigned short index = ui->lineEdit_C8->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_HVACFAN_SPEED, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C8->text(), ui->lineEdit_C8->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C9_clicked()
{
    short index = ui->lineEdit_C9->text().toShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_HVACFAN_TARGETTEMP, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C9->text(), ui->lineEdit_C9->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C10_clicked()
{
    int index = ui->comboBox_C10->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_AIRCONDITIONING, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C10->text(), ui->comboBox_C10->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C11_clicked()
{
    int index = ui->comboBox_C11->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_AIRRECIRCULATION, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C11->text(), ui->comboBox_C11->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C12_clicked()
{
    int index = ui->comboBox_C12->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_HEATER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C12->text(), ui->comboBox_C12->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C13_clicked()
{
    int index = ui->comboBox_C13->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_DEFROST_WINDSHIELD, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C13->text(), ui->comboBox_C13->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C14_clicked()
{
    int index = ui->comboBox_C14->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_DEFROST_REARWINDOW, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C14->text(), ui->comboBox_C14->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C15_clicked()
{
    int index = ui->comboBox_C15->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_DEFROST_SIDEMIRRORS, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C15->text(), ui->comboBox_C15->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C16_clicked()
{
    int index = ui->comboBox_C16->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_STEERINGWHEELHEATER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C16->text(), ui->comboBox_C16->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C17_clicked()
{
    int index = ui->comboBox_C17->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_SEATHEATER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C17->text(), ui->comboBox_C17->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C18_clicked()
{
    int index = ui->comboBox_C18->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_SEATCOOLER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C18->text(), ui->comboBox_C18->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C19_clicked()
{
    unsigned short index = ui->lineEdit_C19->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_WINDOW_DRIVER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C19->text(), ui->lineEdit_C19->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C20_clicked()
{
    unsigned short index = ui->lineEdit_C20->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_WINDOW_PASSENGER, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C20->text(), ui->lineEdit_C20->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C21_clicked()
{
    unsigned short index = ui->lineEdit_C21->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_WINDOW_REARLEFT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C21->text(), ui->lineEdit_C21->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C22_clicked()
{
    unsigned short index = ui->lineEdit_C22->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_WINDOW_REARRIGHT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C22->text(), ui->lineEdit_C22->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C23_clicked()
{
    unsigned short index = ui->lineEdit_C23->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_SUNROOF_OPENNESS, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C23->text(), ui->lineEdit_C23->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C24_clicked()
{
    unsigned short index = ui->lineEdit_C24->text().toUShort();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_SUNROOF_TILT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C24->text(), ui->lineEdit_C24->text(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_C25_clicked()
{
    int index = ui->comboBox_C25->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::CLIMATEENVIRONMENT_CONVERTIBLEROOF, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_C25->text(), ui->comboBox_C25->currentText(), QDateTime::currentDateTime());
}

// Vision System
void MainWindow::on_pushButton_VS1_clicked()
{
    int index = ui->comboBox_VS1->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::VISIONSYSTEM_LANEDEPARTUREDETECTIONSTATUS, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_VS1->text(), ui->comboBox_VS1->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_VS2_clicked()
{
    int index = ui->comboBox_VS2->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::VISIONSYSTEM_LANEDEPARTED, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_VS2->text(), ui->comboBox_VS2->currentText(), QDateTime::currentDateTime());
}

// Parking
void MainWindow::on_pushButton_PA1_clicked()
{
    int index = ui->comboBox_PA1->currentIndex() + 1;
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PARKING_SECURITYALERT, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_PA1->text(), ui->comboBox_PA1->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_PA2_clicked()
{
    int index = ui->comboBox_PA2->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PARKING_PARKINGBRAKE, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_PA2->text(), ui->comboBox_PA2->currentText(), QDateTime::currentDateTime());
}
void MainWindow::on_pushButton_PA3_clicked()
{
    int index = ui->comboBox_PA3->currentIndex();
    sprintf(msgData, "%03d%d", genivi::wavd::socket::PARKING_PARKINGLIGHTS, index);
    QByteArray datagram(msgData);
    udpClient->send(datagram);
    addLog(m_itemModel, "Send", ui->label_PA3->text(), ui->comboBox_PA3->currentText(), QDateTime::currentDateTime());
}
