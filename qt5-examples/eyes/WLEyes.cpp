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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "WLEyes.h"

#define EYE_X(n)   ((n) * 2.0)
#define EYE_Y(n)   (0.0)
#define EYE_OFFSET (0.1)     /* padding between eyes */
#define EYE_THICK  (0.175)   /* thickness of eye rim */
#define BALL_DIAM  (0.3)
#define BALL_PAD   (0.175)
#define EYE_DIAM   (2.0 - (EYE_THICK + EYE_OFFSET) * 2)
#define BALL_DIST  ((EYE_DIAM - BALL_DIAM) / 2.0 - BALL_PAD)

//#define W_MIN_X    (-1.0 + EYE_OFFSET)
//#define W_MAX_X    ( 3.0 - EYE_OFFSET)
//#define W_MIN_Y    (-1.0 + EYE_OFFSET)
//#define W_MAX_Y    ( 1.0 - EYE_OFFSET)
#define W_MIN_X    (-1.05 + EYE_OFFSET)
#define W_MAX_X    ( 3.05 - EYE_OFFSET)
#define W_MIN_Y    (-1.05 + EYE_OFFSET)
#define W_MAX_Y    ( 1.05 - EYE_OFFSET)

#define TPOINT_NONE        (-1000)   /* special value meaning "not yet set" */
#define TPointEqual(a, b)  ((a).x == (b).x && (a).y == (b).y)
#define XPointEqual(a, b)  ((a).x == (b).x && (a).y == (b).y)
#define AngleBetween(A, A0, A1) (A0 <= A1 ? A0 <= A && A <= A1 : \
                                 A0 <= A || A <= A1)

//////////////////////////////////////////////////////////////////////////////

static void
SetTransform(Transform* t, int xx1, int xx2, int xy1, int xy2,
             float tx1, float tx2, float ty1, float ty2)
{
    t->mx = ((float) xx2 - xx1) / (tx2 - tx1);
    t->bx = ((float) xx1) - t->mx * tx1;
    t->my = ((float) xy2 - xy1) / (ty2 - ty1);
    t->by = ((float) xy1) - t->my * ty1;
}

static void
Trectangle(const Transform *t, const TRectangle *i, TRectangle *o)
{
    o->x = t->mx * i->x + t->bx;
    o->y = t->my * i->y + t->by;
    o->width = t->mx * i->width;
    o->height = t->my * i->height;
    if (o->width < 0) {
        o->x += o->width;
        o->width = -o->width;
    }
    if (o->height < 0) {
        o->y += o->height;
        o->height = -o->height;
    }
}

static TPoint
ComputePupil(int num, TPoint mouse, const TRectangle* screen)
{
   float   cx, cy;
   float   dist;
   float   angle;
   float   dx, dy;
   float   cosa, sina;
   TPoint   ret;

   cx = EYE_X(num); dx = mouse.x - cx;
   cy = EYE_Y(num); dy = mouse.y - cy;
   if (dx == 0 && dy == 0);
   else {
      angle = atan2 ((float) dy, (float) dx);
      cosa = cos (angle);
      sina = sin (angle);
      dist = BALL_DIST;
      if (screen)
      {
          /* use distance mapping */
          float x0, y0, x1, y1;
          float a[4];
          x0 = screen->x - cx;
          y0 = screen->y - cy;
          x1 = x0 + screen->width;
          y1 = y0 + screen->height;
          a[0] = atan2(y0, x0);
          a[1] = atan2(y1, x0);
          a[2] = atan2(y1, x1);
          a[3] = atan2(y0, x1);
          if (AngleBetween(angle, a[0], a[1]))
          {
              /* left */
              dist *= dx / x0;
          }
          else if (AngleBetween(angle, a[1], a[2]))
          {
              /* bottom */
              dist *= dy / y1;
          }
          else if (AngleBetween(angle, a[2], a[3]))
          {
              /* right */
              dist *= dx / x1;
          }
          else if (AngleBetween(angle, a[3], a[0]))
          {
              /* top */
              dist *= dy / y0;
          }
          if (dist > BALL_DIST)
              dist = BALL_DIST;
      }
      if (dist > hypot ((double) dx, (double) dy)) {
          cx += dx;
          cy += dy;
      } else {
          cx += dist * cosa;
          cy += dist * sina;
      }
   }
   ret.x =  cx;
   ret.y = -cy;
   return ret;
}

