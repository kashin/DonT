#include "gameview.h"
#include "cube.h"
#include <QDebug>
#include <QMouseEvent>
#include <QFile>
#include <QGLShader>
#include <QGLShaderProgram>

GameView::GameView(QWidget *parent, qreal size)
    : QGLWidget(parent),
      mSize(size)
{
    mTexturePicture = new QPixmap(QString("./Porsche_Wallpapers_7.jpg"));
    mFieldPicture = new QPixmap(QString("./DonT.png"));
    mProgram = new QGLShaderProgram(this);
    mVertexShader = new QGLShader(QGLShader::Vertex, this);
    mFragmentShader = new QGLShader(QGLShader::Fragment, this);
}

void GameView::initializeGL()
{
    grabKeyboard();
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
    mProgram  = new QGLShaderProgram(this);
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

void GameView::resizeGL(int width, int height)
{
    glViewport(0 , 0, width, height);

#if !defined(QT_OPENGL_ES_2)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifndef QT_OPENGL_ES
    glOrtho(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0);
#else
    glOrthof(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
#endif
}

void GameView::paintGL()
{
    qglClearColor(Qt::darkCyan);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 m;
    m.ortho(-0.5f, +0.5f, +0.5f, -0.5f, 4.0f, 15.0f);
    m.translate(0.0f, 0.0f, -10.0f);
    m.rotate(mXRotation / 16.0f, 1.0f, 0.0f, 0.0f);
    m.rotate(mYRotation / 16.0f, 0.0f, 1.0f, 0.0f);
    m.rotate(mZRotation / 16.0f, 0.0f, 0.0f, 1.0f);

    mProgram->setUniformValue("matrix", m);
    mProgram->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    mProgram->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    mProgram->setAttributeArray(PROGRAM_VERTEX_ATTRIBUTE, mCubeVertices.constData());
    mProgram->setAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE, mCubeCoords.constData());

    for (int i = 0; i < 6; ++i) {
        glBindTexture(GL_TEXTURE_2D, mCubeTexture);
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }

    mProgram->setAttributeArray(PROGRAM_VERTEX_ATTRIBUTE, mFieldVertices.constData());
    mProgram->setAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE, mFieldCoords.constData());

    for (int i = 0; i < 6; ++i) {
        glBindTexture(GL_TEXTURE_2D, mFieldTexture);
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }

    mProgram->setAttributeArray(PROGRAM_VERTEX_ATTRIBUTE, mPirVertices.constData());
    mProgram->setAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE, mPirCoords.constData());

    for (int i = 0; i < 6; ++i) {
        glBindTexture(GL_TEXTURE_2D, mPirTexture);
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }
}

