#include "Stage3Scene.h"
#include "Stage4Scene.h"
#include "GameOverScene.h"
#include "BulletSprite.h"
#include "HuajiSprite.h"
#include "SimpleAudioEngine.h"
#include "math.h"

USING_NS_CC;

Scene* Stage3Scene::createScene()
{
	return Stage3Scene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in Stage1Scene.cpp\n");
}

// on "init" you need to initialize your instance
bool Stage3Scene::init()
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
		CC_CALLBACK_1(Stage3Scene::menuCloseCallback, this));

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

	auto label = Label::createWithTTF("Hold left button when flying near bullets.", "fonts/Marker Felt.ttf", 48);
	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		// position the label on the center of the screen
		label->setPosition(visibleSize.width / 2, visibleSize.height * 80 / 100);
		label->setTag(249);
		// add the label as a child to this layer
		this->addChild(label, 1);
	}

	//add a picture sprite as background
	auto backgroundSprite = Sprite::create("background_double.png");
	backgroundSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(backgroundSprite, -1);


	//add stage1 label on the top left of the window
	auto stage3Label = Label::createWithTTF("Stage3", "fonts/Marker Felt.ttf", 24);
	stage3Label->setPosition(stage3Label->getContentSize().width / 2 , visibleSize.height - stage3Label->getContentSize().height / 2);
	this->addChild(stage3Label);

	//add BulletTime! label
	auto btLabel = Label::createWithTTF("BULLET TIME!", "fonts/Marker Felt.ttf", 48);
	btLabel->setPosition(visibleSize.width / 2, visibleSize.height * 90 / 100);
	btLabel->setVisible(false);
	btLabel->setTag(250);
	this->addChild(btLabel);

	//add BulletTime! label
	auto hintLabel = Label::createWithTTF("Release left button to launch Huaji towards your cursor", "fonts/Marker Felt.ttf", 48);
	hintLabel->setPosition(visibleSize.width / 2, visibleSize.height * 80 / 100);
	hintLabel->setVisible(false);
	hintLabel->setTag(251);
	this->addChild(hintLabel);



	//add "huaji" on the bottom left of the window
	HuajiSprite * huajiSprite = HuajiSprite::create("huaji_double.png");
	huajiSprite->setPosition(huajiSprite->getContentSize().width * 3 / 2, huajiSprite->getContentSize().height * 3 / 2);
	huajiSprite->setTag(100);

	this->addChild(huajiSprite, 2);

	Sprite * btCircle = Sprite::create("bullet_time_circle_double.png");
	btCircle->setTag(99);
	btCircle->setPosition(huajiSprite->getPosition());
	this->addChild(btCircle, 2);

	//add grounds on the bottom left of the window
	for (int i = 0; i < 6; i++){
		Sprite* groundSprite = Sprite::create("ground_double.png");
		groundSprite->setPosition(i * groundSprite->getContentSize().width, groundSprite->getContentSize().height / 2);
		groundSprite->setTag(i);
		this->addChild(groundSprite, 1);
	}

	//add grounds on the bottom right of the window, leaving a huge gap between huaji and the rose.
	for (int i = 6; i < 15; i++){
		Sprite* groundSprite = Sprite::create("ground_double.png");
		groundSprite->setPosition(i * groundSprite->getContentSize().width + 4 * groundSprite->getContentSize().width, groundSprite->getContentSize().height/2);
		groundSprite->setTag(i);
		this->addChild(groundSprite, 1);
	}

	//add "meigui" on the bottom left of the window
	auto meiguiSprite = Sprite::create("meigui_double.png");
	meiguiSprite->setPosition(visibleSize.width - meiguiSprite->getContentSize().width * 3 / 2 , meiguiSprite->getContentSize().height * 3 / 2);
	meiguiSprite->setTag(101);
	this->addChild(meiguiSprite, 1);






	this->bullet_tag_begin = 200;
	this->bullet_tag_end = 200;
	//add a bullet for tests
	BulletSprite * bulletSprite = BulletSprite::create("bullet_head_up_double.png");
	bulletSprite->setPosition(visibleSize.width + 100, visibleSize.height * 20 / 100);
	bulletSprite->setTag(200);
	bulletSprite->setRotation(-90);
	bulletSprite->launch_position_x = visibleSize.width + 100;
	bulletSprite->launch_position_y = visibleSize.height * 20 / 100;
	this->addChild(bulletSprite);

	//白色箭头，仅子弹时间内可见，位于滑稽和踩住的子弹之间，指示滑稽弹射位置
	Sprite * whiteArrow = Sprite::create("white_arrow_double.png");
	whiteArrow->setPosition(250, 150);
	whiteArrow->setTag(150);
	whiteArrow->setVisible(false);
	this->addChild(whiteArrow);



	this->leftBoundary = huajiSprite->getContentSize().width / 2;
	this->rightBoundary = visibleSize.width - huajiSprite->getContentSize().width / 2;

	//下面定义键盘和鼠标事件事件监听器
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
	_eventDispatcher->addEventListenerWithSceneGraphPriority(kbListener1, this);


	auto mouseListener1 = EventListenerMouse::create();

	mouseListener1->onMouseMove = [=](Event *event){
		if (this->bullet_time){
			Sprite * whiteArrow = (Sprite*)this->getChildByTag(150);
			whiteArrow->setVisible(true);


			//通过遍历取出一个子弹
			BulletSprite * tempBulletSprite;
			for (int i = bullet_tag_begin; i < bullet_tag_end + 1; i++){
				tempBulletSprite = (BulletSprite*)this->getChildByTag(i);
				int x_distance = huajiSprite->getPositionX() - tempBulletSprite->getPositionX();
				int y_distance = huajiSprite->getPositionY() - tempBulletSprite->getPositionY();
				if (sqrt(pow(x_distance, 2) + pow(y_distance, 2)) < huajiSprite->max_bullet_time_detection_distance + 5){
					break;
				}
			}

			//下面设定白色箭头的位置
			EventMouse * e = (EventMouse*)event;

			int x1 = (huajiSprite->getPositionX() + tempBulletSprite->getPositionX()) / 2;
			int y1 = (huajiSprite->getPositionY() + tempBulletSprite->getPositionY()) / 2;

			int x2 = e->getCursorX();
			int y2 = e->getCursorY();

			
			//下面设定白色箭头的指向方向
			int deltax = x2 - x1;
			int deltay = y2 - y1;

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


			
			whiteArrow->setPosition(x1, y1);
			whiteArrow->setRotation(direction_of_huaji_in_game_world);

		}

	};
	mouseListener1->onMouseDown = [=](Event * event){

		EventMouse* e = (EventMouse*)event;
		EventMouse::MouseButton  mouseButton = e->getMouseButton();

		if (mouseButton == EventMouse::MouseButton::BUTTON_LEFT){

			this->mouse1_down = true;
			this->mouse1_up = false;

			//如果滑稽在子弹附近时按下左键，则触发子弹时间
			if (huajiSprite->flying){
				for (int i = this->bullet_tag_begin; i < this->bullet_tag_end + 1; i++){
					BulletSprite * bulletSprite = (BulletSprite*)this->getChildByTag(i);
					int x_distance = huajiSprite->getPositionX() - bulletSprite->getPositionX();
					int y_distance = huajiSprite->getPositionY() - bulletSprite->getPositionY();
					if (sqrt(pow(x_distance, 2) + pow(y_distance, 2)) < huajiSprite->max_bullet_time_detection_distance){

						Label* btLabel = (Label *)this->getChildByTag(250);
						btLabel->setVisible(true);
						Label* hintLabel = (Label *)this->getChildByTag(251);
						hintLabel->setVisible(true);
						Label* label = (Label*)this->getChildByTag(249);
						label->setVisible(false);

						bulletSprite->harmful_to_huaji = false;

						this->bullet_time = true;

						//显示白色箭头
						Sprite * whiteArrow = (Sprite*)this->getChildByTag(150);
						whiteArrow->setVisible(true);


						//下面设定白色箭头的位置
						EventMouse * e = (EventMouse*)event;

						int x1 = (huajiSprite->getPositionX() + bulletSprite->getPositionX()) / 2;
						int y1 = (huajiSprite->getPositionY() + bulletSprite->getPositionY()) / 2;

						int x2 = e->getCursorX();
						int y2 = e->getCursorY();


						//下面设定白色箭头的指向方向
						int deltax = x2 - x1;
						int deltay = y2 - y1;

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



						whiteArrow->setPosition(x1, y1);
						whiteArrow->setRotation(direction_of_huaji_in_game_world);


						Director::getInstance()->pause();
						break;
					}
				}
			}


			if (huajiSprite->on_the_ground){
				//huajiSprite->setPositionY(huajiSprite->getPositionY() + 5);
				huajiSprite->flying = true;
				huajiSprite->on_the_ground = false;
				huajiSprite->current_speed_y_pix_by_frame = huajiSprite->jump_from_ground_speed_y;
			}
		}
		if (mouseButton == EventMouse::MouseButton::BUTTON_MIDDLE){
			this->key_A_down = true;
		}

		if (mouseButton == EventMouse::MouseButton::BUTTON_RIGHT){
			this->key_D_down = true;
		}
		//EventMouse* e = (EventMouse*)event;
		//huajiSprite->setPosition(e->getCursorX(), e->getCursorY());
	};
	mouseListener1->onMouseUp = [this, huajiSprite](Event * event){
		EventMouse* e = (EventMouse*)event;
		EventMouse::MouseButton  mouseButton = e->getMouseButton();
		if (mouseButton == EventMouse::MouseButton::BUTTON_LEFT){
			this->mouse1_down = false;
			this->mouse1_up = true;

			Sprite * whiteArrow = (Sprite*)this->getChildByTag(150);
			whiteArrow->setVisible(false);

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
					if (sqrt(pow(x_distance, 2) + pow(y_distance, 2)) < huajiSprite->max_bullet_time_detection_distance + 5){
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
				/*
				Sprite* startPointSprite = Sprite::create("red_crosshair.png");
				startPointSprite->setPosition(x1, y1);
				this->addChild(startPointSprite, 1);
				*/


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
					/*
					if (radius < huajiSprite->max_bullet_hit_distance/2){
					radius = huajiSprite->max_bullet_hit_distance/2;
					}
					*/

					double huaji_x_offset = radius*cos(theta_in_radius);
					double huaji_y_offset = radius*sin(theta_in_radius);
					double bullet_x_offset = radius*cos(theta_in_radius + M_PI);
					double bullet_y_offset = radius*sin(theta_in_radius + M_PI);

					huajiSprite->setPosition(middle_x + huaji_x_offset, middle_y + huaji_y_offset);
					tempBulletSprite->setPosition(middle_x + bullet_x_offset, middle_y + bullet_y_offset);


					//以下为屏幕上设置一个滑稽弹射箭头的提示
					Sprite* arrowSprite = Sprite::create("white_arrow_double.png");
					//arrowSprite->setAnchorPoint(0.5,0.5);
					arrowSprite->setPosition(x1, y1);
					arrowSprite->setRotation(direction_of_huaji_in_game_world);


					this->addChild(arrowSprite);

					FadeOut *fadeOutAction = FadeOut::create(2);
					arrowSprite->runAction(fadeOutAction);

					Sprite* endPointSprite = Sprite::create("black_crosshair.png");
					endPointSprite->setPosition(x2, y2);
					this->addChild(endPointSprite, 1);
					//FadeOut * fadeOutAction = FadeOut::create(2);
					endPointSprite->runAction(fadeOutAction->clone());

					//this->removeChild(arrowSprite);

				}
				//this->gravity_acc_by_squareframe = 0;
				//huajiSprite->current_speed_y_pix_by_frame = 3;
				Director::getInstance()->resume();
				this->bullet_time = false;
			}
		}
		if (mouseButton == EventMouse::MouseButton::BUTTON_MIDDLE){
			this->key_A_down = false;
		}

		if (mouseButton == EventMouse::MouseButton::BUTTON_RIGHT){
			this->key_D_down = false;
		}
		//Director::getInstance()->resume();
		//EventMouse* e = (EventMouse*)event;d
		//huajiSprite->setPosition(e->getCursorX(), e->getCursorY());
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener1, this);

	this->schedule(schedule_selector(Stage3Scene::update));
	return true;
}


