#include "HuajiSprite.h"


HuajiSprite::HuajiSprite()
{
}


HuajiSprite::~HuajiSprite()
{
}

HuajiSprite * HuajiSprite::create(const std::string & filename){
	HuajiSprite * mySprite = new HuajiSprite();
	if (mySprite && mySprite->initWithFile(filename)){
		mySprite->autorelease();
		return mySprite;
	}
	CC_SAFE_DELETE(mySprite);
	return nullptr;
}