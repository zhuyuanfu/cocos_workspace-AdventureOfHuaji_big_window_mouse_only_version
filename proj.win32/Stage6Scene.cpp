#include "Stage6Scene.h"
#include "PrologueScene.h"
#include "GameOverScene.h"
#include "BossSprite.h"
#include "HuajiSprite.h"
#include "SimpleAudioEngine.h"
#include "math.h"


USING_NS_CC;

Scene* Stage6Scene::createScene()
{
	return Stage6Scene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in Stage1Scene.cpp\n");
}

// on "init" you need to initialize your instance
bool Stage6Scene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(Stage6Scene::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 3);

	auto label = Label::createWithTTF("Thank you for playing!", "fonts/Marker Felt.ttf", 48);
	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		// position the label on the center of the screen
		label->setPosition(visibleSize.width/2, visibleSize.height *40/100);
		label->setOpacity(0);
		//label->setVisible(false);
		// add the label as a child to this layer
		this->addChild(label, 1);
	}

	Label * finallyLabel = Label::createWithTTF("Grats! Huaji and Xiaoguai are happy together again.", "fonts/Marker Felt.ttf", 48);
	finallyLabel->setPosition(visibleSize.width / 2, visibleSize.height * 50 / 100);
	finallyLabel->setOpacity(0);
	this->addChild(finallyLabel);

	//add a picture sprite as background
	auto backgroundSprite = Sprite::create("background_double.png");
	backgroundSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(backgroundSprite, -1);


	//add epilugue label on the top left of the window
	auto epilogueLabel = Label::createWithTTF("Epilogue", "fonts/Marker Felt.ttf", 24);
	epilogueLabel->setPosition(epilogueLabel->getContentSize().width / 2, visibleSize.height - epilogueLabel->getContentSize().height/2);
	this->addChild(epilogueLabel);

	Label * skipLabel = Label::createWithTTF("Replay", "fonts/Marker Felt.ttf", 24);
	MenuItemLabel * menuItemSkip = MenuItemLabel::create(skipLabel, CC_CALLBACK_1(Stage6Scene::goToPrologueSceneCallback, this));
	Menu * topRightMenu = Menu::create(menuItemSkip, nullptr);
	topRightMenu->setPosition(visibleSize.width - skipLabel->getContentSize().width / 2, visibleSize.height - skipLabel->getContentSize().height / 2);
	this->addChild(topRightMenu);


	//add "huaji" on the bottom left of the window
	HuajiSprite * huajiSprite = HuajiSprite::create("huaji_double.png");
	huajiSprite->setPosition(huajiSprite->getContentSize().width * 3 / 2, huajiSprite->getContentSize().height *3/2 -5);
	huajiSprite->setTag(100);

	this->addChild(huajiSprite, 1);

	//add grounds on the bottom left of the window
	for (int i = 0; i < 18; i++){
		Sprite* groundSprite = Sprite::create("ground_double.png");
		groundSprite->setPosition(i * groundSprite->getContentSize().width, groundSprite->getContentSize().height/2);
		groundSprite->setTag(i);
		this->addChild(groundSprite, 0);
	}


	/*
	//add "meigui" on the bottom right of the window
	auto meiguiSprite = Sprite::create("meigui.png");
	meiguiSprite->setPosition(420, 48);
	meiguiSprite->setTag(101);
	this->addChild(meiguiSprite, 1);
	*/

	//add "乖" on the bottom right of the window
	Sprite * guaiSprite = Sprite::create("guai_double.png");
	guaiSprite->setPosition(visibleSize.width - guaiSprite->getContentSize().width * 3 / 2, guaiSprite->getContentSize().height * 3 / 2 - 5);
	guaiSprite->setTag(101);
	this->addChild(guaiSprite, 1);


	//四朵玫瑰
	Sprite * roseSprite1 = Sprite::create("meigui_double.png");
	roseSprite1->setPosition(-280, 150);
	roseSprite1->setOpacity(0);
	this->addChild(roseSprite1,1);

	Sprite * roseSprite2 = Sprite::create("meigui_double.png");
	roseSprite2->setPosition(-250, 150);
	roseSprite2->setOpacity(0);
	this->addChild(roseSprite2,1);

	Sprite * roseSprite3 = Sprite::create("meigui_double.png");
	roseSprite3->setPosition(-220, 150);
	roseSprite3->setOpacity(0);
	this->addChild(roseSprite3,1);

	Sprite * roseSprite4 = Sprite::create("meigui_double.png");
	roseSprite4->setPosition(-190, 150);
	roseSprite4->setOpacity(0);
	this->addChild(roseSprite4,1);

	//心形圈
	Sprite * heartSprite = Sprite::create("aixin_double.png");
	heartSprite->setPosition(-150, 180);
	heartSprite->setOpacity(0);
	this->addChild(heartSprite);


	DelayTime * delayTime_1s = DelayTime::create(1.0f);
	//以下是动作
	//滑稽和小乖奔向彼此
	MoveTo * guaiMoveToAction = MoveTo::create(3, Vec2(visibleSize.width / 2 + huajiSprite->getContentSize().width * 5 / 2, huajiSprite->getContentSize().height * 3 / 2 - 5));
	MoveTo* huajiMoveToAction = MoveTo::create(4, Vec2(visibleSize.width / 2 + huajiSprite->getContentSize().width * 3 / 2, huajiSprite->getContentSize().height * 3 / 2 - 5));

	TargetedAction *huajiMoveTo = TargetedAction::create(huajiSprite, huajiMoveToAction);
	TargetedAction *guaiMoveTo = TargetedAction::create(guaiSprite, guaiMoveToAction);
	Spawn *runCloseSpawn = Spawn::create(huajiMoveToAction, guaiMoveTo, nullptr);

	//滑稽定睛细看小乖两秒
	DelayTime * delayTime_2s = DelayTime::create(2.0f);

	//滑稽高兴地跳了两下
	JumpBy * huajiJumpByAction = JumpBy::create(1,Vec2(0,0),30,2);

	//放下第一支玫瑰
	MoveTo * rose1MoveToAction = MoveTo::create(0, Vec2(visibleSize.width / 2 + huajiSprite->getContentSize().width * 3 / 2, huajiSprite->getContentSize().height * 3 / 2 - 5));
	FadeIn * rose1FadeInAction = FadeIn::create(1);
	Sequence * rose1Sequence = Sequence::create(rose1MoveToAction, rose1FadeInAction, nullptr);
	TargetedAction * rose1Appear = TargetedAction::create(roseSprite1, rose1Sequence);

	MoveTo * huajiMoveToAction1 = MoveTo::create(1, Vec2(visibleSize.width / 2 + huajiSprite->getContentSize().width * 1 / 2, huajiSprite->getContentSize().height * 3 / 2 - 5));
	TargetedAction * huajiMoveTo1 = TargetedAction::create(huajiSprite, huajiMoveToAction1);

	//放下第二支玫瑰
	MoveTo * rose2MoveToAction = MoveTo::create(0, Vec2(visibleSize.width / 2 + huajiSprite->getContentSize().width * 1 / 2, huajiSprite->getContentSize().height * 3 / 2 - 5));
	FadeIn * rose2FadeInAction = FadeIn::create(1);
	Sequence * rose2Sequence = Sequence::create(rose2MoveToAction, rose2FadeInAction, nullptr);
	TargetedAction * rose2Appear = TargetedAction::create(roseSprite2, rose2Sequence);

	MoveTo * huajiMoveToAction2 = MoveTo::create(1, Vec2(visibleSize.width / 2 + huajiSprite->getContentSize().width * -1 / 2, huajiSprite->getContentSize().height * 3 / 2 - 5));
	TargetedAction * huajiMoveTo2 = TargetedAction::create(huajiSprite, huajiMoveToAction2);

	//放下第三支玫瑰
	MoveTo * rose3MoveToAction = MoveTo::create(0, Vec2(visibleSize.width / 2 + huajiSprite->getContentSize().width * -1 / 2, huajiSprite->getContentSize().height * 3 / 2 - 5));
	FadeIn * rose3FadeInAction = FadeIn::create(1);
	Sequence * rose3Sequence = Sequence::create(rose3MoveToAction, rose3FadeInAction, nullptr);
	TargetedAction * rose3Appear = TargetedAction::create(roseSprite3, rose3Sequence);

	MoveTo * huajiMoveToAction3 = MoveTo::create(1, Vec2(visibleSize.width / 2 + huajiSprite->getContentSize().width * -3 / 2, huajiSprite->getContentSize().height * 3 / 2 - 5));
	TargetedAction * huajiMoveTo3 = TargetedAction::create(huajiSprite, huajiMoveToAction3);

	//放下第四支玫瑰
	MoveTo * rose4MoveToAction = MoveTo::create(0, Vec2(visibleSize.width / 2 + huajiSprite->getContentSize().width * -3 / 2, huajiSprite->getContentSize().height * 3 / 2 - 5));
	FadeIn * rose4FadeInAction = FadeIn::create(1);
	Sequence * rose4Sequence = Sequence::create(rose4MoveToAction, rose4FadeInAction, nullptr);
	TargetedAction * rose4Appear = TargetedAction::create(roseSprite4, rose4Sequence);

	MoveTo * huajiMoveToAction4 = MoveTo::create(1, Vec2(visibleSize.width / 2 + huajiSprite->getContentSize().width * -5 / 2, huajiSprite->getContentSize().height * 3 / 2 - 5));
	TargetedAction * huajiMoveTo4 = TargetedAction::create(huajiSprite, huajiMoveToAction4);

	//小乖高兴地跳两下
	JumpBy * guaiJumpAction = JumpBy::create(1,Vec2(0,0),30,2);
	TargetedAction *guaiJump = TargetedAction::create(guaiSprite, guaiJumpAction);

	//爱心出现在他们之间
	MoveTo * heartMoveToAction = MoveTo::create(0, Vec2(visibleSize.width / 2, huajiSprite->getContentSize().height * 5 / 2 - 5));
	JumpBy * heartJumpByAction = JumpBy::create(1,Vec2(0,0),30,2);
	FadeIn * heartFadeInAction = FadeIn::create(1);
	TargetedAction * heartMoveTo = TargetedAction::create(heartSprite, heartMoveToAction);
	TargetedAction * heartJumpBy = TargetedAction::create(heartSprite, heartJumpByAction);
	TargetedAction * heartFadeIn = TargetedAction::create(heartSprite, heartFadeInAction);

	//滑稽和小乖终于重聚的标题渐入
	FadeIn * finallyLabelFadeInAction = FadeIn::create(3);
	TargetedAction * finallyLabelFadeIn = TargetedAction::create(finallyLabel, finallyLabelFadeInAction);

	//感谢玩家游玩的标题渐入
	FadeIn * labelFadeInAction = FadeIn::create(3);
	TargetedAction * labelFadeIn = TargetedAction::create(label, labelFadeInAction);


	Sequence * finalSequence = Sequence::create(runCloseSpawn, delayTime_2s, huajiJumpByAction, delayTime_1s, rose1Appear, huajiMoveTo1, rose2Appear, huajiMoveTo2, rose3Appear, huajiMoveTo3, rose4Appear, huajiMoveTo4, delayTime_1s, guaiJump, delayTime_1s, heartMoveTo, heartFadeIn, heartJumpBy, delayTime_2s, finallyLabelFadeIn, labelFadeIn, nullptr);
	huajiSprite->runAction(finalSequence);
	


	//下面定义键盘事件监听器
	//add keyboard listener，检测A和D是否按下
	auto kbListener1 = EventListenerKeyboard::create();
	kbListener1->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event){
		if (keyCode == EventKeyboard::KeyCode::KEY_D){
			this->key_D_down = true;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_A){
			this->key_A_down = true;
		}
	};
	kbListener1->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event){
		if (keyCode == EventKeyboard::KeyCode::KEY_D){
			this->key_D_down = false;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_A){
			this->key_A_down = false;
		}
	};
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(kbListener1, this);

	//下面定义鼠标事件监听器
	auto mouseListener1 = EventListenerMouse::create();
	mouseListener1->onMouseDown = [=](Event * event){
		this->mouse1_down = true;
		this->mouse1_up = false;

		if (huajiSprite->on_the_ground){
			//huajiSprite->setPositionY(huajiSprite->getPositionY() + 5);
			huajiSprite->flying = true;
			huajiSprite->on_the_ground = false;
			huajiSprite->current_speed_y_pix_by_frame = 3;
		}
		//EventMouse* e = (EventMouse*)event;
		//huajiSprite->setPosition(e->getCursorX(), e->getCursorY());
	};
	mouseListener1->onMouseUp = [this, huajiSprite](Event * event){
		this->mouse1_down = false;
		this->mouse1_up = true;

		if (this->bullet_time){
			this->key_A_down = false;
			this->key_D_down = false;
			//TODO 这边需要放一个很复杂的设置双方速度、方向的大段算法。
			//通过遍历取出一个子弹
			BulletSprite * tempBulletSprite;
			for (int i = bullet_tag_begin; i < bullet_tag_end + 1; i++){
				tempBulletSprite = (BulletSprite*)this->getChildByTag(i);
				int x_distance = huajiSprite->getPositionX() - tempBulletSprite->getPositionX();
				int y_distance = huajiSprite->getPositionY() - tempBulletSprite->getPositionY();
				if (sqrt(pow(x_distance, 2) + pow(y_distance, 2)) < huajiSprite->max_bullet_time_detection_distance){
					break;
				}
			}


			EventMouse * e = (EventMouse*)event;

			//int x1 = huajiSprite->getPositionX();
			//int y1 = huajiSprite->getPositionY();

			int x1 = (huajiSprite->getPositionX() + tempBulletSprite->getPositionX()) / 2;
			int y1 = (huajiSprite->getPositionY() + tempBulletSprite->getPositionY()) / 2;

			int x2 = e->getCursorX();
			int y2 = e->getCursorY();



			//以下设置主角和子弹的飞行方向
			int deltax = x2 - x1;
			int deltay = y2 - y1;
			if (deltax == 0){
				if (deltay >= 0){
					huajiSprite->current_speed_y_pix_by_frame = huajiSprite->max_boost_speed;
					tempBulletSprite->current_speed_y = -tempBulletSprite->max_boost_speed;
				}
				else{
					huajiSprite->current_speed_y_pix_by_frame = -huajiSprite->max_boost_speed;
					tempBulletSprite->current_speed_y = tempBulletSprite->max_boost_speed;
				}
			}
			else{
				double divider = 1.0 + pow(deltay / deltax, 2);
				double huaji_speed_x_abs = sqrt(std::abs(huajiSprite->max_boost_speed / divider));
				double huaji_speed_y_abs = std::abs(huaji_speed_x_abs * deltay / deltax);

				double bullet_speed_x_abs = sqrt(std::abs(tempBulletSprite->max_boost_speed / divider));
				double bullet_speed_y_abs = std::abs(bullet_speed_x_abs * deltay / deltax);

				if (deltax > 0){
					huajiSprite->current_speed_x_pix_by_frame = huaji_speed_x_abs;
					tempBulletSprite->current_speed_x = -bullet_speed_x_abs;
				}
				else{
					huajiSprite->current_speed_x_pix_by_frame = -huaji_speed_x_abs;
					tempBulletSprite->current_speed_x = bullet_speed_x_abs;

				}

				if (deltay > 0){
					huajiSprite->current_speed_y_pix_by_frame = huaji_speed_y_abs;
					tempBulletSprite->current_speed_y = -bullet_speed_y_abs;
				}
				else{
					huajiSprite->current_speed_y_pix_by_frame = -huaji_speed_y_abs;
					tempBulletSprite->current_speed_y = bullet_speed_y_abs;

				}

				//以下设置子弹弹头的方向
				//TODO in mathematical world...
				double sinTheta = (double)deltay / ((double)sqrt((pow(deltax, 2) + pow(deltay, 2))));
				//double pi = 3.1415926536;
				//double cosTheta = deltax / (pow(deltax, 2) + pow(deltay, 2));
				//double tanTheta = deltay / deltax;
				double theta_in_radius = asin(sinTheta);
				if (deltax < 0 && deltay >= 0){
					theta_in_radius = M_PI - theta_in_radius;
				}
				else if (deltax < 0 && deltay < 0){
					theta_in_radius = -M_PI - theta_in_radius;
				}

				double direction_of_huaji_in_degree_in_mathematical_coordinates = theta_in_radius * 180.0 / M_PI;
				double direction_of_huaji_in_game_world = 90 - direction_of_huaji_in_degree_in_mathematical_coordinates;
				double direction_of_bullet_in_degree_in_mathematical_coordinates = direction_of_huaji_in_degree_in_mathematical_coordinates + 180.0;
				double direction_of_bullet_in_game_world = 90.0 - direction_of_bullet_in_degree_in_mathematical_coordinates;

				tempBulletSprite->setRotation(direction_of_bullet_in_game_world);

				//以下设置子弹和主角的位置，使其远离不至于撞上
				double middle_x = (x1 + tempBulletSprite->getPositionX()) / 2;
				double middle_y = (y1 + tempBulletSprite->getPositionY()) / 2;

				double radius = sqrt((huajiSprite->getPositionX() - middle_x)*(huajiSprite->getPositionX() - middle_x) + (y1 - middle_y)*(y1 - middle_y));
				if (radius < 5){
					radius = 5.0;
				}

				double huaji_x_offset = radius*cos(theta_in_radius);
				double huaji_y_offset = radius*sin(theta_in_radius);
				double bullet_x_offset = radius*cos(theta_in_radius + M_PI);
				double bullet_y_offset = radius*sin(theta_in_radius + M_PI);

				huajiSprite->setPosition(middle_x + huaji_x_offset, middle_y + huaji_y_offset);
				tempBulletSprite->setPosition(middle_x + bullet_x_offset, middle_y + bullet_y_offset);

				//以下为屏幕上设置一个滑稽弹射箭头的提示
				Sprite* arrowSprite = Sprite::create("arrow.png");
				//arrowSprite->setAnchorPoint(0.5,0.5);
				arrowSprite->setPosition(x1, y1);
				arrowSprite->setRotation(direction_of_huaji_in_game_world);


				this->addChild(arrowSprite);

				FadeOut *fadeOutAction = FadeOut::create(2);
				arrowSprite->runAction(fadeOutAction);

				Sprite* endPointSprite = Sprite::create("black_crosshair.png");
				endPointSprite->setPosition(x2, y2);
				endPointSprite->runAction(fadeOutAction->clone());
				this->addChild(endPointSprite, 1);
				//this->removeChild(arrowSprite);

			}
			//this->gravity_acc_by_squareframe = 0;
			//huajiSprite->current_speed_y_pix_by_frame = 3;
			Director::getInstance()->resume();
			this->bullet_time = false;
		}
		//Director::getInstance()->resume();
		//EventMouse* e = (EventMouse*)event;d
		//huajiSprite->setPosition(e->getCursorX(), e->getCursorY());
	};
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener1, this);

	this->schedule(schedule_selector(Stage6Scene::update));
	return true;
}


