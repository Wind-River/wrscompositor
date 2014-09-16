/*
 * Copyright © 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#ifndef MH_H
#define MH_H

#include <QObject>
#include <QVariant>
#include <QProcess>
#include <QQuickView>
#include <QWidget>

class MenuHandler : public QObject
{
    Q_OBJECT
public:
    explicit MenuHandler(QObject *parent = 0);
    Q_INVOKABLE bool launch(const QString &cmd);

public:
    //static QQuickView *mainQmlView;
    static QWidget *mainQmlView;
    static QWidget *htmlView;
};

#endif
