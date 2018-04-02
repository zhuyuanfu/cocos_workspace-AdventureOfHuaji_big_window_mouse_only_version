#include "D:\cocos_workspace\AdventureOfHuaji\cocos2d\cocos\2d\CCSprite.h"
//#include "2d\CCSprite.h"
//#include "cocos\2d\CCSprite.h"
//#include "..\cocos2d\cocos\2d\CCSprite.h"
#include "cocos2d.h"



class HeroSprite :public cocos2d::CCSprite
{
public:
	bool flying = false;
	bool on_the_ground = true;
	double gravity_acc_by_squareframe = 9.8;
	double speed_x_pix_by_frame = 0;
	double speed_y_pix_by_frame = 0;

	HeroSprite();
	~HeroSprite();
};

