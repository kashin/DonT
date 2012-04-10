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

    for (int i = 0; i < mVertices.count(); ++i) {
        glBindTexture(GL_TEXTURE_2D, texture());
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }
}

void Box::makeGeometry()
{
    static const qreal coords[6][4][3] = {
        { { mCenter.x()+mSidesSize.x(), mCenter.y()-mSidesSize.y(), mCenter.z()-mSidesSize.z() }, { mCenter.x()-mSidesSize.x(), mCenter.y()-mSidesSize.y(), mCenter.z()-mSidesSize.z() }, { mCenter.x()-mSidesSize.x(), mCenter.y()+mSidesSize.y(), mCenter.z()-mSidesSize.z() }, { mCenter.x()+mSidesSize.x(), mCenter.y()+mSidesSize.y(), mCenter.z()-mSidesSize.z() } },
        { { mCenter.x()+mSidesSize.x(), mCenter.y()+mSidesSize.y(), mCenter.z()-mSidesSize.z() }, { mCenter.x()-mSidesSize.x(), mCenter.y()+mSidesSize.y(), mCenter.z()-mSidesSize.z() }, { mCenter.x()-mSidesSize.x(), mCenter.y()+mSidesSize.y(), mCenter.z()+mSidesSize.z() }, { mCenter.x()+mSidesSize.x(), mCenter.y()+mSidesSize.y(), mCenter.z()+mSidesSize.z() } },
        { { mCenter.x()+mSidesSize.x(), mCenter.y()-mSidesSize.y(), mCenter.z()+mSidesSize.z() }, { mCenter.x()+mSidesSize.x(), mCenter.y()-mSidesSize.y(), mCenter.z()-mSidesSize.z() }, { mCenter.x()+mSidesSize.x(), mCenter.y()+mSidesSize.y(), mCenter.z()-mSidesSize.z() }, { mCenter.x()+mSidesSize.x(), mCenter.y()+mSidesSize.y(), mCenter.z()+mSidesSize.z() } },
        { { mCenter.x()-mSidesSize.x(), mCenter.y()-mSidesSize.y(), mCenter.z()-mSidesSize.z() }, { mCenter.x()-mSidesSize.x(), mCenter.y()-mSidesSize.y(), mCenter.z()+mSidesSize.z() }, { mCenter.x()-mSidesSize.x(), mCenter.y()+mSidesSize.y(), mCenter.z()+mSidesSize.z() }, { mCenter.x()-mSidesSize.x(), mCenter.y()+mSidesSize.y(), mCenter.z()-mSidesSize.z() } },
        { { mCenter.x()+mSidesSize.x(), mCenter.y()-mSidesSize.y(), mCenter.z()+mSidesSize.z() }, { mCenter.x()-mSidesSize.x(), mCenter.y()-mSidesSize.y(), mCenter.z()+mSidesSize.z() }, { mCenter.x()-mSidesSize.x(), mCenter.y()-mSidesSize.y(), mCenter.z()-mSidesSize.z() }, { mCenter.x()+mSidesSize.x(), mCenter.y()-mSidesSize.y(), mCenter.z()-mSidesSize.z() } },
        { { mCenter.x()-mSidesSize.x(), mCenter.y()-mSidesSize.y(), mCenter.z()+mSidesSize.z() }, { mCenter.x()+mSidesSize.x(), mCenter.y()-mSidesSize.y(), mCenter.z()+mSidesSize.z() }, { mCenter.x()+mSidesSize.x(), mCenter.y()+mSidesSize.y(), mCenter.z()+mSidesSize.z() }, { mCenter.x()-mSidesSize.x(), mCenter.y()+mSidesSize.y(), mCenter.z()+mSidesSize.z() } }
    };

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            mCoords.append(QVector2D(j == 0 || j == 3, j == 0 || j == 1));
            mVertices.append(QVector3D(0.3 * coords[i][j][0], 0.3 * coords[i][j][1],
                           0.3 * coords[i][j][2]));
        }
    }
}

QPair<QVector3D, QVector3D> Box::fireConditions() const
{
    return qMakePair(mCenter+(mSidesSize/2), QVector3D(0.005, 0.0, 0.0));
}

