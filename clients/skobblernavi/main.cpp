#include <QtWidgets>
#include <QtNetwork>
#include <QtWebKitWidgets>


int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    QUrl url = QUrl("qrc:///html5/index.html");

    QWebView w;
    w.load(url);
    w.showFullScreen();

    return app.exec();
}
