#ifndef WRSLOGGING_H
#define WRSLOGGING_H

#define INFO()   qInfo()  << "[INFO]  (" << __func__ << ":" << __LINE__ << ")"
#define TRACE()  qDebug() << "[TRACE] (" << __func__ << ":" << __LINE__ << ")"
#define DEBUG()  qDebug() << "[DEBUG] (" << __func__ << ":" << __LINE__ << ")"

#endif // WRSLOGGING_H
