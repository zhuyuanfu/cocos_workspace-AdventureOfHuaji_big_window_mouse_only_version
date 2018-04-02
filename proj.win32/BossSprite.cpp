#include "BossSprite.h"


BossSprite::BossSprite()
{
}


BossSprite::~BossSprite()
{
}

BossSprite * BossSprite::create(const std::string & filename){
	BossSprite * mySprite = new BossSprite();
	if (mySprite && mySprite->initWithFile(filename)){
		mySprite->autorelease();
		return mySprite;
	}
	CC_SAFE_DELETE(mySprite);
	return nullptr;
}