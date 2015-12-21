#ifndef DUDUREGI_CLUSTER
#define DUDUREGI_CLUSTER
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
