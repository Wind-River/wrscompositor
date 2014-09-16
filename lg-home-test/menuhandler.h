#ifndef HH_H
#define HH_H

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
    static QQuickView *mainQmlView;
    static QWidget *htmlView;
};

#endif
