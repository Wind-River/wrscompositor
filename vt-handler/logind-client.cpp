/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#include <QDebug>
#include <QtDBus/QtDBus>
#include <QDBusVariant>

#include "logind-client.h"
//#include <systemd/sd-login.h>

#include <sys/types.h>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/vt.h>
#include <linux/major.h>
#include <linux/kd.h>
#include <linux/input.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/signalfd.h>
#ifndef KDSKBMUTE
#define KDSKBMUTE 0x4B51
#endif
#include <xf86drm.h>

#define LOGIND_SERVICE "org.freedesktop.login1"
#define LOGIND_SESSION_IF "org.freedesktop.login1.Session"
#define LOGIND_MANAGER_IF "org.freedesktop.login1.Manager"
#define XDG_PROPERTIES_IF "org.freedesktop.DBus.Properties"


static LogindClient *thiz;

LogindClient::LogindClient() : mDRMFd(-1) {
}

void LogindClient::systemdConnect() {
    if (!QDBusConnection::systemBus().isConnected()) {
        qWarning() << "Could not connect to the D-Bus session.\n";
        return;
    }


    /*
    int r = -1;
    //char *seat = NULL;
    char *session = NULL;
    r = sd_pid_get_session(getpid(), &session);
    if(r < 0) {
        qWarning() << "Could not get session from systemd";
        return;
    }
    qWarning() << "Current login session:" << session;
    free(session);
    */

    qWarning() << "GetSessionByPID" << getppid();
    QDBusMessage ret;
    ret = QDBusConnection::systemBus().call(
            QDBusMessage::createMethodCall (LOGIND_SERVICE,
                QString("/org/freedesktop/login1"),
                LOGIND_MANAGER_IF,
                QStringLiteral("GetSessionByPID")) << (uint)getppid());
    qWarning() << ret;
    mSession = ret.arguments()[0].value<QDBusObjectPath>().path();
    qWarning() << "session path" << mSession;

    ret = QDBusConnection::systemBus().call(
            QDBusMessage::createMethodCall (LOGIND_SERVICE,
                mSession,
                LOGIND_SESSION_IF, QStringLiteral("TakeControl")) << false);
    qWarning() << ret;


    /*
    r = sd_session_get_seat(session, &seat);
    if(r < 0) {
        qWarning() << "Could not get seat from systemd";
        return;
    }
    qWarning() << "Current login seat:" << seat;
    mSeat = seat;
    free(seat);


#ifdef sd_session_get_vt
    unsigned int vtnr;
    if(sd_session_get_vt(session, &vtnr) >= 0) {
        // check tty and vt#
    }
    qWarning() << "vtnr: " << vtnr;
#endif
    */

    setupTTY();


#if 0
    if(mDRMFd > 0) {
        struct stat s;
        fstat(mDRMFd, &s);
        qWarning() << "TakeDevice";
        ret = QDBusConnection::systemBus().call(
                QDBusMessage::createMethodCall (LOGIND_SERVICE,
                    mSession, LOGIND_SESSION_IF, QStringLiteral("TakeDevice"))
                 << major(s.st_rdev)
                 << minor(s.st_rdev));
        qWarning() << ret;
        //mDRMFd = ret.arguments()[0].value<QDBusUnixFileDescriptor>().fileDescriptor();
        qWarning() << "new drm fd" << mDRMFd;
    }
#endif

    qWarning() << "connect pausedevice signal" <<
        QDBusConnection::systemBus().connect(LOGIND_SERVICE,
            mSession, LOGIND_SESSION_IF,
            QStringLiteral("PauseDevice"), this,
            SLOT(slotPauseDevice(uint, uint, QString)));

}

void LogindClient::slotPauseDevice(uint major, uint minor, const QString &type) {
    qWarning() << __func__ << major << minor << type;
    QDBusMessage ret;
    ret = QDBusConnection::systemBus().call(
            QDBusMessage::createMethodCall (LOGIND_SERVICE,
                QString("/org/freedesktop/login1"),
                LOGIND_MANAGER_IF,
                QStringLiteral("PauseDeviceComplete")) << major << minor);
    qWarning() << ret;
}


