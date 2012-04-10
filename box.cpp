#include "box.h"
#include <QDebug>
#include <QMouseEvent>
#include <QFile>
#include <QGLShader>
#include <QGLShaderProgram>

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

Box::Box(const QVector3D &center,
         const QVector3D &sizes,
         const QString &textureName,
         QObject *parent)
    : DrawElement(textureName, parent),
      mCenter(center),
      mSidesSize(sizes),
      mXRotation(0),
      mYRotation(0),
      mZRotation(0)
{
    init();
}

Box::~Box()
{
}

void Box::init()
{
    makeGeometry();
}

void Box::setRotation(const int &xrotation, const int &yrotation, const int &zrotation)
{
    mXRotation = xrotation;
    mYRotation = yrotation;
    mZRotation = zrotation;
}

void Box::moveToVector(const QVector3D &vector)
{
    for (QVector<QVector3D>::iterator it = mVertices.begin(); it != mVertices.end(); ++it)
    {
        (*it) += vector;
    }
    mCenter += vector;
}

void Box::draw(QGLShaderProgram *program)
{
    if (!program)
    {
        qCritical() << "Shader program is NULL!!!";
        return;
    }
    program->setAttributeArray(PROGRAM_VERTEX_ATTRIBUTE, mVertices.constData());
    program->setAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE, mCoords.constData());

    glBindTexture(GL_TEXTURE_2D, texture());
    for (int i = 0; i < mVertices.count(); i += 4) {
        glDrawArrays(GL_TRIANGLE_FAN, i, 4);
    }
}

void Box::makeGeometry()
{

    static qreal cubeArray[][3] = {
         {0, 0, 0}, {0, 0.1, 0}, {0.1, 0.1, 0}, {0.1, 0, 0},
         {0, 0, 0.1}, {0.1, 0, 0.1}, {0.1, 0.1, 0.1}, {0, 0.1, 0.1},
         {0, 0, 0}, {0.1, 0, 0}, {0.1, 0, 0.1}, {0, 0, 0.1},
         {0, 0.1, 0}, {0, 0.1, 0.1}, {0.1, 0.1, 0.1}, {0.1, 0.1, 0},
         {0, 0.1, 0}, {0, 0, 0}, {0, 0, 0.1}, {0, 0.1, 0.1},
         {0.1, 0, 0}, {0.1, 0.1, 0}, {0.1, 0.1, 0.1}, {0.1, 0, 0.1}
     };

     static qreal cubeTextureArray[][2] = {
         {0, 0}, {1, 0}, {1, 1}, {0, 1},
         {0, 0}, {0, 1}, {1, 1}, {1, 0},
         {0, 0}, {1, 0}, {1, 1}, {0, 1},
         {1, 0}, {0, 0}, {0, 1}, {1, 1},
         {0, 0}, {1, 0}, {1, 1}, {0, 1},
         {1, 0}, {0, 0}, {0, 1}, {1, 1}
     };

     for (int i=0; i < 6*4; ++i) {
         mCoords.append(QVector2D(cubeTextureArray[i][0], cubeTextureArray[i][1]));
         mVertices.append(QVector3D(mCenter.x() + cubeArray[i][0],
                                    mCenter.y() + cubeArray[i][1],
                                    mCenter.z() + cubeArray[i][2]));
     }
}

QPair<QVector3D, QVector3D> Box::fireConditions() const
{
    return qMakePair(mCenter+(mSidesSize/2), QVector3D(0.005, 0.0, 0.0));
}

