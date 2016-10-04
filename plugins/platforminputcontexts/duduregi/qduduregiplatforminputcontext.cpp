#include "qduduregiplatforminputcontext.h"

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QGuiApplication>
#include <QInputMethodEvent>
#include <QWidget>
#include <QDebug>

QDuduregiPlatformInputContext::QDuduregiPlatformInputContext()
    : m_focusObject(0) {
    if (!QDBusConnection::sessionBus().isConnected()) {
        qDebug() << "Could not connect to the D-Bus session in QDuduregiPlatformInputContext.\n";
        return;
    }
    m_keyboardInterface = new QDBusInterface("com.windriver.automotive.VirtualKeyboard", "/VirtualKeyboard", "com.windriver.automotive.VirtualKeyboard", QDBusConnection::sessionBus(), this);

    connect(m_keyboardInterface, SIGNAL(keyCodePressed(QString)), this, SLOT(keyboardKeyPressed(QString)));
    connect(m_keyboardInterface, SIGNAL(specialKeyCodePressed(QString)), this, SLOT(keyboardSpecialKeyPressed(QString)));
}

QDuduregiPlatformInputContext::~QDuduregiPlatformInputContext()
{
}

void QDuduregiPlatformInputContext::update(Qt::InputMethodQueries) {
    qDebug() << "QDuduregiPlatformInputContext::update()";
}

bool QDuduregiPlatformInputContext::isValid() const
{
    bool result = m_keyboardInterface->isValid();
    qDebug() << "QDuduregiPlatformInputContext::isValid(), result = " << result;
    return result;
}

void QDuduregiPlatformInputContext::setFocusObject(QObject *object) 
{
    qDebug() << "QDuduregiPlatformInputContext::setFocusObject(), object = " << object;
    m_focusObject = object;
}

void QDuduregiPlatformInputContext::showInputPanel()
{
    qDebug() << "QDuduregiPlatformInputContext::showInputPanel()";
    m_keyboardInterface->call("showKeyboard");
}

void QDuduregiPlatformInputContext::hideInputPanel()
{
    qDebug() << "QDuduregiPlatformInputContext::hideInputPanel()";
    m_keyboardInterface->call("hideKeyboard");
}

bool QDuduregiPlatformInputContext::isInputPanelVisible() const
{
    const QDBusReply<bool> reply = m_keyboardInterface->call("keyboardVisible");
    qDebug() << "QDuduregiPlatformInputContext::isInputPanelVisible(), reply = " << reply;

    if (reply.isValid())
        return reply.value();
    else
        return false;
}

void QDuduregiPlatformInputContext::keyboardSpecialKeyPressed(const QString &keycode)
{
    if (!m_focusObject)
        return;

    qDebug() << "QDuduregiPlatformInputContext::keyboardSpecialKeyPressed()";
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

void QDuduregiPlatformInputContext::keyboardKeyPressed(const QString &keycode)
{
    if (!m_focusObject)
        return;

    qDebug() << "QDuduregiPlatformInputContext::keyboardKeyPressed(), keycode = " << keycode << " m_focusObject = " << m_focusObject;
    QInputMethodEvent event;
    event.setCommitString(keycode);

    QGuiApplication::sendEvent(m_focusObject, &event);
}