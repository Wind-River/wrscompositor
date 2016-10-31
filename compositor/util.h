#ifndef UTIL_H
#define UTIL_H

#include <QObject>

class Util : public QObject
{
    Q_OBJECT
public:
    explicit Util(QObject *parent = 0);
    Q_INVOKABLE QString getCmdForPid(int pid) const;

};

#endif // UTIL_H
