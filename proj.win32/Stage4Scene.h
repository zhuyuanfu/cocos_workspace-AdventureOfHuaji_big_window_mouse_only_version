#ifndef __STAGE4_SCENE_H__
#define __STAGE4_SCENE_H__

#include "cocos2d.h"

class Stage4Scene : public cocos2d::Scene
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

	int bullet_tag_begin = 200;
	int bullet_tag_end = 200;

	//bool huaji_dead = false;

	double gravity_acc_by_squareframe = -0.2;//����
	//double current_speed_x_pix_by_frame = 0;
	///double current_speed_y_pix_by_frame = 3;
	//double max_speed_x_pix_by_frame = 2;
	//double max_speed_y_pix_by_frame = 10;



	int leftBoundary = 20;
	int rightBoundary = 465;


	void update(float dt);
	void myUpdate(float dt);
	//void onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event);
	//void onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event);
	void updateIfHuajiIsfOnTheGroundAndResetHuajisYPosition();
	void updateBulletPositionAndResetThemIfTheyFlyOutOfTheScreen();


	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Stage4Scene);
};

#endif // __STAGE3_SCENE_H__