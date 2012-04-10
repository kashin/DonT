#ifndef CUBE_H
#define CUBE_H

#include <box.h>

class QGLShaderProgram;

class Cube : public Box
{
    Q_OBJECT
public:
    Cube(const QVector3D& center,
         const QString& textureName,
         qreal size = 0.5, QObject *parent = 0);
    ~Cube();
    qreal size() const { return sidesSizes().x(); }
};

#endif // CUBE_H
