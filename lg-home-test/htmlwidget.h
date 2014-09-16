/*
 * Copyright © 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#ifndef HTMLWIDGET_H
#define HTMLWIDGET_H

#include <QtWidgets>

class QWebView;
class QPushButton;

class HTMLWidget : public QWidget
{
    Q_OBJECT

public:
    HTMLWidget(const QUrl& url, QWidget *parent);

protected:
    virtual void resizeEvent(QResizeEvent * event);

protected slots:

    void backClicked();
    void setProgress(int p);
    void finishLoading(bool);


private:
    QWebView *view;
    QPushButton *button;
    int progress;
};
#endif
