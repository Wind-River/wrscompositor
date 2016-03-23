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
#ifndef _MOCKNAVI_H
#define _MOCKNAVI_H

#include "IRenderable.h"
#include "IUpdateable.h"
#include "ShaderLighting.h"
#include "Camera.h"

#include <list>
using std::list;

class MockNaviHouse;
class ShaderLighting;

class MockNavi : public QWidget
{
    Q_OBJECT
public:
    MockNavi(float fps, float animationSpeed, QWidget *parent = 0);

    ShaderLighting* mShader;
    virtual void update(int currentTimeInMs, int lastFrameTime);
    virtual void render();

protected:
    void timerEvent(QTimerEvent *e) Q_DECL_OVERRIDE;

private:
    void generateCity();
    unsigned int getTickCount();
    unsigned int startTimeInMS;
    unsigned int frameStartTimeInMS;
    unsigned int renderTimeInMS;
    unsigned int frameEndTimeInMS;
    unsigned int frameTimeInMS;
    QBasicTimer timer;

    Camera m_camera;
    int m_houseCount;
    float m_animationSpeed;
    list<IRenderable*> m_renderList;
    list<IUpdateable*> m_updateList;
};

#endif /* _MOCKNAVI_H */
