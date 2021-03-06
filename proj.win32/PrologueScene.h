#ifndef __PROLOGUE_SCENE_H__
#define __PROLOGUE_SCENE_H__

#include "cocos2d.h"

class PrologueScene : public cocos2d::Scene
{
public:

	void restartGameCallBack(Ref* pSender);

	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);


	void goToStage1SceneCallback(Ref * pSender);
	// implement the "static create()" method manually
	CREATE_FUNC(PrologueScene);
};

#endif // __PROLOGUE_SCENE_H__
