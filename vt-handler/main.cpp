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


class VTHandlerClient: public QLocalSocket {
    Q_OBJECT
public:
    VTHandlerClient(QObject *parent=Q_NULLPTR) : QLocalSocket(parent) {
        connect(this, SIGNAL(connected()), this, SLOT(slotConnected()));
    }
public slots:
    void slotConnected() {
        qDebug() << "connected";
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

        qDebug() << "fd" << socketDescriptor();
        do {
            len = recvmsg(socketDescriptor(), &msg, 0);
        } while (len < 0 && errno == EINTR);
        qDebug() << "recv len " << len;

        for(cmsg = CMSG_FIRSTHDR(&msg);
                cmsg != NULL;
                cmsg = CMSG_NXTHDR(&msg, cmsg))
        {
            if(cmsg->cmsg_level == SOL_SOCKET &&
                    cmsg->cmsg_type == SCM_RIGHTS) {
                return *((int*)CMSG_DATA(cmsg));
            }
        }
        return -1;
    }
};


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    VTHandlerClient c;
    c.connectToServer(".duduregi-vt");
    if(c.waitForConnected()) {
        int fd = c.readFd();
        qDebug() << "read drm_fd " << fd;
    }

    return app.exec();
}
#include "main.moc"