void Stage3Scene::menuCloseCallback(Ref* pSender)
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

void Stage3Scene::update(float dt){
	HuajiSprite * huaji = (HuajiSprite *) this->getChildByTag(100);
	Sprite * meigui = (Sprite*) this->getChildByTag(101);

	Sprite * btCircle = (Sprite*) this->getChildByTag(99);
	btCircle->setPosition(huaji->getPosition());

	//滑稽取到玫瑰就算过关
	if (std::abs(huaji->getPositionX() - meigui->getPositionX()) < 15 && std::abs(huaji->getPositionY() - meigui->getPositionY()) < 15){
		huaji->setPosition(huaji->getPositionX(), huaji->getPositionY() + 30);
		Director::getInstance()->replaceScene(Stage4Scene::create());
	}

	//如果滑稽高度过低，则判定坠亡
	if (huaji->getPositionY() < 10){
		Director::getInstance()->replaceScene(GameOverScene::createScene());
	}

	int bulletTagBegin = this->bullet_tag_begin;
	int bulletTagEnd = this->bullet_tag_end;

	//如果滑稽触碰子弹，则判定受击身亡
	for (int i = bulletTagBegin; i < bulletTagEnd + 1; i++){
		BulletSprite * bulletSprite = (BulletSprite*)this->getChildByTag(i);
		int x_distance = huaji->getPositionX() - bulletSprite->getPositionX();
		int y_distance = huaji->getPositionY() - bulletSprite->getPositionY();
		if (sqrt(pow(x_distance, 2) + pow(y_distance, 2)) < huaji->max_bullet_hit_distance && bulletSprite->harmful_to_huaji){
			Director::getInstance()->replaceScene(GameOverScene::createScene());
		}
	}
	/*
	//如果滑稽在子弹附近时按下左键，则触发子弹时间
	if (huaji->flying && this->mouse1_down){
		for (int i = bulletTagBegin; i < bulletTagEnd + 1; i++){
			BulletSprite * bulletSprite = (BulletSprite*)this->getChildByTag(i);
			int x_distance = huaji->getPositionX() - bulletSprite->getPositionX();
			int y_distance = huaji->getPositionY() - bulletSprite->getPositionY();
			if (sqrt(pow(x_distance, 2) + pow(y_distance, 2)) < huaji->max_bullet_time_detection_distance){

				Label* btLabel = (Label *)this->getChildByTag(250);
				btLabel->setVisible(true);
				Label* hintLabel = (Label *)this->getChildByTag(251);
				hintLabel->setVisible(true);
				Label* label = (Label*)this->getChildByTag(249);
				label->setVisible(false);

				this->bullet_time = true;
				Director::getInstance()->pause();
			}
		}
	}
	*/
	if (this->mouse1_up){
		Label* btLabel = (Label *)this->getChildByTag(250);
		btLabel->setVisible(false);
		Label* hintLabel = (Label *)this->getChildByTag(251);
		hintLabel->setVisible(false);
		Label* label = (Label*)this->getChildByTag(249);
		label->setVisible(true);

		this->bullet_time = false;
		Director::getInstance()->resume();
	}


	if (huaji->flying){
		huaji->setPositionY(huaji->getPositionY() + huaji->current_speed_y_pix_by_frame * 1);
		huaji->current_speed_y_pix_by_frame = huaji->current_speed_y_pix_by_frame + this->gravity_acc_by_squareframe * 1;
		if (huaji->current_speed_y_pix_by_frame < -abs(huaji->max_speed_y_pix_by_frame)){
			huaji->current_speed_y_pix_by_frame = -abs(huaji->max_speed_y_pix_by_frame);
		}

		huaji->setPositionX(huaji->getPositionX() + huaji->current_speed_x_pix_by_frame);
	}
	if (huaji->on_the_ground){
		huaji->current_speed_y_pix_by_frame = 0;
		if ((!this->key_A_down) && (!this->key_D_down)){
			huaji->current_speed_x_pix_by_frame = 0;
		}
	}

	if (huaji->flying && this->key_D_down){
		huaji->current_speed_x_pix_by_frame = huaji->max_speed_x_pix_by_frame;
		//huaji->setPositionX(huaji->getPositionX() + huaji->current_speed_x_pix_by_frame);

	}
	if (huaji->flying && this->key_A_down){
		huaji->current_speed_x_pix_by_frame = -huaji->max_speed_x_pix_by_frame;
		//huaji->setPositionX(huaji->getPositionX() + huaji->current_speed_x_pix_by_frame);

	}

	if (huaji->on_the_ground && this->key_D_down){
		huaji->current_speed_x_pix_by_frame = huaji->max_speed_x_pix_by_frame;
		huaji->setPositionX(huaji->getPositionX() + huaji->current_speed_x_pix_by_frame);
	}
	if (huaji->on_the_ground && this->key_A_down){
		huaji->current_speed_x_pix_by_frame = -huaji->max_speed_x_pix_by_frame;
		huaji->setPositionX(huaji->getPositionX() + huaji->current_speed_x_pix_by_frame);
	}
	/*for (int i = bulletTagBegin; i < bulletTagEnd + 1; i++){
		BulletSprite * bullet = (BulletSprite *) this->getChildByTag(i);
		bullet->setPosition(bullet->getPositionX() + bullet->speed_x, bullet->getPositionY() + bullet->speed_y);
		}
		*/

	if (huaji->getPositionX() < this->leftBoundary){
		huaji->setPositionX(leftBoundary);
		huaji->current_speed_x_pix_by_frame = 0;
	}
	if (huaji->getPositionX() > this->rightBoundary){
		huaji->setPositionX(rightBoundary);
		huaji->current_speed_x_pix_by_frame = 0;
	}

	this->updateBulletPositionAndResetThemIfTheyFlyOutOfTheScreen();

	this->updateIfHuajiIsfOnTheGroundAndResetHuajisYPosition();
}


