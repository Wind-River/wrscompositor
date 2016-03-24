/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#include "duduregiconfig.h"

#include <QGuiApplication>
#include <QScreen>
#include <QSettings>
#include "duduregicompositor.h"

#if DUDUREGI_WEBENGINE
#include <QtWebEngine>
#else
#include <QtWebKitWidgets>
#endif

#include "Process.h"
#include "vna_dbusclient.h"
#include "wr_dbusclient.h"
#include "projectionmode.h"

#if DUDUREGI_DIGITALCLUSTER
#include "digitalcluster.h"
#endif
#if DUDUREGI_REARDISPLAY
#include "reardisplay.h"
#endif

#if DUDUREGI_WAYLAND_COMPOSITOR
#include "GeniviWaylandIVIExtension.h"
#endif

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <QFileInfo>
#include <QLocalServer>
#include <QLocalSocket>
#include "gbm.h"
#include <pwd.h>
#include <grp.h>

#include <qpa/qplatformnativeinterface.h>


#define HEIGHT_FULLSCREEN 0xFFFF
#define HEIGHT_720 720
#define HEIGHT_1080 1080
static int mode = HEIGHT_FULLSCREEN;

int start_duduregi(QGuiApplication *app = NULL) {
    qDebug() << "Starting duduregi ... with uid " << getuid();

    QSettings settings(DUDUREGI_MANUFACTURER, DUDUREGI_PRODUCT_NAME);
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
#if 0
    QDesktopWidget d;
    QRect screenGeometry = d.screenGeometry();
#endif

    qmlRegisterType<Process>("com.windriver.duduregi", 1, 0, "Process");
    qmlRegisterType<VNADBusClient>("com.windriver.automotive", 1, 0, "VNADBusClient");
    qmlRegisterType<WRDBusClient>("com.windriver.automotive", 1, 0, "WRDBusClient");
    qmlRegisterType<ProjectionMode>("com.windriver.automotive", 1, 0, "ProjectionMode");
#if DUDUREGI_WAYLAND_COMPOSITOR
    qmlRegisterType<GeniviWaylandIVIExtension::IVIScene>("com.windriver.genivi", 1, 0, "IVIScene");
    qmlRegisterType<GeniviWaylandIVIExtension::IVIScreen>("com.windriver.genivi", 1, 0, "IVIScreen");
    qmlRegisterType<GeniviWaylandIVIExtension::IVILayer>("com.windriver.genivi", 1, 0, "IVILayer");
    qmlRegisterType<GeniviWaylandIVIExtension::IVISurface>("com.windriver.genivi", 1, 0, "IVISurface");
    qRegisterMetaType<GeniviWaylandIVIExtension::IVILayer* >("IVILayer*");
    qRegisterMetaType<GeniviWaylandIVIExtension::IVISurface* >("IVISurface*");
    qRegisterMetaType<GeniviWaylandIVIExtension::IVIScreen* >("IVIScreen*");
    qRegisterMetaType<GeniviWaylandIVIExtension::IVIScene* >("IVIScene*");
#endif

#if DUDUREGI_WEBENGINE
    QtWebEngine::initialize();
#endif

    DuduregiCompositor compositor;

#if DUDUREGI_DIGITALCLUSTER
    DigitalCluster dc;
#endif
#if DUDUREGI_REARDISPLAY
    RearDisplay rd;
    compositor.setRearDisplay(&rd);
    rd.setMainDisplay(&compositor);
    rd.setMainOutput(compositor.mainOutput());
#endif

    if(mode != HEIGHT_FULLSCREEN) {
        if(mode == HEIGHT_720) {
            compositor.setGeometry(settings.value("geometry-for-maindisplay", QRect(50, 50, 1280, 720)).toRect());
#if DUDUREGI_DIGITALCLUSTER
            dc.setGeometry(settings.value("geometry-for-cluster", QRect(100, 100, 1280, 720)).toRect());
#endif
#if DUDUREGI_REARDISPLAY
            rd.setGeometry(settings.value("geometry-for-reardisplay", QRect(150, 150, 1280, 720)).toRect());
#endif
        } else {
            compositor.setGeometry(settings.value("geometry-for-maindisplay", QRect(50, 50, 1920, 1080)).toRect());
#if DUDUREGI_DIGITALCLUSTER
            dc.setGeometry(settings.value("geometry-for-cluster", QRect(100, 100, 1920, 1080)).toRect());
#endif
#if DUDUREGI_REARDISPLAY
            rd.setGeometry(settings.value("geometry-for-reardisplay", QRect(150, 150, 1920, 1080)).toRect());
#endif
        }
    } else { // full screen
        compositor.setGeometry(screenGeometry);
    }
#if DUDUREGI_DIGITALCLUSTER
    dc.show();
#endif
#if DUDUREGI_REARDISPLAY
    rd.show();
#endif
    compositor.show();
    //DuduregiCompositor rearcompositor("cluster.qml", "wayland-1");
    //rearcompositor.show();

    if(app == NULL)
        return 0;

    int ret = app->exec();

    if(mode != HEIGHT_FULLSCREEN) { // save last geometry
        settings.setValue("geometry-for-maindisplay", compositor.geometry());
#if DUDUREGI_DIGITALCLUSTER
        settings.setValue("geometry-for-cluster", dc.geometry());
#endif
#if DUDUREGI_REARDISPLAY
        settings.setValue("geometry-for-reardisplay", rd.geometry());
#endif
    }

    return ret;
}

