#ifndef PROJECTIONMODE_H
#define PROJECTIONMODE_H

#include <QObject>
#include <QVariant>
#include <QProcess>
#include "projectionmode_adaptor.h"

class ProjectionModePrivate : public QObject
{
    Q_OBJECT
public:
    explicit ProjectionModePrivate(QObject *parent = 0);
    Q_INVOKABLE void sendMousePressed(int x, int y);
    Q_INVOKABLE void sendKeyPressed(int keycode);
    Q_INVOKABLE void sendMouseReleased(int x, int y);
    Q_INVOKABLE void sendKeyReleased(int keycode);

Q_SIGNALS:
    void touchEvent(int x, int y, int button);
    void keyEvent(int keycode, int pressed);
    void focusEvent(bool acquired);

};
class ProjectionMode : public QObject
{
    Q_OBJECT
public:
    explicit ProjectionMode(QObject *parent = 0);
private:
    ProjectionModePrivate *mPM;
};

#endif