void Stage3Scene::updateIfHuajiIsfOnTheGroundAndResetHuajisYPosition(){
	bool huajiOnOneGround = false;
	HuajiSprite* huaji = (HuajiSprite*)this->getChildByTag(100);
	int huajiWidth = huaji->getContentSize().width;
	int huajiHeight = huaji->getContentSize().height;
	for (int i = 0; i < 15; i++){
		Sprite* block = (Sprite*)this->getChildByTag(i);
		if (std::abs(huaji->getPositionX() - block->getPositionX()) < (huajiWidth / 2 + block->getContentSize().width / 2) && (huajiHeight / 2 + block->getContentSize().height / 2 - 12) <= huaji->getPositionY() - block->getPositionY() && huaji->getPositionY() - block->getPositionY() <= (huajiHeight / 2 + block->getContentSize().height / 2 - 4)){
			//if (this->current_speed_y_pix_by_frame < 0){
			huaji->setPositionY(block->getPositionY() + huajiHeight / 2 + block->getContentSize().height / 2 - 5);
			//}
			huajiOnOneGround = true;
			break;
		}
	}

	huaji->on_the_ground = huajiOnOneGround;
	huaji->flying = !huajiOnOneGround;
}

void Stage3Scene::updateBulletPositionAndResetThemIfTheyFlyOutOfTheScreen(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	int bulletTagBegin = this->bullet_tag_begin;
	int bulletTagEnd = this->bullet_tag_end;
	for (int i = bulletTagBegin; i < bulletTagEnd + 1; i++){
		BulletSprite * bullet = (BulletSprite *) this->getChildByTag(i);
		bullet->setPosition(bullet->getPositionX() + bullet->current_speed_x, bullet->getPositionY() + bullet->current_speed_y);
		if (bullet->getPositionX() < -100
			|| bullet->getPositionX() > visibleSize.width + 100
			|| bullet->getPositionY() < -100
			|| bullet->getPositionY() > visibleSize.height + 100){
			bullet->setPosition(bullet->launch_position_x, bullet->launch_position_y);
			bullet->current_speed_x = -bullet->speed;
			bullet->current_speed_y = 0;
			bullet->setRotation(-90);
			bullet->harmful_to_huaji = true;
		}
	}
}