static int kb_mode;
static int tty;
int LogindClient::setupTTY() {
    struct vt_mode mode;
    char *t = ttyname(STDIN_FILENO);
    struct stat buf;

    QString ttyPath = "auto";
    int i = -1;
    QStringList args = qApp->arguments();
    QRegExp rtty("--tty=(\\S+)");
    i = args.indexOf(rtty);
    if(i >= 0) {
        if(rtty.exactMatch(args.at(i))) {
            ttyPath = rtty.capturedTexts()[1];
        }
    }
    if(ttyPath == "auto") {
        QDBusMessage ret;
        ret = QDBusConnection::systemBus().call(
                QDBusMessage::createMethodCall (LOGIND_SERVICE,
                    mSession, XDG_PROPERTIES_IF,
                    QStringLiteral("Get")) << LOGIND_SESSION_IF << "TTY");
        ttyPath=ret.arguments()[0].value<QDBusVariant>().variant().toString();
        if(!ttyPath.startsWith("/"))
            ttyPath = "/dev/"+ttyPath;
    }

    if(t && strcmp(t, ttyPath.toUtf8().constData()) == 0)
        tty = STDIN_FILENO;
    else
        tty = open(ttyPath.toUtf8().constData(), O_RDWR | O_NOCTTY);

    qWarning() << "ttyPath: "<< ttyPath << "ttyfd" << tty;
    /*
    } else {
        int tty0 = open("/dev/tty0", O_WRONLY | O_CLOEXEC);
        int ttynr = 1;
        char filename[16];
        if (tty0 < 0)
            qCritical() << errno << "could not open tty0";
        if (ioctl(tty0, VT_OPENQRY, &ttynr) < 0 || ttynr == -1)
            qCritical() << errno << "failed to find non-opened console";
        snprintf(filename, sizeof filename, "/dev/tty%d", ttynr);
        tty = open(filename, O_RDWR | O_NOCTTY);
        close(tty0);
    }
    */
    if(tty < 0) {
        qCritical() << "failed to open tty" << ttyPath;
        return 1;
    }
    qWarning() << "fstat" << fstat(tty, &buf);
    qWarning() << "major" << major(buf.st_rdev) << TTY_MAJOR;
    qWarning() << "minor" << minor(buf.st_rdev);
    if (fstat(tty, &buf) == -1 ||
            major(buf.st_rdev) != TTY_MAJOR || minor(buf.st_rdev) == 0) {
        qWarning() << ttyPath << "is not vt";
        return 1;
    }

    if (ioctl(tty, KDGKBMODE, &kb_mode))
        qWarning() << "failed to get kb mode";

    if (ioctl(tty, KDSKBMUTE, 1) && ioctl(tty, KDSKBMODE, K_OFF))
        qWarning() << "failed to set K_OFF";

    if (ioctl(tty, KDSETMODE, KD_GRAPHICS))
        qWarning() << "failed to set KD_GRAPHICS mode";

    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, mSigFD))
        qFatal("Couldn't create socketpair for signal handler");
    mSNSig = new QSocketNotifier(mSigFD[1], QSocketNotifier::Read, this);
    connect(mSNSig, SIGNAL(activated(int)), this, SLOT(slotSigNotifier(int)));
    thiz = this;

    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = signalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags |= SA_RESTART;
    sigaction(SIGRTMIN, &sa, 0);
    for(int i=0; i<9; i++)
        sigaction(SIGRTMIN+i, &sa, 0);
    sigaction(SIGINT, &sa, 0);
    sigaction(SIGUSR1, &sa, 0);
    sigaction(SIGUSR2, &sa, 0);
    qWarning() << "sigmax" << SIGRTMAX;

    mode.mode = VT_PROCESS;
    mode.relsig = SIGUSR1;
    mode.acqsig = SIGUSR2;
    //mode.relsig = SIGUSR1;
    //mode.acqsig = SIGUSR2;
    if (ioctl(tty, VT_SETMODE, &mode) < 0)
        qWarning() << "failed to set VT_STMODE";

    return 0;
}

void LogindClient::signalHandler(int sigNo) {
    int r = ::write(thiz->mSigFD[0], &sigNo, sizeof(sigNo));
    (void)r;
}

void LogindClient::slotSigNotifier(int) {
    int sigNo;
    int r = ::read(mSigFD[1], &sigNo, sizeof(sigNo));
    qWarning() << "read sig" << sigNo << r << "drm_fd" << mDRMFd;
    if (sigNo == SIGUSR1) {
        qWarning() << "drop";
        r = drmDropMaster(mDRMFd);
        qWarning() << "drmDropMaster(" << mDRMFd << ")" << r;
        r = ioctl(tty, VT_RELDISP, 1);
        qWarning() << "ioctl(tty, VT_RELDISP, 1) =>" << r;
        emit paused();
    } else if (sigNo == SIGUSR2) {
        qWarning() << "set";
        r = ioctl(tty, VT_RELDISP, VT_ACKACQ);
        qWarning() << "ioctl(tty, VT_RELDISP, VT_ACKACQ) =>" << r;
        r = drmSetMaster(mDRMFd);
        qWarning() << "drmSetMaster(" << mDRMFd << ")" << r;
        emit resumed();
    } else if (sigNo == SIGINT) {
        qWarning() << "restore";
        restoreTTY();
    } else {
        for(int i=0; i<9; i++) {
            if(sigNo == SIGRTMIN+i) {
                ioctl(tty, VT_ACTIVATE, i+1);
                break;
            }
        }
    }
}


void LogindClient::restoreTTY() {
    if (ioctl(tty, KDSKBMUTE, 0) && ioctl(tty, KDSKBMODE, K_UNICODE))
    //if (ioctl(tty, KDSKBMUTE, 0) && ioctl(tty, KDSKBMODE, kb_mode))
        qWarning() << "failed to restore kb mode";
    if (ioctl(tty, KDSETMODE, KD_TEXT))
        qWarning() << "failed to set KD_TEXT";

    struct vt_mode mode;
    mode.mode = VT_AUTO;
    if (ioctl(tty, VT_SETMODE, &mode) < 0)
        qWarning() << "failed to set VTMODE as AUTO";
}
