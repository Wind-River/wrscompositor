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

#ifndef QWAYLANDINPUTMETHOD_H
#define QWAYLANDINPUTMETHOD_H

#include "qwayland-input-method.h"
#include <QDebug>

QT_BEGIN_NAMESPACE

namespace QtWaylandClient {

class QWaylandInputMethodContext : public QtWayland::wl_input_method_context
{
public:
    QWaylandInputMethodContext(struct ::wl_input_method_context *context);
    uint32_t getSerial() {
        return mSerial;
    }

protected:
    void input_method_context_surrounding_text(const QString &text, uint32_t cursor, uint32_t anchor);
    void input_method_context_reset();
    void input_method_context_content_type(uint32_t hint, uint32_t purpose);
    void input_method_context_invoke_action(uint32_t button, uint32_t index);
    void input_method_context_commit_state(uint32_t serial);
    void input_method_context_preferred_language(const QString &language);

private:
    uint32_t mSerial;
};

class QWaylandInputMethod : public QtWayland::wl_input_method
{
public:
    QWaylandInputMethod(struct ::wl_registry *registry, int id, int version);
    QWaylandInputMethodContext* getQWaylandInputMethodContext() {
        return mContext;
    }

protected:
    void input_method_activate(struct ::wl_input_method_context *id);
    void input_method_deactivate(struct ::wl_input_method_context *context);

private:
    QWaylandInputMethodContext* mContext;
};

}

QT_END_NAMESPACE

#endif // QWAYLANDINPUTMETHOD_H