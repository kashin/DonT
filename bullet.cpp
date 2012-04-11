#include "bullet.h"
#include "box.h"

#include <QDebug>

Bullet::Bullet(const QVector3D &center,
               const BulletCaliber& caliber,
               const QVector3D &initialVelocity,
               QObject *parent)
    : QObject(parent),
      mBulletStoped(-500),
      mBox(0),
      mVelocityTimer(0),
      mCaliber(caliber),
      mVelocity(initialVelocity)
{
    initBulletsSizesMap();

    if (!mBulletSizes.contains(mCaliber))
    {
        qFatal("Dumpass, Add new Caliber in the sizes Hash!!!!");
        return;
    }

    mBox = new Box(center, mBulletSizes.value(mCaliber),
                   QString("./9mm_silver_bullet.jpg"), this);
    mVelocityTimer = new QTimer(this);
    mVelocityTimer->setSingleShot(false);
    mVelocityTimer->setInterval(100);
    connect(mVelocityTimer, SIGNAL(timeout()), this, SLOT(onVelocityTimer()));
    mVelocityTimer->start();
}

Bullet::~Bullet()
{
    mVelocityTimer->stop();
    delete mVelocityTimer;
}

void Bullet::initBulletsSizesMap()
{
    mBulletSizes.insert(Bullet9mm, QVector3D(0.0009f, 0.0002f, 0.0002f));
    mBulletSizes.insert(Bullet105mm, QVector3D(0.0105f, 0.001f, 0.001f));
    mBulletSizes.insert(Rocket, QVector3D(0.1f, 0.01f, 0.01f));
}

void Bullet::draw(QGLShaderProgram *program)
{
    if(mBox)
        mBox->draw(program);
}

void Bullet::setRotation(const int &xrotation, const int &yrotation, const int &zrotation)
{
    if (mBox)
        mBox->setRotation(xrotation, yrotation, zrotation);
}

void Bullet::moveToVector(const QVector3D &vector)
{
    if (mBox)
        mBox->moveToVector(vector);
}

GLuint Bullet::texture() const
{
    if (mBox)
        return mBox->texture();
    return 0;
}

void Bullet::setTexture(const GLuint &texture)
{
    if (mBox)
        mBox->setTexture(texture);
}

const QPixmap* Bullet::texturePicture() const
{
    if (mBox)
        return mBox->texturePicture();
    return 0;
}

void Bullet::onVelocityTimer()
{
    if (mBulletStoped >= 0)
    {
        emit bulletStoped(this);
        return;
    }
    ++mBulletStoped;
    moveToVector(mVelocity);
    emit bulletMoved();
}
