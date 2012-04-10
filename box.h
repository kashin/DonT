#ifndef BOX_H
#define BOX_H

#include <drawelement.h>

class QGLShaderProgram;

class Box : public DrawElement
{
public:
    Box(const QVector3D& center,
        const QVector3D& sizes,
        const QString& textureName,
        QObject *parent = 0);
    ~Box();

    QVector3D sidesSizes() const { return mSidesSize; }

    int xrotation() const { return mXRotation; }
    int yrotation() const { return mYRotation; }
    int zrotation() const { return mZRotation; }

    virtual void setRotation(const int& xrotation, const int& yrotation, const int& zrotation);
    virtual void moveToVector(const QVector3D& vector);
    virtual void draw(QGLShaderProgram* program);
    virtual QPair<QVector3D, QVector3D> fireConditions() const;

protected:
    void init();

private:
    void makeGeometry();

private:
    QPixmap* mTexturePicture;
    GLuint mTexture;
    QString mTextureName;
    QVector3D mCenter;
    QVector3D mSidesSize;
    int mXRotation, mYRotation, mZRotation;
    QVector<QVector3D> mVertices;
    QVector<QVector2D> mCoords;
};

#endif // BOX_H
