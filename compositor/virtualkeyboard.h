/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */

 #ifndef VIRTUALKEYBOARD_H
 #define VIRTUALKEYBOARD_H

#include <QtCore/QObject>
#include <QDBusInterface>
#include <QDBusConnection>
#include <QDebug>
#include "virtualkeyboard_adaptor.h"

class VirtualKeyboard;
class VirtualKeyboardPrivate : public QObject
{
	Q_OBJECT
public:
	explicit VirtualKeyboardPrivate(VirtualKeyboard *vk, QObject *parent = 0);
Q_SIGNALS:
	void keyCodePressed(const QString &keycode);
	void specialKeyCodePressed(const QString &keycode);
public Q_SLOTS:
	void showKeyboard();
	void hideKeyboard();
	bool keyboardVisible();
signals:
	void showKeyboardCalled();
	void hideKeyboardCalled();
	void keyboardVisibleCalled();
private:
	VirtualKeyboard *mVK;
};

 class VirtualKeyboard: public QObject 
 {
 	Q_OBJECT
public:
 	explicit VirtualKeyboard(QObject *parent = 0);
 	~VirtualKeyboard();
 	Q_INVOKABLE void sendKeyCode(const QString &keycode);
 	Q_INVOKABLE void sendSpecialKeyCode(const QString &keycode);
	Q_INVOKABLE void sendActiveState(bool active) { mActive = active; }
	bool checkActiveState() {
		return mActive;
	}

signals:
 	void showKeyboardRequested();
 	void hideKeyboardRequested();
 	void keyboardVisibleRequested();

private:
	VirtualKeyboardPrivate *mVKPrivate;
 	VirtualKeyboardAdaptor *mVKAdapter;
 	
private slots:
 	void slotShowKeyboardRequested();
 	void slotHideKeyboardRequested();
 	void slotKeyboardVisibleRequested();

private:
	bool mActive;
 };

 #endif