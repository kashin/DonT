#include "gameview.h"

#include "cube.h"
#include "bullet.h"

#include <QDebug>
#include <QMouseEvent>
#include <QFile>
#include <QGLShader>
#include <QGLShaderProgram>

GameView::GameView(QWidget *parent)
    : QGLWidget(parent)
{
    mFieldPicture = new QPixmap(QString(":/images/DonT.png"));
    mProgram = new QGLShaderProgram(this);
    mVertexShader = new QGLShader(QGLShader::Vertex, this);
    mFragmentShader = new QGLShader(QGLShader::Fragment, this);
    Cube* cube = new Cube(QVector3D(0, 0, 0), QString(":/images/DonT.png"), 0.25, this);
    mElements.append(cube);

    Box* box = new Box(QVector3D(-0.1,0.2,-0.3), QVector3D(0.1,0.2,0.4),
                       QString(":/images/Porsche_Wallpapers_7.jpg"), this);
    mElements.append(box);
}

GameView::~GameView()
{
    foreach (DrawElement* element, mElements)
        delete element;
    mElements.clear();
    foreach (Bullet* bullet, mBullets)
        delete bullet;
    mBullets.clear();
}

void GameView::initializeGL()
{
    makeCurrent();
    grabKeyboard();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
#ifndef QT_OPENGL_ES_2
    glEnable(GL_TEXTURE_2D);
#endif
#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

    mVertexShader = new QGLShader(QGLShader::Vertex, this);
    if (!mVertexShader->compileSourceFile(":/shaders/vertexCode.vsh"))
    {
        qDebug() << Q_FUNC_INFO << "Can't compile vertex shader code: " << mVertexShader->log();
    }
    mFragmentShader = new QGLShader(QGLShader::Fragment, this);
    if (!mFragmentShader->compileSourceFile(":/shaders/fragmentCode.fsh"))
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

    // init textures
    foreach (DrawElement* element, mElements)
        element->setTexture(bindTexture(*element->texturePicture(), GL_TEXTURE_2D));

    qglClearColor(Qt::darkCyan);
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

    foreach (DrawElement* element, mElements)
    {
        element->draw(mProgram);
    }

    foreach (Bullet* bullet, mBullets)
    {
        bullet->draw(mProgram);
    }
}

#if 0
void GameView::makeGeometry()
{
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

    mPirTexture = bindTexture(*mFieldPicture, GL_TEXTURE_2D);

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            mPirCoords.append(QVector2D(j == 0 || j == 3, j == 0 || j == 1));
            mPirVertices.append(QVector3D(0.3 * pirCoords[i][j][0], 0.3 * pirCoords[i][j][1],
                           0.3 * pirCoords[i][j][2]));
        }
    }
}

#endif

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
    foreach (DrawElement* element, mElements)
        element->setRotation(mXRotation, mYRotation, mZRotation);
    foreach (Bullet* bullet, mBullets)
        bullet->setRotation(mXRotation, mYRotation, mZRotation);
    updateGL();
}

void GameView::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
        foreach (DrawElement* element, mElements)
            element->moveToVector(QVector3D(0,0,0.01));
        break;
    case Qt::Key_Down:
        foreach (DrawElement* element, mElements)
            element->moveToVector(QVector3D(0,0,-0.01));
        break;
    case Qt::Key_Left:
        foreach (DrawElement* element, mElements)
            element->moveToVector(QVector3D(-0.01,0,0));
        break;
    case Qt::Key_Right:
        foreach (DrawElement* element, mElements)
            element->moveToVector(QVector3D(0.01,0,0));
        break;
    case Qt::Key_PageUp:
        foreach (DrawElement* element, mElements)
            element->moveToVector(QVector3D(0,0.01,0));
        break;
    case Qt::Key_PageDown:
        foreach (DrawElement* element, mElements)
            element->moveToVector(QVector3D(0,-0.01,0));
        break;
    case Qt::Key_F:
        if (!mElements.isEmpty())
        {
            // FIXME: yeah, we are firing only from first element atm
            fire(0);
        }
        break;

    default:
        if (event->key() == Qt::Key_Escape)
           {
               releaseKeyboard();
               emit closeApplication();
               return;
           }
    }
    updateGL();
}

void GameView::fire(int elementIndex)
{
    if (elementIndex >= 0 && elementIndex < mElements.count())
    {
        const DrawElement* element = mElements.at(elementIndex);
        QPair<QVector3D, QVector3D> fireConditions = element->fireConditions();
        Bullet* bullet = new Bullet(fireConditions.first, Bullet::Rocket, fireConditions.second, this);
        bullet->setTexture(bindTexture(*bullet->texturePicture(), GL_TEXTURE_2D));
        bullet->setRotation(mXRotation, mYRotation, mZRotation);
        mBullets.append(bullet);
        connect(bullet, SIGNAL(bulletMoved()), this, SLOT(onElementMoved()));
        connect(bullet, SIGNAL(bulletStoped(Bullet*)), this, SLOT(onBulletStoped(Bullet*)));
    }
}

void GameView::onElementMoved()
{
    updateGL();
}

void GameView::onBulletStoped(Bullet *bullet)
{
    if (mBullets.contains(bullet))
    {
        mBullets.removeAll(bullet);
        bullet->deleteLater();
        updateGL();
    }
}
