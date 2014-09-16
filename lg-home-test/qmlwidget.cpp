#include "qmlwidget.h"

QMLWidget::QMLWidget(QWidget *parent)
{
    setSource(QUrl("home.qml"));
    setResizeMode(QQuickView::SizeRootObjectToView);
    setColor(Qt::black);
}