void Stage6Scene::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}

void Stage6Scene::update(float dt){
	
}


void Stage6Scene::updateIfHuajiIsfOnTheGroundAndResetHuajisYPosition(int blockBegin, int blockEnd){
	bool huajiOnOneGround = false;
	HuajiSprite* huaji = (HuajiSprite*)this->getChildByTag(100);
	for (int i = blockBegin; i < blockEnd + 1; i++){
		Sprite* block = (Sprite*)this->getChildByTag(i);
		if (std::abs(huaji->getPositionX() - block->getPositionX()) < 28 && 25 <= huaji->getPositionY() - block->getPositionY() && huaji->getPositionY() - block->getPositionY() <= 30){
			//if (this->current_speed_y_pix_by_frame < 0){
			huaji->setPositionY(block->getPositionY() + 30);
			//}
			huajiOnOneGround = true;
			break;
		}
	}

	huaji->on_the_ground = huajiOnOneGround;
	huaji->flying = !huajiOnOneGround;
}

void Stage6Scene::updateBulletPositionAndResetThemIfTheyFlyOutOfTheScreen(){
	BossSprite* boss = (BossSprite*)this->getChildByTag(300);
	int bulletTagBegin = this->bullet_tag_begin;
	int bulletTagEnd = this->bullet_tag_end;
	for (int i = bulletTagBegin; i < bulletTagEnd + 1; i++){
		BulletSprite * bullet = (BulletSprite *)this->getChildByTag(i);
		bullet->setPosition(bullet->getPositionX() + bullet->current_speed_x, bullet->getPositionY() + bullet->current_speed_y);
		if (bullet->getPositionX() < -100
			|| bullet->getPositionX() > 600
			|| bullet->getPositionY() < -100
			|| bullet->getPositionY() >400){
			bullet->setPosition(bullet->launch_position_x, bullet->launch_position_y);
			bullet->current_speed_x = 0;
			bullet->current_speed_y = 0;
			bullet->setRotation(-90);
		}
		if (sqrt(pow(bullet->getPositionX() - boss->getPositionX(), 2) + pow(bullet->getPositionY() - boss->getPositionY(), 2)) < 40){

			boss->hp--;
			Sprite * explosionSprite = Sprite::create("explosion_1.png");
			FadeOut* fadeOutAction = FadeOut::create(2);
			explosionSprite->setPosition(bullet->getPosition());
			this->addChild(explosionSprite, 3);
			explosionSprite->runAction(fadeOutAction);
			bullet->setPosition(bullet->launch_position_x, bullet->launch_position_y);
			bullet->current_speed_x = 0;
			bullet->current_speed_y = 0;
			bullet->setRotation(-90);

		}
	}
}
void Stage6Scene::updateTimerLoop(){
	BulletSprite* bullet = (BulletSprite*) this->getChildByTag(199);
	bullet->setPosition(bullet->getPositionX() + bullet->current_speed_x, bullet->getPositionY() + bullet->current_speed_y);
	if (bullet->getPositionX() < -100
		|| bullet->getPositionX() > 600
		|| bullet->getPositionY() < -100
		|| bullet->getPositionY() >400){
		bullet->setPosition(bullet->launch_position_x, bullet->launch_position_y);
		bullet->current_speed_x = -bullet->speed;
		bullet->current_speed_y = 0;
		bullet->setRotation(-90);
		this->seconds_count += 1;
	}
}

