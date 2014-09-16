#include "menuhandler.h"
#include <QVariant>
#include <QMetaType>
#include <QCoreApplication>
#include <QDebug>

QQuickView *MenuHandler::mainQmlView = NULL;
QWidget *MenuHandler::htmlView = NULL;

MenuHandler::MenuHandler(QObject *parent) :
    QObject(parent)
{
}
bool MenuHandler::launch(const QString &cmd)
{
    if(cmd == "ipod") {
        MenuHandler::htmlView->show();
        MenuHandler::mainQmlView->hide();
    }
    return true;
}
