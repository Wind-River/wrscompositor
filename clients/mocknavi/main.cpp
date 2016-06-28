/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QApplication>
#include <QLabel>
#include <QSurfaceFormat>
#include "Argument.h"
#include "ShaderLighting.h"
#include "LayerScene.h"

#include <iostream>
using std::cout;

#define DEFAULT_FPS      30.0
#define DEFAULT_ANIM     30.0
#define DEFAULT_SURFACE  SURFACE_EXAMPLE_EGLX11_APPLICATION
#define DEFAULT_WIDTH    1920
#define DEFAULT_HEIGHT   1080
#define DEFAULT_OPACITY  1.0
#define DEFAULT_NOSKY    false
#define DEFAULT_HELP     false


#ifndef QT_NO_OPENGL
#include "mainwidget.h"
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    app.setApplicationName("cube");
    app.setApplicationVersion("0.1");
#ifndef QT_NO_OPENGL
    //MainWidget widget;
    //widget.show();
    /*
    FloatArgument fps("fps", DEFAULT_FPS, argc, argv);
    FloatArgument animSpeed("anim", DEFAULT_ANIM, argc, argv);
    UnsignedIntArgument surfaceId("surface", DEFAULT_SURFACE, argc, argv);
    IntArgument width("width", DEFAULT_WIDTH, argc, argv);
    IntArgument height("height", DEFAULT_HEIGHT, argc, argv);
    BoolArgument nosky("nosky", DEFAULT_NOSKY, argc, argv);
    BoolArgument help("help", DEFAULT_HELP, argc, argv);

    if (help.get())
    {
        cout << "\nUsage: " << argv[0] << " [options]\n"
             << "possible options are:\n"
             << "  -help         show this help text (default " << DEFAULT_HELP << ")\n"
             << "  -fps x        limit frames per second to x (default " << DEFAULT_FPS << ")\n"
             << "  -anim x       set animation speed (default " << DEFAULT_ANIM << ")\n"
             << "  -nosky        do not render sky, background transparent (default " << DEFAULT_NOSKY << ")\n"
             << "  -surface x    render to surface id x (default " << DEFAULT_SURFACE << ")\n"
             << "  -width x      set surface width to x (default " << DEFAULT_WIDTH << ")\n"
             << "  -height x     set surface height to x (default " << DEFAULT_HEIGHT << ")\n\n";
    }
    else
    {
        MockNavi navi(fps.get(), animSpeed.get());
    }
    */
    ShaderLighting navi(DEFAULT_FPS, DEFAULT_ANIM);
    navi.resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    navi.show();
#else
    QLabel note("OpenGL Support required");
    note.show();
#endif
    return app.exec();
}
