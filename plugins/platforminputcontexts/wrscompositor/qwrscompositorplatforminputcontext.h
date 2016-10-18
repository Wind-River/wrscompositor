/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */

#ifndef QWRSCOMPOSITORPLATFORMINPUTCONTEXT_H
#define QWRSCOMPOSITORPLATFORMINPUTCONTEXT_H

#include <qpa/qplatforminputcontext.h>

QT_BEGIN_NAMESPACE

class QDBusInterface;

class QWrsCompositorPlatformInputContext : public QPlatformInputContext
{
    Q_OBJECT

public:
    QWrsCompositorPlatformInputContext();
    ~QWrsCompositorPlatformInputContext();

    bool isValid() const Q_DECL_OVERRIDE;
    void update(Qt::InputMethodQueries) Q_DECL_OVERRIDE;
    void setFocusObject(QObject *object) Q_DECL_OVERRIDE;

    void showInputPanel() Q_DECL_OVERRIDE;
    void hideInputPanel() Q_DECL_OVERRIDE;
    bool isInputPanelVisible() const Q_DECL_OVERRIDE;

private slots:
    void keyboardSpecialKeyPressed(const QString &keycode);
    void keyboardKeyPressed(const QString &keycode);

private:
    QDBusInterface *m_keyboardInterface;
    QObject *m_focusObject;
};

QT_END_NAMESPACE

#endif
