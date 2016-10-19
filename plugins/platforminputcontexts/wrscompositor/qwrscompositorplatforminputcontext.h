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
