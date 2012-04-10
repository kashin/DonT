#ifndef BULLET_H
#define BULLET_H

#include <qgl.h>
#include <QObject>
#include <QHash>
#include <QVector3D>
#include <QTimer>

class Box;
class QGLShaderProgram;

class Bullet : public QObject
{

    Q_OBJECT
public:
    enum BulletCaliber {
        Bullet9mm = 0,
        Bullet105mm,
        Rocket // well, it is a bullet... :-D
    };

    explicit Bullet(const QVector3D& center,
           const BulletCaliber& caliber,
           const QVector3D& initialVelocity,
           QObject *parent = 0);
    ~Bullet();

    GLuint texture() const;
    void setTexture(const GLuint& texture);
    const QPixmap* texturePicture() const;

    void draw(QGLShaderProgram* program);
    void setRotation(const int& xrotation, const int& yrotation, const int& zrotation);
    void moveToVector(const QVector3D& vector);

private slots:
    void onVelocityTimer();

signals:
    void bulletMoved();

private:
    void initBulletsSizesMap();

private:
    int mBulletStoped;
    QHash<BulletCaliber, QVector3D> mBulletSizes;
    Box* mBox;
    QTimer* mVelocityTimer;
    BulletCaliber mCaliber;
    QVector3D mVelocity;
};

#endif // BULLET_H
