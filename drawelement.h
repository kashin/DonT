#ifndef DRAWELEMENT_H
#define DRAWELEMENT_H

#include <QObject>
#include <qgl.h>
#include <QVector>
#include <QPair>
#include <QVector2D>
#include <QVector3D>

class QGLShaderProgram;
// AbstractClass for all elements
class DrawElement : public QObject
{
    Q_OBJECT
public:
    explicit DrawElement(const QString& textureName, QObject *parent = 0);
    ~DrawElement();

    GLuint texture() const { return mTexture; }
    void setTexture(const GLuint& texture) { mTexture = texture; }

    const QPixmap* texturePicture() const { return mTexturePicture; }
    void setTexturePicture(const QString& name);

    virtual void setRotation(const int& xrotation, const int& yrotation, const int& zrotation) = 0;
    virtual void moveToVector(const QVector3D& vector) = 0;
    virtual void draw(QGLShaderProgram* program) = 0;    
    virtual QPair<QVector3D, QVector3D> fireConditions() const = 0;

private:
    QString mTextureName;
    QPixmap* mTexturePicture;
    GLuint mTexture;
};

#endif // DRAWELEMENT_H
