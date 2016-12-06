/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "config.h"
#include "util.h"
#include <QGuiApplication>
#include <QScreen>
#include <QSettings>
#include "wrscompositor.h"

#if WRSCOMPOSITOR_WEBENGINE
#include <QtWebEngine>
#else
#include <QtWebKitWidgets>
#endif

#include "Process.h"
#include "systemd-util.h"
//#include "vna_dbusclient.h"
#include "wr_dbusclient.h"
#include "projectionmode.h"

#if WRSCOMPOSITOR_DIGITALCLUSTER
#include "digitalcluster.h"
#endif
#if WRSCOMPOSITOR_REARDISPLAY
#include "reardisplay.h"
#endif

#if WRSCOMPOSITOR_WAYLAND_COMPOSITOR
#include "wrsivimodel.h"
#endif

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <QFileInfo>
#include <QLocalServer>
#include <QLocalSocket>
#include "gbm.h"
#include <pwd.h>
#include <grp.h>
#include <security/pam_appl.h>

#include <qpa/qplatformnativeinterface.h>


#define HEIGHT_FULLSCREEN 0xFFFF
#define HEIGHT_720 720
#define HEIGHT_1080 1080
static int mode = HEIGHT_FULLSCREEN;

static int pam_conv_cb(int msg_count,
        const struct pam_message **messages,
        struct pam_response **responses, void *user_data) {
    (void)msg_count;
    (void)messages;
    (void)responses;
    (void)user_data;
    return PAM_SUCCESS;
}

