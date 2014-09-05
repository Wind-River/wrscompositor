/*
 * Copyright © 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#include <QtWidgets>

class QWebView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QUrl& url);

protected slots:

    void setProgress(int p);
    void finishLoading(bool);


private:
    QWebView *view;
    int progress;
};
