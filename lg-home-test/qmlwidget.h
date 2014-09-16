/*
 * Copyright © 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#ifndef QMLWIDGET_H
#define QMLWIDGET_H

#include <QtWidgets>
#include <QQuickView>

class QMLWidget : public QQuickView
{
    Q_OBJECT

public:
    QMLWidget(QWidget *parent);
};

#endif
