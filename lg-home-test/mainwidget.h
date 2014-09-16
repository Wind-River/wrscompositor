/*
 * Copyright © 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#ifndef MW_H
#define MW_H

#include <QtWidgets>
#include "htmlwidget.h"
#include "qmlwidget.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget();

protected:
    virtual void resizeEvent(QResizeEvent * event);

protected slots:

private:
    HTMLWidget *html;
    QMLWidget *qml;
};
#endif
