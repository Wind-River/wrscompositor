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
 * @file mainwindow.h
 * For further information, see http://www.genivi.org/
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "udpclient.h"
#include "udpserver.h"

#define BUF_SIZE 1024
#define COLUMN_SIZE_LOG 4
#define CONFIG_FILE "network.cfg"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void networkSettings();
    void logInit();
    void addLog(QAbstractItemModel *itemModel, const QString &status, const QString &msg, const QString &value, const QDateTime &dateTime);
    ~MainWindow();

private slots:
    void processPendingDatagrams();

    void on_pushButton_A1_clicked();
    void on_pushButton_A2_clicked();
    void on_pushButton_V1_clicked();
    void on_pushButton_V2_clicked();
    void on_pushButton_V3_clicked();
    void on_pushButton_V4_clicked();
    void on_pushButton_V5_clicked();
    void on_pushButton_V6_clicked();
    void on_pushButton_V7_clicked();
    void on_pushButton_V8_clicked();

    void on_pushButton_M1_clicked();
    void on_pushButton_M2_clicked();
    void on_pushButton_M3_clicked();
    void on_pushButton_M4_clicked();
    void on_pushButton_M5_clicked();
    void on_pushButton_M6_clicked();
    void on_pushButton_M7_clicked();
    void on_pushButton_M8_clicked();
    void on_pushButton_M9_clicked();
    void on_pushButton_M10_clicked();
    void on_pushButton_M11_clicked();
    void on_pushButton_M12_clicked();
    void on_pushButton_M13_clicked();
    void on_pushButton_M14_clicked();
    void on_pushButton_M15_clicked();
    void on_pushButton_M16_clicked();

    void on_pushButton_R1_clicked();
    void on_pushButton_R2_clicked();
    void on_pushButton_R3_clicked();
    void on_pushButton_R4_clicked();
    void on_pushButton_R5_clicked();
    void on_pushButton_R6_clicked();
    void on_pushButton_R7_clicked();
    void on_pushButton_R8_clicked();
    void on_pushButton_R9_clicked();
    void on_pushButton_R10_clicked();
    void on_pushButton_R11_clicked();
    void on_pushButton_R12_clicked();
    void on_pushButton_R13_clicked();
    void on_pushButton_R14_clicked();
    void on_pushButton_R15_clicked();
    void on_pushButton_R16_clicked();
    void on_pushButton_R17_clicked();
    void on_pushButton_R18_clicked();
    void on_pushButton_R19_clicked();
    void on_pushButton_R20_clicked();
    void on_pushButton_R21_clicked();
    void on_pushButton_R22_clicked();
    void on_pushButton_R23_clicked();
    void on_pushButton_R24_clicked();
    void on_pushButton_R25_clicked();
    void on_pushButton_R26_clicked();
    void on_pushButton_R27_clicked();
    void on_pushButton_R28_clicked();
    void on_pushButton_R29_clicked();
    void on_pushButton_R30_clicked();
    void on_pushButton_R31_clicked();
    void on_pushButton_R32_clicked();
    void on_pushButton_R33_clicked();
    void on_pushButton_R34_clicked();
    void on_pushButton_R35_clicked();
    void on_pushButton_R36_clicked();
    void on_pushButton_R37_clicked();

    void on_pushButton_P1_clicked();
    void on_pushButton_P2_clicked();
    void on_pushButton_P3_clicked();
    void on_pushButton_P4_clicked();
    void on_pushButton_P5_clicked();
    void on_pushButton_P6_clicked();
    void on_pushButton_P7_clicked();
    void on_pushButton_P8_clicked();
    void on_pushButton_P9_clicked();
    void on_pushButton_P10_clicked();
    void on_pushButton_P11_clicked();
    void on_pushButton_P12_clicked();
    void on_pushButton_P13_clicked();
    void on_pushButton_P14_clicked();
    void on_pushButton_P15_clicked();
    void on_pushButton_P16_clicked();
    void on_pushButton_P17_clicked();
    void on_pushButton_P18_clicked();
    void on_pushButton_P19_clicked();
    void on_pushButton_P20_clicked();
    void on_pushButton_P21_clicked();
    void on_pushButton_P22_clicked();
    void on_pushButton_P23_clicked();
    void on_pushButton_P24_clicked();
    void on_pushButton_P25_clicked();
    void on_pushButton_P26_clicked();
    void on_pushButton_P27_clicked();

    void on_pushButton_D1_clicked();
    void on_pushButton_D2_clicked();
    void on_pushButton_D3_clicked();
    void on_pushButton_D4_clicked();
    void on_pushButton_D5_clicked();
    void on_pushButton_D6_clicked();
    void on_pushButton_D7_clicked();
    void on_pushButton_D8_clicked();
    void on_pushButton_D9_clicked();
    void on_pushButton_D10_clicked();
    void on_pushButton_D11_clicked();
    void on_pushButton_D12_clicked();
    void on_pushButton_D13_clicked();
    void on_pushButton_D14_clicked();
    void on_pushButton_D15_clicked();
    void on_pushButton_D16_clicked();
    void on_pushButton_D17_clicked();
    void on_pushButton_D18_clicked();
    void on_pushButton_D19_clicked();
    void on_pushButton_D20_clicked();
    void on_pushButton_D21_clicked();
    void on_pushButton_D22_clicked();
    void on_pushButton_D23_clicked();
    void on_pushButton_D24_clicked();
    void on_pushButton_D25_clicked();
    void on_pushButton_D26_clicked();
    void on_pushButton_D27_clicked();
    void on_pushButton_D28_clicked();
    void on_pushButton_D29_clicked();
    void on_pushButton_D30_clicked();
    void on_pushButton_D31_clicked();
    void on_pushButton_D32_clicked();
    void on_pushButton_D33_clicked();
    void on_pushButton_D34_clicked();
    void on_pushButton_D35_clicked();
    void on_pushButton_D36_clicked();
    void on_pushButton_D37_clicked();
    void on_pushButton_D38_clicked();
    void on_pushButton_D39_clicked();
    void on_pushButton_D40_clicked();
    void on_pushButton_D41_clicked();
    void on_pushButton_D42_clicked();
    void on_pushButton_D43_clicked();

    void on_pushButton_C1_clicked();
    void on_pushButton_C2_clicked();
    void on_pushButton_C3_clicked();
    void on_pushButton_C4_clicked();
    void on_pushButton_C5_clicked();
    void on_pushButton_C6_clicked();
    void on_pushButton_C7_clicked();
    void on_pushButton_C8_clicked();
    void on_pushButton_C9_clicked();
    void on_pushButton_C10_clicked();
    void on_pushButton_C11_clicked();
    void on_pushButton_C12_clicked();
    void on_pushButton_C13_clicked();
    void on_pushButton_C14_clicked();
    void on_pushButton_C15_clicked();
    void on_pushButton_C16_clicked();
    void on_pushButton_C17_clicked();
    void on_pushButton_C18_clicked();
    void on_pushButton_C19_clicked();
    void on_pushButton_C20_clicked();
    void on_pushButton_C21_clicked();
    void on_pushButton_C22_clicked();
    void on_pushButton_C23_clicked();
    void on_pushButton_C24_clicked();
    void on_pushButton_C25_clicked();

    void on_pushButton_VS1_clicked();
    void on_pushButton_VS2_clicked();

    void on_pushButton_PA1_clicked();
    void on_pushButton_PA2_clicked();
    void on_pushButton_PA3_clicked();

private:
    Ui::MainWindow *ui;
    UdpClient *udpClient;
    UdpServer *udpServer;
    QString m_ipAddress;
    int m_portNumberSend;
    int m_portNumberRcv;
    char *msgData;
    int m_msgID;
    int m_index;

    QSortFilterProxyModel *m_proxyModel;
    QStandardItemModel *m_itemModel;

    QString logHeaderText[COLUMN_SIZE_LOG];
};

#endif // MAINWINDOW_H
