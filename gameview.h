#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QList>
#include <QGLWidget>
#include <QVector2D>
#include <QVector3D>

class QGLShaderProgram;
class QGLShader;
class DrawElement;
class Bullet;

class GameView : public QGLWidget
{
    Q_OBJECT
public:
    GameView(QWidget *parent = 0);
    ~GameView();

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
    void fire(int elementIndex);

private slots:
    void onElementMoved();

private:
    QList<DrawElement*> mElements;
    QList<Bullet*> mBullets;
    QPixmap* mFieldPicture;
    QGLShaderProgram* mProgram;
    QGLShader* mVertexShader;
    QGLShader* mFragmentShader;
    int mXRotation, mYRotation, mZRotation;
    QVector<QVector3D> mFieldVertices;
    QVector<QVector2D> mFieldCoords;
    QVector<QVector3D> mPirVertices;
    QVector<QVector2D> mPirCoords;
    GLuint mFieldTexture;
    GLuint mPirTexture;
    QPoint mLastPosition;
};

#endif // GAMEVIEW_H
