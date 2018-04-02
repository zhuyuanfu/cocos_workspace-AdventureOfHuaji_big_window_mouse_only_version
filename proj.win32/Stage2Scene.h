#ifndef __STAGE2_SCENE_H__
#define __STAGE2_SCENE_H__

#include "cocos2d.h"

class Stage2Scene : public cocos2d::Scene
{
public:
	bool key_D_down = false;
	bool key_A_down = false;

	bool mouse1_down = false;
	bool mouse1_up = true;

	//bool flying = true;
	//bool on_the_ground = false;

	//bool jump_from_ground = false;

	bool bullet_time = false;

	double gravity_acc_by_squareframe = -0.2;//∏Ω“È
	//double current_speed_x_pix_by_frame = 0;
	//double current_speed_y_pix_by_frame = 3;
	//double max_speed_x_pix_by_frame = 2;
	//double max_speed_y_pix_by_frame = 10;

	int leftBoundary = 20;
	int rightBoundary = 465;
	int num_of_grounds = 0;

	void update(float dt);
	void myUpdate(float dt);
	//void onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event);
	//void onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event);
	void updateIfHuajiIsfOnTheGroundAndResetHuajisYPosition();



	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Stage2Scene);
};

#endif // __STAGE2_SCENE_H__