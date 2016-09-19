/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
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