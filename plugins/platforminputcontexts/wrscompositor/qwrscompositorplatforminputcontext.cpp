/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "qwrscompositorplatforminputcontext.h"

#include <QGuiApplication>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QInputMethodEvent>
#include <QDebug>

QWrsCompositorPlatformInputContext::QWrsCompositorPlatformInputContext()
    : m_focusObject(0) {
    if (!QDBusConnection::sessionBus().isConnected()) {
        qDebug() << "Could not connect to the D-Bus session in QWrsCompositorPlatformInputContext.\n";
        return;
    }
    m_keyboardInterface = new QDBusInterface("com.windriver.automotive.VirtualKeyboard", "/VirtualKeyboard", "com.windriver.automotive.VirtualKeyboard", QDBusConnection::sessionBus(), this);

    connect(m_keyboardInterface, SIGNAL(keyCodePressed(QString)), this, SLOT(keyboardKeyPressed(QString)));
    connect(m_keyboardInterface, SIGNAL(specialKeyCodePressed(QString)), this, SLOT(keyboardSpecialKeyPressed(QString)));
}

QWrsCompositorPlatformInputContext::~QWrsCompositorPlatformInputContext()
{
}

void QWrsCompositorPlatformInputContext::update(Qt::InputMethodQueries) {
    qDebug() << "QWrsCompositorPlatformInputContext::update()";
}

bool QWrsCompositorPlatformInputContext::isValid() const
{
    bool result = m_keyboardInterface->isValid();
    qDebug() << "QWrsCompositorPlatformInputContext::isValid(), result = " << result;
    return result;
}

void QWrsCompositorPlatformInputContext::setFocusObject(QObject *object) 
{
    qDebug() << "QWrsCompositorPlatformInputContext::setFocusObject(), object = " << object;
    m_focusObject = object;
}

void QWrsCompositorPlatformInputContext::showInputPanel()
{
    qDebug() << "QWrsCompositorPlatformInputContext::showInputPanel()";
    m_keyboardInterface->call("showKeyboard");
}

void QWrsCompositorPlatformInputContext::hideInputPanel()
{
    qDebug() << "QWrsCompositorPlatformInputContext::hideInputPanel()";
    m_keyboardInterface->call("hideKeyboard");
}

bool QWrsCompositorPlatformInputContext::isInputPanelVisible() const
{
    const QDBusReply<bool> reply = m_keyboardInterface->call("keyboardVisible");
    qDebug() << "QWrsCompositorPlatformInputContext::isInputPanelVisible(), reply = " << reply;

    if (reply.isValid())
        return reply.value();
    else
        return false;
}

void QWrsCompositorPlatformInputContext::keyboardSpecialKeyPressed(const QString &keycode)
{
    if (!m_focusObject)
        return;

    qDebug() << "QWrsCompositorPlatformInputContext::keyboardSpecialKeyPressed()";
    if (keycode == "enter") {
        qDebug() << "Pressed enter button";
        QKeyEvent *pressEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
        QGuiApplication::postEvent(m_focusObject, pressEvent);

        QKeyEvent *releaseEvent = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Enter, Qt::NoModifier);
        QGuiApplication::postEvent(m_focusObject, releaseEvent);
    } else if (keycode == "backspace") {
        qDebug() << "Pressed backspace button";
        QKeyEvent *pressEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
        QGuiApplication::postEvent(m_focusObject, pressEvent);

        QKeyEvent *releaseEvent = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Backspace, Qt::NoModifier);
        QGuiApplication::postEvent(m_focusObject, releaseEvent);
    }
}

void QWrsCompositorPlatformInputContext::keyboardKeyPressed(const QString &keycode)
{
    if (!m_focusObject)
        return;

    qDebug() << "QWrsCompositorPlatformInputContext::keyboardKeyPressed(), keycode = " << keycode << " m_focusObject = " << m_focusObject;
    QInputMethodEvent event;
    event.setCommitString(keycode);

    QGuiApplication::sendEvent(m_focusObject, &event);
}
