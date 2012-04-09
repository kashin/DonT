#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGLWidget>
#include <QVector2D>
#include <QVector3D>

class QGLShaderProgram;
class QGLShader;

class GameView : public QGLWidget
{
    Q_OBJECT
public:
    GameView(QWidget *parent = 0, uint size = 1);
    void setPicture(const QString& name);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void rotate(int xAngle, int yAngle, int zAngle);

private:
    void makeGeometry();

private:
    QPixmap* mTexturePicture;
    QGLShaderProgram* mProgram;
    QGLShader* mVertexShader;
    QGLShader* mFragmentShader;
    int mXRotation, mYRotation, mZRotation;
    QVector<QVector3D> mVertices;
    QVector<QVector2D> mCoords;
    GLuint mTexture;
    QPoint mLastPosition;
    uint mSize;
};

#endif // GAMEVIEW_H