static void
CreateEllipsePoints(const float& centerX, const float& centerY,
                    const float& diam, const Transform& trans,
                    int& nPoint, TPoint*& points)
{
    int n, i;
    float hd, c, s, sx, sy, x, y, px, py;

    const TRectangle tpos = {
        centerX - diam * 0.5f,
        centerY - diam * 0.5f,
        diam, diam };
    TRectangle pos;

    Trectangle(&trans, &tpos, &pos);

    pos.x = pos.x + pos.width  * 0.5;
    pos.y = pos.y + pos.height * 0.5;

    hd = hypot(pos.width, pos.height) * 0.5;
    n = (M_PI / acos(hd / (hd + 1.0))) + 0.5;
    if (n < 2) n = 2;

    c = cos(M_PI / n);
    s = sin(M_PI / n);
    sx = -(pos.width  * s) / pos.height;
    sy =  (pos.height * s) / pos.width;

    n *= 2;
    n += 2; // adds center point and first point
    points = (TPoint*)malloc(sizeof(TPoint) * n);
    if (!points)
        return;

    // add center point
    points[0].x = pos.x;
    points[0].y = pos.y;

    x = 0;
    y = pos.height * 0.5;
    for (i = 1; i < n; ++i){
        points[i].x = x + pos.x;
        points[i].y = y + pos.y;
        px = x;
        py = y;
        x = c * px + sx * py;
        y = c * py + sy * px;
    }

    // add first point
    points[n-1] = points[1];

    nPoint = n;
}

//////////////////////////////////////////////////////////////////////////////

WLEye::WLEye()
: m_nPoint(0)
, m_eyeLiner(NULL)
, m_nPupilPoint(0)
, m_pupil(NULL)
, m_nWhiteEyePoint(0)
, m_whiteEye(NULL)
{
}


WLEye::~WLEye()
{
    if (m_nPoint > 0 && m_eyeLiner){
        free(m_eyeLiner);
    }
    if (m_nPupilPoint > 0 && m_pupil){
        free(m_pupil);
    }
    if (m_nWhiteEyePoint > 0 && m_whiteEye){
        free(m_whiteEye);
    }
}

void
WLEye::CreateEyeLiner(const float& centerX, const float& centerY,
                      const float& diam, const Transform& trans)
{
    if (m_nPoint > 0 && m_eyeLiner){
        free(m_eyeLiner);
        m_nPoint = 0;
    }
    CreateEllipsePoints(centerX, centerY, diam, trans, m_nPoint, m_eyeLiner);

    CreateWhiteEye(centerX, centerY, diam * 0.8, trans);
}

void
WLEye::CreateWhiteEye(const float& centerX, const float& centerY,
                      const float& diam, const Transform& trans)
{
    if (m_nWhiteEyePoint > 0 && m_whiteEye){
        free(m_whiteEye);
        m_nWhiteEyePoint = 0;
    }
    CreateEllipsePoints(centerX, centerY, diam, trans, m_nWhiteEyePoint, m_whiteEye);
}

void
WLEye::CreatePupil(const float& centerX, const float& centerY,
                   const float& diam, const Transform& trans)
{
    if (m_nPupilPoint > 0 && m_pupil){
        free(m_pupil);
        m_nPupilPoint = 0;
    }
    CreateEllipsePoints(centerX, centerY, diam, trans, m_nPupilPoint, m_pupil);
}

void
WLEye::GetEyeLinerGeom(int* nPoint, float** points)
{
    *nPoint = m_nPoint;
    *points = (float*)m_eyeLiner;
}

void
WLEye::GetWhiteEyeGeom(int* nPoint, float** points)
{
    *nPoint = m_nWhiteEyePoint;
    *points = (float*)m_whiteEye;
}

void
WLEye::GetPupilGeom(int* nPoint, float** points)
{
    *nPoint = m_nPupilPoint;
    *points = (float*)m_pupil;
}

//////////////////////////////////////////////////////////////////////////////

WLEyes::WLEyes(QWidget *parent)
: QOpenGLWidget(parent) 
, m_width(400)
, m_height(240)
{
   SetTransform(&m_trans, 0, m_width, m_height, 0,
                 W_MIN_X, W_MAX_X, W_MIN_Y, W_MAX_Y);

    m_eyes[0].CreateEyeLiner(0.0, 0.0, EYE_DIAM + 2.0 * EYE_THICK, m_trans);
    m_eyes[1].CreateEyeLiner(2.0, 0.0, EYE_DIAM + 2.0 * EYE_THICK, m_trans);

}

