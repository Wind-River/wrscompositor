#ifndef QIVIWINDOW_H
#define QIVIWINDOW_H

#include <QMainWindow>
#include "qtwaylandiviapplication.h"

class QIVIWindow : public QMainWindow
{
    Q_OBJECT

public:
    int                                         mIviID;
    QtWaylandClient::QtWaylandIviApplication    *mIviApp;
    QtWaylandClient::QtWaylandIviSurface        *mIviSurface;
    QtWaylandClient::QtWaylandIviSurface        *mIviSurfaceInst;

    explicit QIVIWindow(QtWaylandClient::QtWaylandIviApplication *iviApp, int iviId);
    void repaint();

signals:

public slots:

protected:
    bool event(QEvent *event);
};

#endif // QIVIWINDOW_H
