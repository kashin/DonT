#ifndef CUBE_H
#define CUBE_H

#include <QObject>
#include <QVector>
#include <QVector2D>
#include <QVector3D>

class QGLShaderProgram;
class QGLShader;

class Cube : public QObject
{
    Q_OBJECT
public:
    Cube(QGLShaderProgram* program, uint size = 1, QObject *parent = 0);
    ~Cube();
    void setPicture(const QString& name);
    void draw();

protected:
    void init();

private:
    void makeGeometry();

private:
    QPixmap* mTexturePicture;
    QGLShaderProgram* mProgram;
    QGLShader* mVertexShader;
    QGLShader* mFragmentShader;
    QVector<QVector3D> mVertices;
    QVector<QVector2D> mCoords;
    uint mSize;
};

#endif // CUBE_H
