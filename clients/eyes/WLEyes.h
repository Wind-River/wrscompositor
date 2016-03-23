/**************************************************************************
 *
 * Copyright 2010, 2011 BMW Car IT GmbH
 * Copyright (C) 2011 DENSO CORPORATION and Robert Bosch Car Multimedia Gmbh
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
#ifndef _WLEYES_H_
#define _WLEYES_H_

#include "transform.h"
#include <QEvent>
#include <QTouchEvent>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>


//////////////////////////////////////////////////////////////////////////////
typedef struct _es2shaderobject {
    GLuint fragmentShaderId;
    GLuint vertexShaderId;
    GLuint shaderProgramId;
    GLint  matrixLocation;
    GLint  colorLocation;
} ES2ShaderObject;

typedef struct _es2vertexbufferobject {
    GLuint vbo;
} ES2VertexBuffer;


class WLEye
{
private:
    int     m_nPoint;
    TPoint* m_eyeLiner;
    int     m_nPupilPoint;
    TPoint* m_pupil;
    int     m_nWhiteEyePoint;
    TPoint* m_whiteEye;

public:
    explicit WLEye();
    virtual ~WLEye();

    void CreateEyeLiner(const float& centerX,
                        const float& centerY,
                        const float& diam,
                        const Transform& trans);
    void CreatePupil(const float& centerX,
                     const float& centerY,
                     const float& diam,
                     const Transform& trans);

    void GetEyeLinerGeom(int* nPoint, float** points);
    void GetWhiteEyeGeom(int* nPoint, float** points);
    void GetPupilGeom(int* nPoint, float** points);

private:
    void CreateWhiteEye(const float& centerX,
                        const float& centerY,
                        const float& diam,
                        const Transform& trans);
};

//////////////////////////////////////////////////////////////////////////////

class WLEyes : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
private:
    WLEye    m_eyes[2];
    int       m_width;
    int       m_height;
    Transform m_trans;
    int gPointerX, gPointerY;
    int lastPointerX, lastPointerY;
    ES2ShaderObject gShaderObject;
    ES2VertexBuffer gVertexBuffer;
// Fragment shader code
    char* sourceFragShader = "  \
    uniform mediump vec4 u_color; \
    void main(void)               \
    {                             \
        gl_FragColor = u_color;   \
    }";

// Vertex shader code
    char* sourceVertexShader = " \
    attribute highp vec4 a_vertex; \
    uniform mediump mat4 u_matrix; \
    void main(void)                \
    {                              \
        gl_Position = u_matrix * a_vertex; \
    }";


    void drawFillPoly(const int nPoint, const float* points, const float color[4]);
    bool initShader();
    bool initVertexBuffer();
    void attachVertexBuffer();
    char* buildShaderErrorLog(GLuint id);
    void terminateRenderer();

    QOpenGLShaderProgram program;
    QBasicTimer timer;

public:
    explicit WLEyes(QWidget *parent = 0);
    virtual ~WLEyes();

    void SetPointOfView(int mousePosX, int mousePosY);

    void GetEyeLinerGeom(int n, int* nPoint, float** points);
    void GetWhiteEyeGeom(int n, int* nPoint, float** points);
    void GetPupilGeom(int n, int* nPoint, float** points);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *e) Q_DECL_OVERRIDE;
    //void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void initShaders();
    bool event(QEvent *event);
};

#endif /* _WLEYES_H_ */
