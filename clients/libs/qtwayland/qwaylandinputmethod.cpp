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

#include "qwaylandinputmethod.h"

QT_BEGIN_NAMESPACE

namespace QtWaylandClient {

QWaylandInputMethodContext::QWaylandInputMethodContext(struct ::wl_input_method_context *context)
    : wl_input_method_context(context), mSerial(0)
{
    qDebug() << "[QWaylandInputMethodContext]" << __FILE__ << ":" << __LINE__ << "(" << __func__ << ")";
}

void QWaylandInputMethodContext::input_method_context_surrounding_text(const QString &text, uint32_t cursor, uint32_t anchor)
{
    qDebug() << "[QWaylandInputMethodContext]" << __FILE__ << ":" << __LINE__ << "(" << __func__ << ")";
}

void QWaylandInputMethodContext::input_method_context_reset()
{
    qDebug() << "[QWaylandInputMethodContext]" << __FILE__ << ":" << __LINE__ << "(" << __func__ << ")";
}

void QWaylandInputMethodContext::input_method_context_content_type(uint32_t hint, uint32_t purpose)
{
    qDebug() << "[QWaylandInputMethodContext]" << __FILE__ << ":" << __LINE__ << "(" << __func__ << ")";
}

void QWaylandInputMethodContext::input_method_context_invoke_action(uint32_t button, uint32_t index)
{
    qDebug() << "[QWaylandInputMethodContext]" << __FILE__ << ":" << __LINE__ << "(" << __func__ << ")";
}

void QWaylandInputMethodContext::input_method_context_commit_state(uint32_t serial)
{
    qDebug() << "[QWaylandInputMethodContext]" << __FILE__ << ":" << __LINE__ << "(" << __func__ << ")";
    mSerial = serial;
}

void QWaylandInputMethodContext::input_method_context_preferred_language(const QString &language)
{
    qDebug() << "[QWaylandInputMethodContext]" << __FILE__ << ":" << __LINE__ << "(" << __func__ << ")";
}

QWaylandInputMethod::QWaylandInputMethod(struct ::wl_registry *registry, int id, int version)
    : wl_input_method (registry, id, version), mContext(NULL)
{
}

void QWaylandInputMethod::input_method_activate(struct ::wl_input_method_context *id)
{
    qDebug() << "[QWaylandInputMethodContext]" << __FILE__ << ":" << __LINE__ << "(" << __func__ << ")";
    mContext = new QWaylandInputMethodContext(id);
}

void QWaylandInputMethod::input_method_deactivate(struct ::wl_input_method_context *context)
{
    qDebug() << "[QWaylandInputMethodContext]" << __FILE__ << ":" << __LINE__ << "(" << __func__ << ")";
    delete mContext;
}

}

QT_END_NAMESPACE