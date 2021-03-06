#ifndef __COVER_SCENE_H__
#define __COVER_SCENE_H__

#include "cocos2d.h"
 
class CoverScene : public cocos2d::Scene
{
public:

	void restartGameCallBack(Ref* pSender);

	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);


	void goToPrologueSceneCallback(Ref * pSender);
	// implement the "static create()" method manually
	CREATE_FUNC(CoverScene);
};

#endif // __COVER_SCENE_H__
