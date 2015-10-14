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
#ifndef SHADERLIGHTING_H_
#define SHADERLIGHTING_H_

#include "ShaderBase.h"
#include "House.h"
#include "Street.h"
#include "Ground.h"
#include "Car.h"
#include "Camera.h"
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
#include <list>
using std::list;

#include <sys/time.h>
#include <unistd.h>
class IlmMatrix;
class MockNavi;
class Camera;
class Ground;
class House;
class Car;

class ShaderLighting: public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
	ShaderLighting(float fps, float animationSpeed, QWidget* parent = 0);

	~ShaderLighting();

	void use(vec3f* position, vec4f* color);
private:
	int m_uniformModelMatrix;
	int m_uniformColor;
    unsigned int fragmentShaderId;
    unsigned int vertexShaderId;
    int m_uniformProjectionMatrix;

	string m_vertexCode;
	string m_fragmentCode;
	IlmMatrix* m_projectionMatrix;

    QBasicTimer timer;

    Camera m_camera;
    int m_houseCount;
    list<IRenderable*> m_renderList;
    list<IUpdateable*> m_updateList;

	bool initShader();
	bool destroyShader();
    void generateCity();
    void render();
    void updateObject(int currentTimeInMS, int lastFrameTime);
    unsigned int frameHistory;
    unsigned int getTickCount();
    unsigned int startTimeInMS;
    unsigned int frameStartTimeInMS;
    unsigned int renderTimeInMS;
    unsigned int frameEndTimeInMS;
    unsigned int frameTimeInMS;
 

protected:
    float m_framesPerSecond;
    float m_animationSpeed;
    unsigned int m_timerIntervalInMs;


    unsigned int shaderProgramId;
    QOpenGLShaderProgram program;
    void timerEvent(QTimerEvent *e) Q_DECL_OVERRIDE;
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
};

#endif /* SHADERLIGHTING_H_ */
