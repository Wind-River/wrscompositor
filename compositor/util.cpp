#include "util.h"
#include <QFile>
#include <QTextStream>
#include <QIODevice>


Util::Util(QObject *parent) :
    QObject(parent)
{
}


Q_INVOKABLE QString Util::getCmdForPid(int pid) const {
   char buffer[1024] = {0};
   sprintf(buffer, "/proc/%d/comm", pid);
   QFile        file(buffer);
   QTextStream  in(&file);

   if(!file.open(QIODevice::ReadOnly)) {
       return "";
   } else {
       return in.readLine();
   }
}