void GameView::makeGeometry()
{
    static const qreal cubeCoords[6][4][3] = {
//        { { +mSize, -mSize, -mSize }, { -mSize, -mSize, -mSize }, { -mSize, +mSize, -mSize }, { +mSize, +mSize, -mSize } },
//        { { +mSize, +mSize, -mSize }, { -mSize, +mSize, -mSize }, { -mSize, +mSize, +mSize }, { +mSize, +mSize, +mSize } },
//        { { +mSize, -mSize, +mSize }, { +mSize, -mSize, -mSize }, { +mSize, +mSize, -mSize }, { +mSize, +mSize, +mSize } },
//        { { -mSize, -mSize, -mSize }, { -mSize, -mSize, +mSize }, { -mSize, +mSize, +mSize }, { -mSize, +mSize, -mSize } },
//        { { +mSize, -mSize, +mSize }, { -mSize, -mSize, +mSize }, { -mSize, -mSize, -mSize }, { +mSize, -mSize, -mSize } },
//        { { -mSize, -mSize, +mSize }, { +mSize, -mSize, +mSize }, { +mSize, +mSize, +mSize }, { -mSize, +mSize, +mSize } }

        { { 1,0,0 }, { 0,0,0 }, { 0,1,0 }, { 1,1,0 } },
        { { 1,1,0 }, { 0,1,0 }, { 0,1,1 }, { 1,1,1 } },
        { { 1,0,1 }, { 1,0,0 }, { 1,1,0 }, { 1,1,1 } },
        { { 0,0,0 }, { 0,0,1 }, { 0,1,1 }, { 0,1,0 } },
        { { 1,0,1 }, { 0,0,1 }, { 0,0,0 }, { 1,0,0 } },
        { { 0,0,1 }, { 1,0,1 }, { 1,1,1 }, { 0,1,1 } }
    };

    mCubeTexture = bindTexture(*mTexturePicture, GL_TEXTURE_2D);

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            mCubeCoords.append(QVector2D((j == 0 || j == 3) ? 1 : -2 , (j == 0 || j == 1) ? 1 : -1));
            mCubeVertices.append(QVector3D(0.4 * cubeCoords[i][j][0], 0.4 * cubeCoords[i][j][1],
                           0.4 * cubeCoords[i][j][2]));
        }
    }

    static const qreal fieldCoords[6][4][3] = {
        { { 1,0,-2 }, { 0,0,-2 }, { 0,1,-2 }, { 1,1,-2 } },
        { { 1,1,-2 }, { 0,1,-2 }, { 0,1,-1}, { 1,1,-1} },
        { { 1,0,-1}, { 1,0,-2 }, { 1,1,-2 }, { 1,1,-1} },
        { { 0,0,-2 }, { 0,0,-1}, { 0,1,-1}, { 0,1,-2 } },
        { { 1,0,-1}, { 0,0,-1}, { 0,0,-2 }, { 1,0,-2 } },
        { { 0,0,-1}, { 1,0,-1}, { 1,1,-1}, { 0,1,-1} }
    };

    mFieldTexture = bindTexture(*mFieldPicture, GL_TEXTURE_2D);

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            mFieldCoords.append(QVector2D(j == 0 || j == 3, j == 0 || j == 1));
            mFieldVertices.append(QVector3D(0.3 * fieldCoords[i][j][0], 0.3 * fieldCoords[i][j][1],
                           0.3 * fieldCoords[i][j][2]));
        }
    }

    static const qreal pirCoords[6][4][3] = {
        { { 1,0,-2 }, { 0,0,-2 }, { 0,1,-2 }, { 1,1,-2 } },
        { { 0.5,0.25,-3 }, { 0.5,0.5,-3 }, { 0.25,0.5,-3}, { 0.25,0.25,-3} },
        { { 0,0,-2 }, { 1,0,-2 }, { 0.5,0.25,-3 }, { 0.25,0.25,-3 } },
        { { 1,0,-2 }, { 1,1,-2 }, { 0.5,0.5,-3}, { 0.5,0.25,-3} },
        { { 1,1,-2 }, { 0,1,-2 }, { 0.25,0.5,-3}, { 0.5,0.5,-3} },
        { { 0,1,-2 }, { 0,0,-2 }, { 0.25,0.25,-3}, { 0.25,0.5,-3} }
    };

    mPirTexture = bindTexture(*mTexturePicture, GL_TEXTURE_2D);

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            mPirCoords.append(QVector2D(j == 0 || j == 3, j == 0 || j == 1));
            mPirVertices.append(QVector3D(0.3 * pirCoords[i][j][0], 0.3 * pirCoords[i][j][1],
                           0.3 * pirCoords[i][j][2]));
        }
    }
}

void GameView::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - mLastPosition.x();
    int dy = event->y() - mLastPosition.y();

    if (event->buttons() & Qt::LeftButton) {
        rotate(10 * dy, -10 * dx, 0);
    } else if (event->buttons() & Qt::RightButton) {
        rotate(10 * dy, 0, -10 * dx);
    }
    mLastPosition = event->pos();
}

void GameView::mousePressEvent(QMouseEvent *event)
{
    mLastPosition = event->pos();
}

void GameView::rotate(int xAngle, int yAngle, int zAngle)
{
    mXRotation += xAngle;
    mYRotation += yAngle;
    mZRotation += zAngle;
    updateGL();
}

void GameView::setCubePicture(const QString &name)
{
    if (mTexturePicture)
        delete mTexturePicture;
    mTexturePicture = new QPixmap(name);
}

void GameView::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {

    case Qt::Key_Down:
        for (QVector<QVector3D>::iterator it = mCubeVertices.begin(); it != mCubeVertices.end(); ++it)
        {
            (*it) += QVector3D(0,0,-0.01);
        }
        updateGL();
        break;
    case Qt::Key_Up:
        for (QVector<QVector3D>::iterator it = mCubeVertices.begin(); it != mCubeVertices.end(); ++it)
        {
            (*it) += QVector3D(0,0,0.01);
        }
        updateGL();
        break;
    case Qt::Key_Left:
        for (QVector<QVector3D>::iterator it = mCubeVertices.begin(); it != mCubeVertices.end(); ++it)
        {
            (*it) += QVector3D(-0.01,0,0);
        }
        updateGL();
        break;
    case Qt::Key_Right:
        for (QVector<QVector3D>::iterator it = mCubeVertices.begin(); it != mCubeVertices.end(); ++it)
        {
            (*it) += QVector3D(0.01,0,0);
        }
        updateGL();
        break;
    default:
        if (event->key() == Qt::Key_Escape)
           {
               releaseKeyboard();
               emit closeApplication();
               return;
           }
    }
}
