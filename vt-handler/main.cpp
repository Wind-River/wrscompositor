/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#include <QCoreApplication>
#include <QDebug>
#include <QLocalSocket>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <QFileInfo>
#include <QLocalServer>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <linux/vt.h>
#include <linux/major.h>
#include <linux/kd.h>
#ifndef KDSKBMUTE
#define KDSKBMUTE 0x4B51
#endif

#include "logind-client.h"

#define DRM_MAJOR 226

class VTHandlerClient: public QLocalSocket {
    Q_OBJECT
public:
    VTHandlerClient(QObject *parent=Q_NULLPTR) : QLocalSocket(parent) {
        connect(this, SIGNAL(connected()), this, SLOT(slotConnected()));
        connect(this, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
        mParentPID = getppid();
        qInfo() << "Parent Compositor: " << mParentPID;
    }
    void setDRMFd(int fd) {
        mLogind.setDRMFd(fd);
        mLogind.systemdConnect();
        connect(&mLogind, SIGNAL(paused()), this, SLOT(slotPaused()));
        connect(&mLogind, SIGNAL(resumed()), this, SLOT(slotResumed()));
    };
private:
    LogindClient mLogind;
    int mParentPID;
public slots:
    void slotConnected() {
        qDebug() << "connected";
    }
    void slotDisconnected() {
        qDebug() << "duduregi-compositor socket disclosed";
        mLogind.restoreTTY();
        qApp->quit();
    }
    void slotPaused() {
        qWarning() << __func__;
        qWarning() << write("p", 1);
    }
    void slotResumed() {
        qWarning() << __func__;
        qWarning() << write("r", 1);
    }
public:
    int readFd() {
        qDebug() << "ready";
        struct msghdr msg;
        struct iovec iov[1];
        struct cmsghdr *cmsg = NULL;
        char fdbuf[CMSG_SPACE(sizeof(int))];
        char data[1];
        ssize_t len;

        memset(&msg, 0, sizeof(struct msghdr));
        memset(fdbuf, 0, CMSG_SPACE(sizeof(int)));

        iov[0].iov_base = data;
        iov[0].iov_len = sizeof(data);

        msg.msg_name = NULL;
        msg.msg_namelen = 0;
        msg.msg_control = fdbuf;
        msg.msg_controllen = CMSG_SPACE(sizeof(int));
        msg.msg_iov = iov;
        msg.msg_iovlen = 1;

        do {
            len = recvmsg(socketDescriptor(), &msg, 0);
        } while (len < 0 && (errno == EINTR || errno == EAGAIN));
        //qDebug() << "recv len " << len << errno << EINTR << EAGAIN;

        for(cmsg = CMSG_FIRSTHDR(&msg);
                cmsg != NULL;
                cmsg = CMSG_NXTHDR(&msg, cmsg))
        {
            if(cmsg->cmsg_level == SOL_SOCKET &&
                    cmsg->cmsg_type == SCM_RIGHTS) {
                return *reinterpret_cast<int*>CMSG_DATA(cmsg);
            }
        }
        return -1;
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QStringList args = app.arguments();
    if(args.contains("--test")) {
        LogindClient logind;
        logind.systemdConnect();
        return app.exec();
    } else {
        QStringList args = qApp->arguments();
        if(getuid()!=0) {
            qCritical() << "root only operation";
            return 1;
        }
        QRegExp rtty("--reset=(\\S+)");
        QString ttyPath = "auto";
        int i = args.indexOf(rtty);
        if(i >= 0) {
            if(rtty.exactMatch(args.at(i))) {
                ttyPath = rtty.capturedTexts()[1];
            }
            if(ttyPath != "auto") {
                int tty=open(ttyPath.toUtf8().constData(), O_RDWR | O_NOCTTY);
                if (ioctl(tty, KDSKBMUTE, 0)&&ioctl(tty, KDSKBMODE, K_UNICODE))
                    qWarning() << "failed to restore kb mode";
                if (ioctl(tty, KDSETMODE, KD_TEXT))
                    qWarning() << "failed to set KD_TEXT";
                struct vt_mode mode;
                mode.mode = VT_AUTO;
                if (ioctl(tty, VT_SETMODE, &mode) < 0)
                    qWarning() << "failed to set VTMODE as AUTO";
                return 0;
            }
            return 1;
        }
    }

    VTHandlerClient c;
    c.connectToServer(".duduregi-vt");
    if(c.waitForConnected()) {
        int fd = c.readFd();
        qInfo() << "read drm_fd " << fd;
        struct stat s;
        fstat(fd, &s);
        qDebug() << "MAJOR_DRM" << major(s.st_rdev);
        if(major(s.st_rdev) != DRM_MAJOR) {
            qCritical() << "Error, Received fd is not DRM device";
            exit(1);
        }
        c.setDRMFd(fd);
    }

    return app.exec();
}
#include "main.moc"
