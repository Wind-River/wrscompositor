/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#ifndef DUDUREGI_CLUSTER_H
#define DUDUREGI_CLUSTER_H
#include "duduregiconfig.h"

#include <QSurfaceFormat>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>
#include <QSettings>

#include <QTimer>

class DigitalCluster : public QQuickView
{
    Q_OBJECT

public:
    DigitalCluster(QWindow *parent = 0);
    virtual ~DigitalCluster();
};
#endif
