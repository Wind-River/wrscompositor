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
#include "ShaderLighting.h"
#include "IlmMatrix.h"

#define CITY_GRID_SIZE 1.0f
#define RUNTIME_IN_MS() (getTickCount() - startTimeInMS)

const char* vertexShaderCode =
		    "attribute mediump vec4 a_vertex;                                 \
		     uniform mediump mat4 u_modelMatrix;                              \
		     varying mediump vec4 v_normal;                                   \
             void main(void)                                                  \
             {                                                                \
                 gl_Position = u_projectionMatrix * u_modelMatrix * a_vertex; \
		         v_normal = normalize(a_vertex);                              \
             }";

const char* fragmentShaderCode =
		    "uniform mediump vec4 u_color;   \
		     varying mediump vec4 v_normal;  \
		     mediump vec4 lightPosition;     \
             void main (void)                \
		     {                               \
		         lightPosition = normalize(vec4(-3.0, -5.0, 10.0, 1.0));   \
		         gl_FragColor = max(dot(v_normal, lightPosition), 0.0) * 0.5 * u_color + 0.8 * u_color;   \
		         gl_FragColor.a = 1.0;   \
		     }";

ShaderLighting::ShaderLighting(float fps, float animationSpeed, QWidget* parent)
: QOpenGLWidget(parent) 
, m_fragmentCode(fragmentShaderCode)
, m_camera(vec3f(-1.5 * CITY_GRID_SIZE, -0.1, 0.0), vec3f(0.0, 0.0, 0.0), 320, 240)
, m_houseCount(15)
, frameStartTimeInMS(0)
, renderTimeInMS(0)
, frameEndTimeInMS(0)
, frameTimeInMS(0)
, m_framesPerSecond(fps)
, m_animationSpeed(animationSpeed)
, m_timerIntervalInMs(1000.0 / m_framesPerSecond)


{
    m_vertexCode = "uniform mediump mat4 u_projectionMatrix;\n";
    m_vertexCode += vertexShaderCode;
    qDebug() << "ShaderLighting!!!!";
}

void
ShaderLighting::initializeGL()
{
    qDebug() << "******* INIT GL *******";
    initializeOpenGLFunctions();
    glClearColor(0.2f, 0.2f, 0.5f, 1.0f);
    glDisable(GL_BLEND);

    glClearDepthf(1.0f);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	initShader();

    program.bind();
    m_uniformModelMatrix = program.uniformLocation("u_modelMatrix");
    m_uniformColor = program.uniformLocation("u_color");
    generateCity();
}

ShaderLighting::~ShaderLighting()
{
    makeCurrent();
	destroyShader();
    doneCurrent();
}

bool ShaderLighting::initShader()
{
    GLint glResult = GL_FALSE;

    const char* fragmentShaderCode = m_fragmentCode.c_str();
    const char* vertexShaderCode = m_vertexCode.c_str();
    program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderCode);
    program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderCode);

    /*
    // Create the fragment shader object
    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    // Load Fragment Source
    glShaderSource(fragmentShaderId, 1, &fragmentShaderCode, NULL);

    // Compile the source code of fragment shader
    glCompileShader(fragmentShaderId);

    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &glResult);

    if (glResult == GL_FALSE)
    {
        t_ilm_int infoLength, numberChars;
        glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &infoLength);

        // Allocate Log Space
        char* info = (char*) malloc(sizeof(char) * infoLength);
        glGetShaderInfoLog(fragmentShaderId, infoLength, &numberChars, info);

        // Print the error
        cout << "Failed to compile fragment shader: " << info << "\n";
        free(info);
        return ILM_FALSE;
    }

    // Create the fragment shader object
    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);

    // Load Fragment Source
    glShaderSource(vertexShaderId, 1, &vertexShaderCode, NULL);

    // Compile the source code of fragment shader
    glCompileShader(vertexShaderId);

    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &glResult);

    if (glResult == GL_FALSE)
    {
        t_ilm_int infoLength, numberChars;
        glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &infoLength);

        // Allocate Log Space
        char* info = (char*) malloc(sizeof(char) * infoLength);
        glGetShaderInfoLog(vertexShaderId, infoLength, &numberChars, info);

        // Print the error
        cout << "Failed to compile vertex shader: " << info << "\n";
        free(info);
        return ILM_FALSE;
    }

    shaderProgramId = glCreateProgram();

    glAttachShader(shaderProgramId, fragmentShaderId);
    glAttachShader(shaderProgramId, vertexShaderId);
    */

    program.bindAttributeLocation("a_vertex", 0);
    program.link();
    program.bind();
    shaderProgramId = program.programId();
    /*
    glBindAttribLocation(shaderProgramId, 0, "a_vertex");

    glLinkProgram(shaderProgramId);

    glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &glResult);

    if (glResult == GL_FALSE)
    {
        t_ilm_int infoLength, numberChars;
        glGetShaderiv(shaderProgramId, GL_INFO_LOG_LENGTH, &infoLength);

        // Allocate Log Space
        char* info = (char*) malloc(sizeof(char) * infoLength);
        glGetShaderInfoLog(shaderProgramId, infoLength, &numberChars,
                info);

        // Print the error
        cout << "Failed to link program: " << info << "\n";
        free(info);
        return ILM_FALSE;
    }
    */
#if 1 /* ADIT */
    /*
    glValidateProgram(shaderProgramId);

    glGetProgramiv(shaderProgramId, GL_VALIDATE_STATUS, &glResult);

    if (glResult == GL_FALSE)
    {
        t_ilm_int infoLength, numberChars;
        glGetShaderiv(shaderProgramId, GL_INFO_LOG_LENGTH, &infoLength);

        // Allocate Log Space
        char* info = (char*) malloc(sizeof(char) * infoLength);
        glGetShaderInfoLog(shaderProgramId, infoLength, &numberChars,
                info);

        // Print the error
        cout << "Failed to validate program: " << info << "\n";
        free(info);
        return ILM_FALSE;
    }
    */
