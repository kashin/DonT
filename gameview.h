#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGLWidget>
#include <QVector2D>
#include <QVector3D>

class QGLShaderProgram;
class QGLShader;
class Cube;

class GameView : public QGLWidget
{
    Q_OBJECT
public:
    GameView(QWidget *parent = 0, qreal size = 1);
    void setCubePicture(const QString& name);

signals:
    void closeApplication();

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void rotate(int xAngle, int yAngle, int zAngle);

private:
    void makeGeometry();

private:
    QPixmap* mTexturePicture;
    QPixmap* mFieldPicture;
    QGLShaderProgram* mProgram;
    QGLShader* mVertexShader;
    QGLShader* mFragmentShader;
    int mXRotation, mYRotation, mZRotation;
    QVector<QVector3D> mCubeVertices;
    QVector<QVector2D> mCubeCoords;
    QVector<QVector3D> mFieldVertices;
    QVector<QVector2D> mFieldCoords;
    QVector<QVector3D> mPirVertices;
    QVector<QVector2D> mPirCoords;
    GLuint mCubeTexture;
    GLuint mFieldTexture;
    GLuint mPirTexture;
    QPoint mLastPosition;
    qreal mSize;
};

#endif // GAMEVIEW_H
