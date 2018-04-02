#include "BulletSprite.h"


BulletSprite::BulletSprite()
{
}


BulletSprite::~BulletSprite()
{
}

BulletSprite * BulletSprite::create(const std::string & filename){
	BulletSprite * mySprite = new BulletSprite();
	if (mySprite && mySprite->initWithFile(filename)){
		mySprite->autorelease();
		return mySprite;
	}
	CC_SAFE_DELETE(mySprite);
	return nullptr;
}