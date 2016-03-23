/***************************************************************************
 *
 * Copyright 2010,2011 BMW Car IT GmbH
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
#ifndef __SHADERBASE_H__
#define __SHADERBASE_H__

#include "vec.h"

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


#include <string>
using std::string;

class IlmMatrix;

class ShaderBase : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
	ShaderBase(string vertexCode, string fragmentCode, IlmMatrix* projectionMatrix, QWidget *parent = 0);
	virtual ~ShaderBase();
	virtual void use(vec3f* position, vec4f* color);

protected:
    unsigned int shaderProgramId;
    QOpenGLShaderProgram program;
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

private:
    unsigned int fragmentShaderId;
    unsigned int vertexShaderId;
    int m_uniformProjectionMatrix;

private:
	bool initShader();
	bool destroyShader();

private:
	string m_vertexCode;
	string m_fragmentCode;
	IlmMatrix* m_projectionMatrix;
};

#endif // __SHADERBASE_H__
