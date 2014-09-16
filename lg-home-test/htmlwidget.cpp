/*
 * Copyright © 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#include <QtWidgets>
#include <QtNetwork>
#include <QtWebKitWidgets>
#include <QPushButton>

#include "htmlwidget.h"
#include "menuhandler.h"

HTMLWidget::HTMLWidget(const QUrl& url)
    : QWidget(NULL)
{
    progress = 0;

    QNetworkProxyFactory::setUseSystemConfiguration(true);

    view = new QWebView(this);
    view->load(url);
    connect(view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
    connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));

    button = new QPushButton(tr("Back"), this);
    connect(button, SIGNAL(clicked()), SLOT(backClicked()));
}
void HTMLWidget::resizeEvent(QResizeEvent * event) {
    view->resize(event->size());
    int buttonMargin = 50;
    button->move(event->size().width() - button->size().width() - buttonMargin,
                buttonMargin);
}

void HTMLWidget::setProgress(int p)
{
    progress = p;
}

void HTMLWidget::finishLoading(bool)
{
    progress = 100;
}

void HTMLWidget::backClicked()
{
    MenuHandler::mainQmlView->show();
    MenuHandler::htmlView->hide();
}
