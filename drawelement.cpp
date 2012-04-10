#include "drawelement.h"

DrawElement::DrawElement(const QString &textureName, QObject *parent)
    : QObject(parent),
      mTextureName(textureName),
      mTexturePicture(0),
      mTexture(0)
{
    mTexturePicture = new QPixmap(mTextureName);
}

DrawElement::~DrawElement()
{
    delete mTexturePicture;
}

void DrawElement::setTexturePicture(const QString &name)
{
    if (mTexturePicture)
        delete mTexturePicture;
    mTextureName = name;
    mTexturePicture = new QPixmap(mTextureName);
}