class VTHandlerServer: public QLocalServer {
    Q_OBJECT
public:
    VTHandlerServer(QGuiApplication *app, QObject *parent=Q_NULLPTR):
        QLocalServer(parent), mClient(0), m_gbm_device(0), mStarted(false),
        mConnected(false) {
        connect(this, SIGNAL(newConnection()), this, SLOT(slotAccept()));

        QPlatformNativeInterface *npi = app->platformNativeInterface();
        m_gbm_device = (struct gbm_device*)npi->nativeResourceForIntegration("nativedisplay");

        mTTY = "auto";

        int i = -1;
        QStringList args = app->arguments();
        QRegExp tty("--tty=(\\S+)");
        i = args.indexOf(tty);
        if(i >= 0) {
            if(tty.exactMatch(args.at(i))) {
                mTTY = tty.capturedTexts()[1];
            }
        }
    }
    bool setupPAM(struct passwd *pw) {
        int err;
        struct pam_conv conv = {pam_conv_cb, NULL};
        err = pam_start("login", pw->pw_name, &conv, &mPH);
        if (err != PAM_SUCCESS) {
            qCritical() << "failed to pam_start";
            return false;
        }

        /*
        err = pam_set_item(mPH, PAM_TTY, "/dev/tty1");
        if (err != PAM_SUCCESS) {
            qCritical() << "failed to set PAM_TTY";
            return false;
        }
        */

        err = pam_open_session(mPH, 0);
        if (err != PAM_SUCCESS) {
            qCritical("failed to open pam session");
            return false;
        }

        char xdgruntimedir[32] = {0, };
        sprintf(xdgruntimedir, "/run/user/%d", pw->pw_uid);
        qInfo() << "reset XDG_RUNTIME_DIR to " << xdgruntimedir;
        ::setenv("XDG_RUNTIME_DIR", xdgruntimedir, 1);

        ::setenv("USER", pw->pw_name, 1);
        ::setenv("LOGNAME", pw->pw_name, 1);
        ::setenv("HOME", pw->pw_dir, 1);
        ::setenv("SHELL", pw->pw_shell, 1);

        char **env = pam_getenvlist(mPH);
        if (env) {
            for (int i = 0; env[i]; ++i) {
                qInfo() << env[i];
                if (putenv(env[i]) < 0)
                    qCritical() << "putenv failed" << env[i];
            }
            free(env);
        }


        int r;
        qDebug() << "setuid to " << pw->pw_name;
        r = ::setgid(pw->pw_gid);
        if(r < 0)
            qWarning() << "Failed to setgid to " << pw->pw_gid;
        r = initgroups(pw->pw_name, pw->pw_gid);
        if(r < 0)
            qWarning() << "Failed to initgroups for " <<
                                pw->pw_name << pw->pw_gid;
        r = ::setuid(pw->pw_uid);
        if(r < 0)
            qWarning() << "Failed to setuid to " << pw->pw_uid;

        //::system("export");
        return true;
    }
    void setGBM(struct gbm_device *gbm) {
        m_gbm_device = gbm;
    }
    bool vtHandlerStarted() {
        return mStarted;
    }
    bool vtHandlerConnected() {
        return mConnected;
    }
    void startVTHandler() {
        if(!isListening())
            return;
        qInfo() << "VT Handler Server is staring at " << fullServerName();
        QString args = QString(" --tty=%1").arg(mTTY);
        mStarted = false;
        if(QFileInfo::exists("../vt-handler/wrscompositor-vt-handler")) {
            // for debug
            mStarted = mProcess.execute("../vt-handler/wrscompositor-vt-handler-dbg"+args);
        } else {
            mStarted = mProcess.execute("wrscompositor-vt-handler"+args); // exec in PATH
        }
        if(mStarted)
            qDebug() << "Run VT handler PID:" << mProcess.getPID();
        else
            qDebug() << "Failed to start wrscompositor-vt-handler";
    };
    void addDisplay(QQuickWindow *disp) {
        mDisplayList << disp;
    };
public slots:
    void slotAccept() {
        qDebug() << "wrscompositor-vt-handler is connected";
        if(mClient) {
            mClient->disconnectFromServer();
            delete mClient;
        }
        mClient = nextPendingConnection();

        struct msghdr msg;
        struct iovec iov[1];
        struct cmsghdr *cmsg = NULL;
        int fd = -1;
        char fdbuf[CMSG_SPACE(sizeof(int))];
        char data[1] = {0, };
        ssize_t len;

        memset(&msg, 0, sizeof(struct msghdr));
        memset(&fdbuf, 0, CMSG_SPACE(sizeof(int)));
        data[0] = ' ';
        iov[0].iov_base = data;
        iov[0].iov_len = sizeof(data);

        if(m_gbm_device)
            fd = gbm_device_get_fd(m_gbm_device);
        if (fd != -1) {
            msg.msg_name = NULL;
            msg.msg_namelen = 0;
            msg.msg_iov = iov;
            msg.msg_iovlen = 1;
            msg.msg_controllen = CMSG_SPACE(sizeof(int));
            msg.msg_control = fdbuf;

            cmsg = CMSG_FIRSTHDR(&msg);
            cmsg->cmsg_level = SOL_SOCKET;
            cmsg->cmsg_type = SCM_RIGHTS;
            cmsg->cmsg_len = CMSG_LEN(sizeof(int));

            *reinterpret_cast<int *>CMSG_DATA(cmsg) = fd;

            do {
                len = sendmsg(mClient->socketDescriptor(), &msg, MSG_NOSIGNAL);
            } while (len < 0 && errno == EINTR);
            qDebug() << "sent drm fd" << fd;
            connect(mClient, SIGNAL(readyRead()), this, SLOT(slotCommand()));
        }
        mConnected = true;
    }
    void slotCommand() {
        char cmd[32] = {0, };
        int r = mClient->read(cmd, 1);
        qInfo() << __func__ << r << cmd;
        if(cmd[0] == 'r') {
            for(int i=0; i<mDisplayList.count(); i++) {
                QQuickWindow *d = mDisplayList.at(i);
                d->update();
                qWarning() << d << "updated by VT switch back";
            }
        }
    }
private:
    QLocalSocket *mClient;
    Process mProcess;
    struct gbm_device *m_gbm_device;
    QString mTTY;
    QGuiApplication *mApp;
    bool mStarted;
    bool mConnected;
    QList<QQuickWindow*> mDisplayList;
    pam_handle_t *mPH;
};

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QStringList args = app.arguments();

    if(args.contains("--720")) {
        mode = HEIGHT_720;
    } else if(args.contains("--1080")) {
        mode = HEIGHT_1080;
    } else if(args.contains("--list-displays")) {
        for(int i=0; i<QGuiApplication::screens().count(); i++) {
            QScreen *screen = QGuiApplication::screens().at(i);
            qDebug() << i << screen->name() << screen->size();
        }
        return 0;
    } else if(app.arguments().contains("--clean-geometry")) {
        QSettings settings(WRSCOMPOSITOR_MANUFACTURER, WRSCOMPOSITOR_PRODUCT_NAME);
        settings.clear();
        settings.sync();
        qInfo() << "Geometry Cache Cleared";
        return 0;
    } else if(app.arguments().contains("--help")) {
        qInfo() << "Wind River WrsCompositor Wayland Compositor";
        qInfo() << "Usage:";
        qInfo() << "  " << app.arguments().at(0) << " [arguments]";
        qInfo() << "";
        qInfo() << "Available Arguments";
        qInfo() << "  --help            Show this help";
        qInfo() << "  --debug           Load QML files from directory";
        qInfo() << "  --720             Show main window as 720px height";
        qInfo() << "  --1080            Show main window as 1080px height";
        qInfo() << "  --tty=path        tty dev path (e.g - /dev/tty1)";
        qInfo() << "  --user=username   username for wrscompositor";
        qInfo() << "  --clean-geometry  Clean saved window geometry";
        qInfo() << "  --list-displays   Show display list";
        qInfo() << "  --display=NUM     Set WAYLAND_DISPLAY environment, default NUM=0";
        qInfo() << "  --dbus-systembus  Create D-Bus services on SystemBus";
        return 0;
    }

    VTHandlerServer s(&app);
    if(getuid() == 0) { // root
#if WRSCOMPOSITOR_VTHANDLER
        s.removeServer(".wrscompositor-vt");
        if(s.listen(".wrscompositor-vt")) {
            s.startVTHandler();
            qInfo() << "Waiting until wrscompositor-vt-handler launched";
            while(!s.vtHandlerStarted() || !s.vtHandlerConnected()) {
                app.processEvents();
            }
            qInfo() << "wrscompositor-vt-handler has been launched";
        }
#endif // WRSCOMPOSITOR_VTHANDLER
        // XXX setuid
        QRegExp user("--user=(\\S+)");
        int i = args.indexOf(user);
        if(i >= 0) {
            if(user.exactMatch(args.at(i))) {
                struct passwd *pw =
                getpwnam(user.capturedTexts()[1].toUtf8().constData());

                if(!pw) {
                    qCritical()<< "No such user"<<user.capturedTexts()[1];
                    exit(1);
                }
                if(!s.setupPAM(pw)) {
                    qCritical()<< "Failed to start PAM session";
                    exit(1);
                }
            }
        }
    }

    qInfo() << "Starting wrscompositor ... with uid " << getuid();

    QSettings settings(WRSCOMPOSITOR_MANUFACTURER, WRSCOMPOSITOR_PRODUCT_NAME);
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
#if 0
    QDesktopWidget d;
    QRect screenGeometry = d.screenGeometry();