class VTHandlerServer: public QLocalServer {
    Q_OBJECT
public:
    VTHandlerServer(QGuiApplication *app, QObject *parent=Q_NULLPTR):
        QLocalServer(parent), mClient(0), m_gbm_device(0), mStarted(false) {
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
    void setGBM(struct gbm_device *gbm) {
        m_gbm_device = gbm;
    }
    bool vtHandlerStarted() {
        return mStarted;
    }
    void startVTHandler() {
        if(!isListening())
            return;
        qDebug() << "VT Handler Server is staring at " << fullServerName();
        QString args = QString(" --tty=%1").arg(mTTY);
        mStarted = false;
        if(QFileInfo::exists("../vt-handler/duduregi-vt-handler")) {
            // for debug
            mStarted = mProcess.execute("../vt-handler/duduregi-vt-handler"+args);
        } else {
            mStarted = mProcess.execute("duduregi-vt-handler"+args); // exec in PATH
        }
        if(mStarted)
            qDebug() << "Run VT handler PID:" << mProcess.getPID();
        else
            qDebug() << "Failed to start duduregi-vt-handler";
    };
public slots:
    void slotAccept() {
        qDebug() << "duduregi-vt-handler is connected";
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

            *((int *) CMSG_DATA(cmsg)) = fd;

            do {
                len = sendmsg(mClient->socketDescriptor(), &msg, MSG_NOSIGNAL);
            } while (len < 0 && errno == EINTR);
            qDebug() << "sent drm fd" << fd;
        }
    }
private:
    QLocalSocket *mClient;
    Process mProcess;
    struct gbm_device *m_gbm_device;
    QString mTTY;
    QGuiApplication *mApp;
    bool mStarted;
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
        QSettings settings(DUDUREGI_MANUFACTURER, DUDUREGI_PRODUCT_NAME);
        settings.clear();
        settings.sync();
        qCritical() << "Geometry Cache Cleared";
        return 0;
    } else if(app.arguments().contains("--help")) {
        qCritical() << "Wind River Duduregi Wayland Compositor";
        qCritical() << "Usage:";
        qCritical() << "  " << app.arguments().at(0) << " [arguments]";
        qCritical() << "";
        qCritical() << "Available Arguments";
        qCritical() << "  --help            Show this help";
        qCritical() << "  --debug           Load QML files from directory";
        qCritical() << "  --720             Show main window as 720px height";
        qCritical() << "  --1080            Show main window as 1080px height";
        qCritical() << "  --tty=path        tty dev path (e.g - /dev/tty1)";
        qCritical() << "  --user=username   username for duduregi-compositor";
        qCritical() << "  --clean-geometry  Clean saved window geometry";
        qCritical() << "  --list-displays   Show display list";
        return 0;
    }

    VTHandlerServer s(&app);
    if(getuid() == 0) { // root
        s.removeServer(".duduregi-vt");
        if(s.listen(".duduregi-vt")) {
            s.startVTHandler();
            qDebug() << "Waiting until duduregi-vt-handler launched";
            while(!s.vtHandlerStarted()) {
                app.processEvents();
            }
            qDebug() << "duduregi-vt-handler has been launched";

            // XXX setuid
            QRegExp user("--user=(\\S+)");
            int i = args.indexOf(user);
            if(i >= 0) {
                if(user.exactMatch(args.at(i))) {
                    struct passwd *pw =
                        getpwnam(user.capturedTexts()[1].toUtf8().constData());

                    if(!pw) {
                        qDebug() << "No such user" << user.capturedTexts()[1];
                        exit(1);
                    }

                    qDebug() << "setuid to " << pw->pw_name;
                    ::setuid(pw->pw_uid);
                    setgid(pw->pw_gid);
                    initgroups(pw->pw_name, pw->pw_gid);

                    char xdgruntimedir[32] = {0, };
                    sprintf(xdgruntimedir, "/run/user/%d", pw->pw_uid);
                    qDebug() << "reset XDG_RUNTIME_DIR to " << xdgruntimedir;
                    ::setenv("XDG_RUNTIME_DIR", xdgruntimedir, 1);
                }
            }
        }
    }
    return start_duduregi(&app);
}

#include "main.moc"
