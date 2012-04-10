#include "cube.h"
#include <QDebug>
#include <QMouseEvent>
#include <QFile>
#include <QGLShader>
#include <QGLShaderProgram>

Cube::Cube(const QVector3D &center,
           const QString &textureName,
           qreal size, QObject *parent)
    : Box(center, QVector3D(size, size, size), textureName, parent)
{
}

Cube::~Cube()
{
}