#endif
    qmlRegisterType<Process>("com.windriver.wrscompositor", 1, 0, "Process");
    qmlRegisterType<SystemdDbusClient>("com.windriver.wrscompositor", 1, 0, "SystemdDbusClient");
    qmlRegisterType<SystemdUnit>("com.windriver.wrscompositor", 1, 0, "SystemdUnit");
    //qmlRegisterType<VNADBusClient>("com.windriver.automotive", 1, 0, "VNADBusClient");
    qmlRegisterType<WRDBusClient>("com.windriver.automotive", 1, 0, "WRDBusClient");
    qmlRegisterType<ProjectionMode>("com.windriver.automotive", 1, 0, "ProjectionMode");
#if WRSCOMPOSITOR_WAYLAND_COMPOSITOR
    qmlRegisterType<WrsIVIModel::IVIScene>("com.windriver.genivi", 1, 0, "IVIScene");
    qmlRegisterType<WrsIVIModel::IVIScreen>("com.windriver.genivi", 1, 0, "IVIScreen");
    qmlRegisterType<WrsIVIModel::IVILayer>("com.windriver.genivi", 1, 0, "IVILayer");
    qmlRegisterType<WrsIVIModel::IVISurface>("com.windriver.genivi", 1, 0, "IVISurface");
    qRegisterMetaType<WrsIVIModel::IVILayer* >("IVILayer*");
    qRegisterMetaType<WrsIVIModel::IVISurface* >("IVISurface*");
    qRegisterMetaType<WrsIVIModel::IVIScreen* >("IVIScreen*");
    qRegisterMetaType<WrsIVIModel::IVIScene* >("IVIScene*");
