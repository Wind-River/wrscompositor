#include <QtWidgets>
#include <QtNetwork>
#include <QtWebKitWidgets>


int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    QUrl url = QUrl("qrc:///html5/index.html");

    QWebView w;
    w.load(url);
    if(getenv("WRSCOMPOSITOR_WINDOW_DEFAULT_WIDTH")!=NULL) {
        w.resize(atoi(getenv("WRSCOMPOSITOR_WINDOW_DEFAULT_WIDTH")),
                atoi(getenv("WRSCOMPOSITOR_WINDOW_DEFAULT_HEIGHT")));
        w.show();
    } else
        w.showFullScreen();

    return app.exec();
}
