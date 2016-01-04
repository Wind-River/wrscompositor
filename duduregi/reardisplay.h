#ifndef REAR_DISPLAY_H
#define REAR_DISPLAY_H
#include "duduregiconfig.h"

#include <QSurfaceFormat>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>
#include <QSettings>

#include <QTimer>

class RearDisplay : public QQuickView
{
    Q_OBJECT

public:
    RearDisplay(QWindow *parent = 0);
    virtual ~RearDisplay();
};
#endif
