#include "cube.h"
#include <QGLShader>
#include <QGLShaderProgram>

Cube::Cube(QWidget *parent)
    : QGLWidget(parent)
{
    mProgram = new QGLShaderProgram(this);
    mVertexShader = new QGLShader(QGLShader::Vertex, this);
    mVertexShader = new QGLShader(QGLShader::Fragment, this);
}

void Cube::initializeGL()
{
    glClearColor(0.25f, 0.25f, 0.2f, 0.0f);
}

void Cube::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);
}

void Cube::paintGL()
{
    qglClearColor(Qt::darkBlue);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
