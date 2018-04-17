#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__

#include "cocos2d.h"
#include "Stage1Scene.h"
#include "Stage2Scene.h"
#include "Stage3Scene.h"
#include "Stage4Scene.h"
#include "Stage5Scene.h"

class GameOverScene : public cocos2d::Scene
{
public:

	void restartGameCallBack(Ref* pSender);

	static cocos2d::Scene* createScene();

	int lastStage = 1;

	virtual bool init();


	void gotoLastStage(Ref* pSender);

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(GameOverScene);
};

#endif // __GAMEOVER_SCENE_H__
