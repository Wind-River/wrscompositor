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

#include "virtualkeyboard.h"
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusReply>

VirtualKeyboardPrivate::VirtualKeyboardPrivate(VirtualKeyboard *vk, QObject *parent) :
	mVK(vk), QObject(parent) {
}

void VirtualKeyboardPrivate::showKeyboard() {
	qDebug() << __func__;
	emit showKeyboardCalled();
}

void VirtualKeyboardPrivate::hideKeyboard() {
	qDebug() << __func__;
	emit hideKeyboardCalled();
}

bool VirtualKeyboardPrivate::keyboardVisible() {
	qDebug() << __func__;
	emit keyboardVisibleCalled();

	bool activeState = mVK->checkActiveState();
	qDebug() << __func__ << " activeState = " << activeState;
	return activeState;
}

VirtualKeyboard::VirtualKeyboard(QObject *parent) :
	QObject(parent) {

	mVKPrivate = new VirtualKeyboardPrivate(this);
	connect(mVKPrivate, SIGNAL(showKeyboardCalled()), this, SLOT(slotShowKeyboardRequested()));
	connect(mVKPrivate, SIGNAL(hideKeyboardCalled()), this, SLOT(slotHideKeyboardRequested()));
	connect(mVKPrivate, SIGNAL(keyboardVisibleCalled()), this, SLOT(slotKeyboardVisibleRequested()));

	mVKAdapter = new VirtualKeyboardAdaptor(mVKPrivate);

	if (!QDBusConnection::sessionBus().isConnected()) {
		qDebug() << "Could not connect to the D-Bus session in VirtualKeyboard.\n";
		return;
	}

    if (!QDBusConnection::sessionBus().registerObject("/VirtualKeyboard", mVKPrivate)) {
    	qFatal("Unable to register object at DBus in in VirtualKeyboard");
    	return;
    }

    if (!QDBusConnection::sessionBus().registerService("com.windriver.automotive.VirtualKeyboard")) {
    	qFatal("Unable to register at DBus in in VirtualKeyboard");
    	return;
    }

    mActive = false;
}

VirtualKeyboard::~VirtualKeyboard() {
	delete mVKAdapter;
	delete mVKPrivate;
}

void VirtualKeyboard::slotShowKeyboardRequested() {
	qDebug() << __func__;
	emit showKeyboardRequested();
}

void VirtualKeyboard::slotHideKeyboardRequested() {
	qDebug() << __func__;
	emit hideKeyboardRequested();
}

void VirtualKeyboard::slotKeyboardVisibleRequested() {
	qDebug() << __func__;
	emit keyboardVisibleRequested();
}

void VirtualKeyboard::sendKeyCode(const QString &keycode) {
	qDebug() << "VirtualKeyboard::sendKeyCode(), keycode = " << keycode;
	emit mVKPrivate->keyCodePressed(keycode);
}

void VirtualKeyboard::sendSpecialKeyCode(const QString &keycode) {
	qDebug() << "VirtualKeyboard::sendSpecialKeyCode(), keycode = " << keycode;
	emit mVKPrivate->specialKeyCodePressed(keycode);
}