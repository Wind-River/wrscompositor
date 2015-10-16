#ifndef PROJECTIONMODE_H
#define PROJECTIONMODE_H

#include <QObject>
#include <QVariant>
#include <QProcess>

class ProjectionModePrivate : public QObject
{
    Q_OBJECT
public:
    explicit ProjectionModePrivate(QObject *parent = 0);

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
};

#endif
