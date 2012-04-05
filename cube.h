#ifndef CUBE_H
#define CUBE_H

#include <QGLWidget>

class QGLShaderProgram;
class QGLShader;

class Cube : public QGLWidget
{
    Q_OBJECT
public:
    Cube(QWidget *parent = 0);

protected:
    void initializeGL();

    void resizeGL(int w, int h);

    void paintGL();

private:
    QGLShaderProgram* mProgram;
    QGLShader* mVertexShader;
    QGLShader* mFragmentShader;
};

#endif // CUBE_H