double Stage6Scene::angle_mathematical_radius(int begin_x, int begin_y, int end_x, int end_y){
	int deltax = end_x - begin_x;
	int deltay = end_y - begin_y;

	double sinTheta = (double)deltay / ((double)sqrt((pow(deltax, 2) + pow(deltay, 2))));
	double theta_in_radius = asin(sinTheta);
	if (deltax < 0 && deltay >= 0){
		theta_in_radius = M_PI - theta_in_radius;
	}
	else if (deltax < 0 && deltay < 0){
		theta_in_radius = -M_PI - theta_in_radius;
	}

	return theta_in_radius;
}

double Stage6Scene::angle_mathematical_degree(int begin_x, int begin_y, int end_x, int end_y){
	int deltax = end_x - begin_x;
	int deltay = end_y - begin_y;

	double sinTheta = (double)deltay / ((double)sqrt((pow(deltax, 2) + pow(deltay, 2))));
	double theta_in_radius = asin(sinTheta);
	if (deltax < 0 && deltay >= 0){
		theta_in_radius = M_PI - theta_in_radius;
	}
	else if (deltax < 0 && deltay < 0){
		theta_in_radius = -M_PI - theta_in_radius;
	}
	double direction_mathematical_coordinates_degree = theta_in_radius * 180.0 / M_PI;
	return direction_mathematical_coordinates_degree;
}
double Stage6Scene::angle_sprite_to_sprite_mathematical_degree(Sprite * begin, Sprite* end){
	int deltax = end->getPositionX() - begin->getPositionX();
	int deltay = end->getPositionY() - end->getPositionY();
	double sinTheta = (double)deltay / ((double)sqrt((pow(deltax, 2) + pow(deltay, 2))));
	double theta_in_radius = asin(sinTheta);
	if (deltax < 0 && deltay >= 0){
		theta_in_radius = M_PI - theta_in_radius;
	}
	else if (deltax < 0 && deltay < 0){
		theta_in_radius = -M_PI - theta_in_radius;
	}
	double direction_mathematical_coordinates_degree = theta_in_radius * 180.0 / M_PI;
	return direction_mathematical_coordinates_degree;
}

void Stage6Scene::playEpilogue(){

	BossSprite* boss = (BossSprite*)this->getChildByTag(300);

	FadeOut * fadeOutAction = FadeOut::create(2);
	boss->runAction(fadeOutAction);

	HuajiSprite * huaji = (HuajiSprite*) this->getChildByTag(100);
	Sprite * guai = (Sprite*) this->getChildByTag(101);


}

void Stage6Scene::goToPrologueSceneCallback(Ref * pSender){
	Director::getInstance()->replaceScene(PrologueScene::createScene());
} 