bool
WLEyes::event(QEvent *event)
{
    QMouseEvent *mouseEvent;
    QList<QTouchEvent::TouchPoint> touchPoints;
    switch(event->type()) {
        default:
            return QWidget::event(event);
            break;

        case QEvent::MouseButtonPress:
        case QEvent::MouseMove:
	    qDebug() << "Mouse Event Received : " << event->type();
            mouseEvent = static_cast<QMouseEvent*>(event);
            lastPointerX = gPointerX;
            lastPointerY = gPointerY;
            gPointerX = mouseEvent->x();
            gPointerY = mouseEvent->y();
            //qDebug("Pointer GET : %f %f", gPointerX, gPointerY);
            break;

        case QEvent::TouchBegin:
        case QEvent::TouchUpdate:
        case QEvent::TouchEnd:
	    qDebug() << "Touch Event Received : " << event->type();
            touchPoints = static_cast<QTouchEvent *>(event)->touchPoints();
            QPointF pos = touchPoints.first().pos();
            lastPointerX = gPointerX;
            lastPointerY = gPointerY;
            gPointerX = pos.x();
            gPointerY = pos.y();
            //qDebug("Pointer GET : %f %f", gPointerX, gPointerY);
            break;

    }
    return true;
}

void
WLEyes::timerEvent(QTimerEvent *)
{
    if (lastPointerX != gPointerX || lastPointerY != gPointerY) {
        update();
    }
}

void
WLEyes::initializeGL()
{
    initializeOpenGLFunctions();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
    glViewport(0, 0, 400, 240);

    glClearColor(0.2, 0.2, 0.2, 1.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    initVertexBuffer();
    initShader();
    timer.start(12, this);
    
 }

void
WLEyes::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}
void
WLEyes::terminateRenderer()
{
    glDeleteProgram(gShaderObject.shaderProgramId);
    glDeleteShader(gShaderObject.fragmentShaderId);
    glDeleteShader(gShaderObject.vertexShaderId);

    glDeleteBuffers(1, &gVertexBuffer.vbo);
}


char*
WLEyes::buildShaderErrorLog(GLuint id)
{
    int l, nChar;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &l);

    char* info = (char*)malloc(sizeof(char) * l);
    glGetShaderInfoLog(id, l, &nChar, info);

    return info;
}


void
WLEyes::drawFillPoly(const int nPoint, const float* points, const float color[4])
{
    glUniform4fv(gShaderObject.colorLocation, 1, color);

    unsigned int uiSize = nPoint * (sizeof(GLfloat) * 2);
    glBufferSubData(GL_ARRAY_BUFFER, 0, uiSize, points);

    glDrawArrays(GL_TRIANGLE_FAN, 0, nPoint);
}

bool
WLEyes::initShader()
{
    GLint result = 0;
    char* log = NULL;

    program.addShaderFromSourceCode(QOpenGLShader::Fragment,sourceFragShader);
    program.addShaderFromSourceCode(QOpenGLShader::Vertex,sourceVertexShader);
    program.bindAttributeLocation("a_vertex",0);
    program.link();
    program.bind();
    gShaderObject.shaderProgramId = program.programId();
    gShaderObject.matrixLocation = program.uniformLocation("u_matrix");
    gShaderObject.colorLocation = program.uniformLocation("u_color");


    /*
    //qDebug() << "BBB";
    // Create the fragment shader object
    gShaderObject.fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(gShaderObject.fragmentShaderId, 1,
        (const char**)&sourceFragShader, NULL);
    //qDebug() << "CCC";
    glCompileShader(gShaderObject.fragmentShaderId);

    glGetShaderiv(gShaderObject.fragmentShaderId, GL_COMPILE_STATUS, &result);

    if (!result){
        log = buildShaderErrorLog(gShaderObject.fragmentShaderId);
        fprintf(stderr, "Failed to compile fragment shader: %s\n", log);
        if (log) free(log);
        return false;
    }

    //qDebug() << "AAA";
    // Create the vertex shader object
    gShaderObject.vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(gShaderObject.vertexShaderId, 1,
        (const char**)&sourceVertexShader, NULL);
    glCompileShader(gShaderObject.vertexShaderId);

    glGetShaderiv(gShaderObject.vertexShaderId, GL_COMPILE_STATUS, &result);
    if (!result){
        log = buildShaderErrorLog(gShaderObject.vertexShaderId);
        //qDebug("Failed to compile vertex shader: %s\n", log);
        if (log) free(log);
        return false;
    }

    gShaderObject.shaderProgramId = glCreateProgram();

    glAttachShader(gShaderObject.shaderProgramId, gShaderObject.fragmentShaderId);
    glAttachShader(gShaderObject.shaderProgramId, gShaderObject.vertexShaderId);

    glBindAttribLocation(gShaderObject.shaderProgramId, 0, "a_vertex");

    glLinkProgram(gShaderObject.shaderProgramId);

    glGetProgramiv(gShaderObject.shaderProgramId, GL_LINK_STATUS, &result);
    if (!result){
        log = buildShaderErrorLog(gShaderObject.shaderProgramId);
        //qDebug("Failed to compile fragment shader: %s\n", log);
        if (log) free(log);
        return false;
    }

    glUseProgram(gShaderObject.shaderProgramId);
    gShaderObject.matrixLocation = glGetUniformLocation(
        gShaderObject.shaderProgramId, "u_matrix");
    gShaderObject.colorLocation = glGetUniformLocation(
        gShaderObject.shaderProgramId, "u_color");
    */

    return true;
}

