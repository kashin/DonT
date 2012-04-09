#include "cube.h"
#include <QDebug>
#include <QMouseEvent>
#include <QFile>
#include <QGLShader>
#include <QGLShaderProgram>

Cube::Cube(QGLShaderProgram *program, uint size, QObject *parent)
    : QObject(parent),
      mSize(size)
{
    mTexturePicture = new QPixmap(QString("./Porsche_Wallpapers_7.jpg"));
    mProgram = program;
    mVertexShader = new QGLShader(QGLShader::Vertex, this);
    mFragmentShader = new QGLShader(QGLShader::Fragment, this);
}

Cube::~Cube()
{
    delete mTexturePicture;
}

void Cube::init()
{
    makeGeometry();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
#ifndef QT_OPENGL_ES_2
    glEnable(GL_TEXTURE_2D);
#endif
#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

    mVertexShader = new QGLShader(QGLShader::Vertex, this);
    if (!mVertexShader->compileSourceFile("./vertexCode.vsh"))
    {
        qDebug() << Q_FUNC_INFO << "Can't compile vertex shader code: " << mVertexShader->log();
    }
    mFragmentShader = new QGLShader(QGLShader::Fragment, this);
    if (!mFragmentShader->compileSourceFile("./fragmentCode.fsh"))
    {
        qDebug() << Q_FUNC_INFO << "Can't compile fragment shader code: " << mVertexShader->log();
    }
    mProgram->addShader(mVertexShader);
    mProgram->addShader(mFragmentShader);
    mProgram->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    mProgram->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    if (!mProgram->link())
    {
        qWarning() << "Shader Program Linker Error" << mProgram->log();
    }
    else
    {
        mProgram->bind();
    }
    mProgram->setUniformValue("texture", 0);
}

void Cube::draw()
{
    QMatrix4x4 m;
    m.ortho(-0.5f, +0.5f, +0.5f, -0.5f, 4.0f, 15.0f);
    m.translate(0.0f, 0.0f, -10.0f);
    mProgram->setUniformValue("matrix", m);
    mProgram->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    mProgram->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    mProgram->setAttributeArray(PROGRAM_VERTEX_ATTRIBUTE, mVertices.constData());
    mProgram->setAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE, mCoords.constData());

    for (int i = 0; i < 6; ++i) {
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }
}

void Cube::makeGeometry()
{
    static const int coords[6][4][3] = {
        { { +mSize, -mSize, -mSize }, { -mSize, -mSize, -mSize }, { -mSize, +mSize, -mSize }, { +mSize, +mSize, -mSize } },
        { { +mSize, +mSize, -mSize }, { -mSize, +mSize, -mSize }, { -mSize, +mSize, +mSize }, { +mSize, +mSize, +mSize } },
        { { +mSize, -mSize, +mSize }, { +mSize, -mSize, -mSize }, { +mSize, +mSize, -mSize }, { +mSize, +mSize, +mSize } },
        { { -mSize, -mSize, -mSize }, { -mSize, -mSize, +mSize }, { -mSize, +mSize, +mSize }, { -mSize, +mSize, -mSize } },
        { { +mSize, -mSize, +mSize }, { -mSize, -mSize, +mSize }, { -mSize, -mSize, -mSize }, { +mSize, -mSize, -mSize } },
        { { -mSize, -mSize, +mSize }, { +mSize, -mSize, +mSize }, { +mSize, +mSize, +mSize }, { -mSize, +mSize, +mSize } }
    };

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            mCoords.append(QVector2D(j == 0 || j == 3, j == 0 || j == 1));
            mVertices.append(QVector3D(0.3 * coords[i][j][0], 0.3 * coords[i][j][1],
                           0.3 * coords[i][j][2]));
        }
    }
}

void Cube::setPicture(const QString &name)
{
    if (mTexturePicture)
        delete mTexturePicture;
    mTexturePicture = new QPixmap(name);
}