#endif

#if WRSCOMPOSITOR_WEBENGINE
    QtWebEngine::initialize();
#endif

    int i = -1;
    QString waylandDisplay="wayland-0";
    QRegExp displayNum("--display=(\\d+)");
    i = args.indexOf(displayNum);
    if(i >= 0) {
        if(displayNum.exactMatch(args.at(i))) {
            waylandDisplay = "wayland-"+displayNum.capturedTexts()[1];
            Process::WAYLAND_DISPlAY = waylandDisplay;
        }
    }
    WrsCompositor compositor(waylandDisplay);
    s.addDisplay(&compositor);


#if WRSCOMPOSITOR_DIGITALCLUSTER
    DigitalCluster dc;
    s.addDisplay(&dc);
#endif
#if WRSCOMPOSITOR_REARDISPLAY
    RearDisplay rd;
    s.addDisplay(&rd);
    compositor.setRearDisplay(&rd);
    rd.setMainDisplay(&compositor);
    rd.setMainOutput(compositor.mainOutput());
#endif

    if(mode != HEIGHT_FULLSCREEN) {
        if(mode == HEIGHT_720) {
            compositor.setGeometry(settings.value("geometry-for-maindisplay", QRect(50, 50, 1280, 720)).toRect());
#if WRSCOMPOSITOR_DIGITALCLUSTER
            dc.setGeometry(settings.value("geometry-for-cluster", QRect(100, 100, 1280, 720)).toRect());
#endif
#if WRSCOMPOSITOR_REARDISPLAY
            rd.setGeometry(settings.value("geometry-for-reardisplay", QRect(150, 150, 1280, 720)).toRect());
#endif
        } else {
            compositor.setGeometry(settings.value("geometry-for-maindisplay", QRect(50, 50, 1920, 1080)).toRect());
#if WRSCOMPOSITOR_DIGITALCLUSTER
            dc.setGeometry(settings.value("geometry-for-cluster", QRect(100, 100, 1920, 1080)).toRect());
#endif
#if WRSCOMPOSITOR_REARDISPLAY
            rd.setGeometry(settings.value("geometry-for-reardisplay", QRect(150, 150, 1920, 1080)).toRect());
#endif
        }
    } else { // full screen
        compositor.setGeometry(screenGeometry);
    }
#if WRSCOMPOSITOR_DIGITALCLUSTER
    dc.show();
#endif
#if WRSCOMPOSITOR_REARDISPLAY
    rd.show();
#endif
    Util util;
    compositor.rootContext()->setContextProperty("util", &util);
    compositor.show();
    //WrsCompositor rearcompositor("cluster.qml", "wayland-1");
    //rearcompositor.show();

    int ret = app.exec();

    if(mode != HEIGHT_FULLSCREEN) { // save last geometry
        settings.setValue("geometry-for-maindisplay", compositor.geometry());
#if WRSCOMPOSITOR_DIGITALCLUSTER
        settings.setValue("geometry-for-cluster", dc.geometry());
#endif
#if WRSCOMPOSITOR_REARDISPLAY
        settings.setValue("geometry-for-reardisplay", rd.geometry());
#endif
    }
    return ret;
}

#include "main.moc"
