#include "cube.h"
#include <QDebug>
#include <QGLShader>
#include <QGLShaderProgram>

Cube::Cube(QWidget *parent)
    : QGLWidget(parent)
{
    mProgram = new QGLShaderProgram(this);
    mVertexShader = new QGLShader(QGLShader::Vertex, this);
    mFragmentShader = new QGLShader(QGLShader::Fragment, this);
}

void Cube::initializeGL()
{
    makeCurrent();
    glClearColor(0.4f, 0.15f, 0.2f, 0.0f);

    if (!mVertexShader->compileSourceFile("./vertexCode.vsh"))
        qDebug() << Q_FUNC_INFO << "vertex code is not compiled" << mVertexShader->log();
    else
        mProgram->addShader(mVertexShader);

    if (!mFragmentShader->compileSourceFile("./fragmentCode.fsh"))
        qDebug() << Q_FUNC_INFO << "fragment code is not compiled" << mFragmentShader->log();
    else
        mProgram->addShader(mFragmentShader);

    if(!mProgram->link())
        {
            qWarning() << "Shader Program Linker Error" << mProgram->log();
        }
    else
        mProgram->bind();
}

void Cube::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);
}

void Cube::paintGL()
{
//    qglClearColor(Qt::darkBlue);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);            // move 5 units into the screen
    glRotatef(1,0.0f,0.0f,0.5f);     // rotate z-axis
    glBegin(GL_QUADS);
        glColor3f(1.,0.,0.);
        glVertex3f(-1.0, -1.0,0.0);
        glVertex3f(1.0, -1.0,0.0);
        glVertex3f(1.0, 1.0,0.0);
        glVertex3f(-1.0, 1.0,0.0);
    glEnd();
}