bool
WLEyes::initVertexBuffer()
{
    glGenBuffers(1, &gVertexBuffer.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer.vbo);

    unsigned int uiSize = 100 * (sizeof(GLfloat) * 2);
    glBufferData(GL_ARRAY_BUFFER, uiSize, NULL, GL_STATIC_DRAW);

    return true;
}
void
WLEyes::attachVertexBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer.vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
}



void
WLEyes::paintGL()
{
    //qDebug() << "PAINT GL";
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(gShaderObject.shaderProgramId);

    attachVertexBuffer();
    {
        float a = 2.0 / 400.0;
        float b = 2.0 / 240.0;
        float mtx[16] = {   a,  0.0, 0.0, 0.0,
                          0.0,    b, 0.0, 0.0,
                          0.0,  0.0, 1.0, 0.0,
                         -1.0, -1.0, 0.0, 1.0};

        glUniformMatrix4fv(gShaderObject.matrixLocation, 1, GL_FALSE, mtx);

        SetPointOfView(gPointerX, gPointerY);
        //qDebug() << " X : " << gPointerX << "/  Y: " << gPointerY;
        float black[] = {0.0, 0.0, 0.0, 1.0};
        float white[] = {1.0, 1.0, 1.0, 1.0};
        int nPoint = 0;
        float* points = NULL;
        for (int ii = 0; ii < 2; ++ii){
            // pupil
            GetPupilGeom(ii, &nPoint, &points);
            if (nPoint <= 0 || points == NULL){
                //qDebug() << "continue";
                continue;
            }

            drawFillPoly(nPoint, points, black);


            // eye shape (white eye)
            GetWhiteEyeGeom(ii, &nPoint, &points);
            
            //qDebug("White - nPoint : %d / Points %f",nPoint, points);
            if (nPoint <= 0 || points == NULL){
                continue;
            }
            drawFillPoly(nPoint, points, white);

            // eye liner
            GetEyeLinerGeom(ii, &nPoint, &points);
            //qDebug() << "nPoint : " << nPoint << "Points : " << points;
            if (nPoint <= 0 || points == NULL){
                continue;
            }
            drawFillPoly(nPoint, points, black);

            /*same array is used for both eyes, execute finish for each eye*/
            glFinish();
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/*
void WLEyes::show()
{
    showFullScreen(); 
    m_eyes[0].show();
    m_eyes[1].show();
    //qDebug() << "SHOW START" << width() << " / " << height() << ": " << isValid();
    //qDebug() << "SHOW START1" << m_eyes[0].width() << " / " << m_eyes[0].height() << ": " << isValid();
    //qDebug() << "SHOW START2" << m_eyes[1].width() << " / " << m_eyes[1].height() << ": " << isValid();
}
*/

WLEyes::~WLEyes()
{
    makeCurrent();
    terminateRenderer();
    doneCurrent();
}

void
WLEyes::SetPointOfView(int mousePosX, int mousePosY)
{
    TPoint mouse;
    mouse.x = Tx(mousePosX, mousePosY, &m_trans);
    mouse.y = Ty(mousePosX, mousePosY, &m_trans);

    for (int i = 0; i < 2; ++i){
        TPoint newPupil = ComputePupil(i, mouse, NULL);
        m_eyes[i].CreatePupil(newPupil.x, newPupil.y, BALL_DIAM, m_trans);
    }
}

void
WLEyes::GetEyeLinerGeom(int n, int* nPoint, float** points)
{
    *nPoint = 0;
    *points = NULL;
    if (n < 3){
        m_eyes[n].GetEyeLinerGeom(nPoint, points);
    }
}

void
WLEyes::GetWhiteEyeGeom(int n, int* nPoint, float** points)
{
    *nPoint = 0;
    *points = NULL;
    if (n < 3){
        m_eyes[n].GetWhiteEyeGeom(nPoint, points);
    }
}

void
WLEyes::GetPupilGeom(int n, int* nPoint, float** points)
{
    *nPoint = 0;
    *points = NULL;
    if (n < 3){
        m_eyes[n].GetPupilGeom(nPoint, points);
    }
}