#endif /* ADIT */

    //glUseProgram(shaderProgramId);

    m_uniformProjectionMatrix = program.uniformLocation("u_projectionMatrix");
    //m_uniformProjectionMatrix = glGetUniformLocation(shaderProgramId, "u_projectionMatrix");

    qDebug() << "Shader setup complete.\n";

    return true;
}

bool ShaderLighting::destroyShader()
{
    bool result = true;
    glDeleteProgram(shaderProgramId);
    glDeleteShader(fragmentShaderId);
    glDeleteShader(vertexShaderId);
    return result;
}


void
ShaderLighting::paintGL()
{

    frameTimeInMS = frameEndTimeInMS - frameStartTimeInMS;
    frameStartTimeInMS = RUNTIME_IN_MS();
    updateObject(m_animationSpeed * frameStartTimeInMS, m_animationSpeed * frameTimeInMS);
    render();
    renderTimeInMS = RUNTIME_IN_MS() - frameStartTimeInMS;
    frameEndTimeInMS = RUNTIME_IN_MS();


}

void ShaderLighting::use(vec3f* position, vec4f* color)
{
	(void)position; // prevent warning
	(void)color; // prevent warning

    program.bind();
	glUniformMatrix4fv(m_uniformProjectionMatrix, 1, GL_FALSE, m_projectionMatrix->f);

    IlmMatrix translation;
	IlmMatrixTranslation(translation, position->x, position->y, position->z);

    program.bind();
	glUniformMatrix4fv(m_uniformModelMatrix, 1, GL_FALSE, translation.f);
    glUniform4fv(m_uniformColor, 1, &color->r);
}


void ShaderLighting::timerEvent(QTimerEvent*e)
{
    update();
}

unsigned int ShaderLighting::getTickCount()
{
    struct timeval ts;
    gettimeofday(&ts, 0);
    return (unsigned int) (ts.tv_sec * 1000 + (ts.tv_usec / 1000));
}

void ShaderLighting::updateObject(int currentTimeInMs, int lastFrameTime)
{
	m_camera.update(currentTimeInMs, lastFrameTime);

	list<IUpdateable*>::const_iterator iter = m_updateList.begin();
    list<IUpdateable*>::const_iterator iterEnd = m_updateList.end();

    for (; iter != iterEnd; ++iter)
    {
        (*iter)->update(currentTimeInMs, lastFrameTime);
    }
    update();
}

void ShaderLighting::render()
{
    list<IRenderable*>::const_iterator iter = m_renderList.begin();
    list<IRenderable*>::const_iterator iterEnd = m_renderList.end();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (; iter != iterEnd; ++iter)
    {
        (*iter)->render();
    }
}

void ShaderLighting::generateCity()
{

    qDebug() << "generateCity";
	m_projectionMatrix = m_camera.getViewProjectionMatrix();
    qDebug() << "generateCity";

    // generate base plate
	vec4f groundColor(0.8, 0.8, 0.6, 1.0);
    vec3f position = vec3f(0.0, -0.001, 0.0);
    vec3f size = vec3f(CITY_GRID_SIZE * 3, 0.0, -CITY_GRID_SIZE * 2.0 * m_houseCount);
    Ground* ground = new Ground(position, size, groundColor, this);
    m_renderList.push_back(ground);

    // generate street z direction
    vec4f streetColor(0.0, 0.0, 0.0, 1.0);
    vec3f streetPosition = vec3f(0.6 * CITY_GRID_SIZE, 0.0, 0.0);
    vec3f streetSize = vec3f(CITY_GRID_SIZE * 0.6, 0.0, -CITY_GRID_SIZE * 2.0 * m_houseCount);
    Street* obj = new Street(streetPosition, streetSize, streetColor, this);
    m_renderList.push_back(obj);

    // generate streets x direction
    for (int z = 1; z < m_houseCount; ++z)
    {
        vec4f streetColor(0.0, 0.0, 0.0, 1.0);
        vec3f streetPosition = vec3f(0.0, 0.0, 0.6 - z * CITY_GRID_SIZE);
        vec3f streetSize = vec3f(CITY_GRID_SIZE * 3, 0.0, CITY_GRID_SIZE * 0.6);
        Street* obj = new Street(streetPosition, streetSize, streetColor, this);
        m_renderList.push_back(obj);
        m_updateList.push_back(obj);
    }

    // generate car
    vec3f carPosition(1.4 * CITY_GRID_SIZE, 0.001, -0.3);
    vec3f carSize(0.2f, 0.2f, 0.3f);
    vec4f carColor(0.7, 0.3, 0.3, 1.0);
    Car* car = new Car(carPosition, carSize, carColor, this);
    m_renderList.push_back(car);

    // generate houses
	vec4f houseColor(0.6, 0.6, 0.8, 1.0);
    for (int x = 0; x < 2; ++x)
    {
        for (int z = 0; z < m_houseCount; ++z)
        {
            float posx = x * 2.0 * CITY_GRID_SIZE;
            float posy = 0.0;
            float posz = -z * 2.0 * CITY_GRID_SIZE;

            vec3f housePosition(posx, posy, posz);
            vec3f houseSize(1.0, 1.0, 1.0);

            House* obj = new House(housePosition, houseSize, houseColor, this);

            m_renderList.push_back(obj);
            m_updateList.push_back(obj);
        }
    }
    this->show();
    timer.start(100,this);
}
