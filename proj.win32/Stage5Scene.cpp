#include "Stage5Scene.h"
#include "Stage6Scene.h"
#include "GameOverScene.h"
#include "BossSprite.h"
#include "HuajiSprite.h"
#include "SimpleAudioEngine.h"
#include "math.h"


USING_NS_CC;

Scene* Stage5Scene::createScene()
{
	return Stage5Scene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in Stage1Scene.cpp\n");
}

// on "init" you need to initialize your instance
bool Stage5Scene::init()
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
		CC_CALLBACK_1(Stage5Scene::menuCloseCallback, this));

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

	auto label = Label::createWithTTF("Parry bullets and counter-attack!", "fonts/Marker Felt.ttf", 48);
	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		// position the label on the center of the screen
		label->setPosition(visibleSize.width / 2, visibleSize.height * 80 / 100);

		// add the label as a child to this layer
		this->addChild(label, 1);
	}

	//add a picture sprite as background
	auto backgroundSprite = Sprite::create("background_double.png");
	backgroundSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(backgroundSprite, -1);


	//add stage1 label on the top left of the window
	auto stage5Label = Label::createWithTTF("Stage5", "fonts/Marker Felt.ttf", 24);
	stage5Label->setPosition(stage5Label->getContentSize().width / 2, visibleSize.height - stage5Label->getContentSize().height / 2);
	this->addChild(stage5Label);

	//add BulletTime! label
	auto btLabel = Label::createWithTTF("BULLET TIME!", "fonts/Marker Felt.ttf", 48);
	btLabel->setPosition(visibleSize.width / 2, visibleSize.height * 90 / 100);
	btLabel->setVisible(false);
	btLabel->setTag(250);
	this->addChild(btLabel);

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
	for (int i = 0; i < 18; i++){
		Sprite* groundSprite = Sprite::create("ground_double.png");
		groundSprite->setPosition(i * groundSprite->getContentSize().width, groundSprite->getContentSize().height / 2);
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

	//add "乖" on the bottom left of the window
	auto guaiSprite = Sprite::create("guai_double.png");
	guaiSprite->setPosition(visibleSize.width - guaiSprite->getContentSize().width * 3 / 2, guaiSprite->getContentSize().height * 3 / 2);
	guaiSprite->setTag(101);
	this->addChild(guaiSprite, 1);

	//添加boss
	BossSprite * bossSprite = BossSprite::create("nu_with_gun_double.png");
	bossSprite->setPosition(visibleSize.width - guaiSprite->getContentSize().width * 3 / 2 - bossSprite->getContentSize().width/2, guaiSprite->getContentSize().height * 3 / 2+20);
	bossSprite->setTag(300);
	this->addChild(bossSprite);

	//添加boss血条
	for (int i = 0; i < bossSprite->hp; i++){
		Sprite* hp_box = Sprite::create("hp_1.png");
		hp_box->setPosition(visibleSize.width - guaiSprite->getContentSize().width * 3 / 2 - bossSprite->getContentSize().width / 2  - 40 + i *hp_box->getContentSize().width, guaiSprite->getContentSize().height * 3 / 2 + 120);
		hp_box->setTag(301+i);
		this->addChild(hp_box);
	}

	//auto rotateAction = RotateBy::create(3, 90);
	//bossSprite->runAction(rotateAction);

	
	//这是一个计时子弹。每当它被重置在出生点时，就有可能引出另一件事。
	BulletSprite * bulletSprite = BulletSprite::create("bullet_head_up.png");
	bulletSprite->setPosition(200, -50);
	bulletSprite->setTag(199);
	bulletSprite->setRotation(-90);
	bulletSprite->current_speed_x = -5;
	bulletSprite->speed = 5;
	bulletSprite->launch_position_x = 200;
	bulletSprite->launch_position_y = -50;
	this->addChild(bulletSprite,1);

	
	//批量添加一些子弹
	this->bullet_tag_begin = 200;
	this->bullet_tag_end = 209;
	for (int i = this->bullet_tag_begin; i < bullet_tag_end+1; i++){
		BulletSprite * bullet = BulletSprite::create("bullet_head_up_double.png");
		bullet->setPosition(-40,	-50);
		bullet->setTag(i);
		bullet->setRotation(-90);
		bullet->current_speed_x = -5;
		bullet->speed = 5;
		bullet->launch_position_x = -40;
		bullet->launch_position_y = -50;
		this->addChild(bullet, 1);
	}

	//白色箭头，仅子弹时间内可见，位于滑稽和踩住的子弹之间，指示滑稽弹射位置
	Sprite * whiteArrow = Sprite::create("white_arrow_double.png");
	whiteArrow->setPosition(250, 150);
	whiteArrow->setTag(150);
	whiteArrow->setVisible(false);
	this->addChild(whiteArrow,1);


	this->leftBoundary = huajiSprite->getContentSize().width / 2;
	this->rightBoundary = visibleSize.width - huajiSprite->getContentSize().width / 2;

	//下面定义键盘事件监听器
	//add keyboard listener，检测A和D是否按下
	auto kbListener1 = EventListenerKeyboard::create();

	kbListener1->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event){
		if (keyCode == EventKeyboard::KeyCode::KEY_D){
			this->key_D_down = true;
			this->freezeScene = false;

		}
		if (keyCode == EventKeyboard::KeyCode::KEY_A){
			this->key_A_down = true;
			this->freezeScene = false;

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

	//下面定义鼠标事件监听器
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
			this->freezeScene = false;

			//如果滑稽在子弹附近时按下左键，则触发子弹时间
			if (huajiSprite->flying){
				for (int i = this->bullet_tag_begin; i < this->bullet_tag_end + 1; i++){
					BulletSprite * bulletSprite = (BulletSprite*)this->getChildByTag(i);
					int x_distance = huajiSprite->getPositionX() - bulletSprite->getPositionX();
					int y_distance = huajiSprite->getPositionY() - bulletSprite->getPositionY();
					if (sqrt(pow(x_distance, 2) + pow(y_distance, 2)) < huajiSprite->max_bullet_time_detection_distance){

						Label* btLabel = (Label *)this->getChildByTag(250);
						btLabel->setVisible(true);

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
		this->freezeScene = false;

		EventMouse* e = (EventMouse*)event;
		EventMouse::MouseButton  mouseButton = e->getMouseButton();
		if (mouseButton == EventMouse::MouseButton::BUTTON_LEFT){

			Label* btLabel = (Label *)this->getChildByTag(250);
			btLabel->setVisible(false);

			Sprite * whiteArrow = (Sprite*)this->getChildByTag(150);
			whiteArrow->setVisible(false);

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
					endPointSprite->runAction(fadeOutAction->clone());
					this->addChild(endPointSprite, 1);
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

	this->schedule(schedule_selector(Stage5Scene::update));
	return true;
}


void Stage5Scene::menuCloseCallback(Ref* pSender)
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

void Stage5Scene::update(float dt){
	if (this->freezeScene){
		return;
	}
	HuajiSprite * huaji = (HuajiSprite *) this->getChildByTag(100);
	Sprite * guai = (Sprite*) this->getChildByTag(101);
	BulletSprite *timerBullet = (BulletSprite*)this->getChildByTag(199);
	BossSprite* boss = (BossSprite*)this->getChildByTag(300);

	Sprite * btCircle = (Sprite*) this->getChildByTag(99);
	btCircle->setPosition(huaji->getPosition());

	//boss血量归零就算过关
	if (boss->hp <= 0 && huaji->on_the_ground){
		//BulletSprite * timerBullet = (BulletSprite*)this->getChildByTag(199);
		//timerBullet->current_speed_x = 0;
		//playEpilogue();
		Director::getInstance()->replaceScene(Stage6Scene::create());
	}

	//滑稽救到小乖就算过关
	if (std::abs(huaji->getPositionX() - guai->getPositionX()) < 10 && std::abs(huaji->getPositionY() - guai->getPositionY()) < 10){
		//huaji->setPosition(huaji->getPositionX(), huaji->getPositionY() + hu);
		Director::getInstance()->replaceScene(Stage6Scene::create());
	}

	//滑稽碰到boss就算死亡
	if (sqrt(pow(huaji->getPositionX() - boss->getPositionX(), 2) + pow(huaji->getPositionY() - boss->getPositionY(), 2)) <( huaji->getContentSize().width/2+boss->getContentSize().height/2 -10)){
		//huaji->setPosition(huaji->getPositionX(), huaji->getPositionY() + hu);
		GameOverScene* gmScene = (GameOverScene*)GameOverScene::createScene();
		gmScene->lastStage = 5;
		Director::getInstance()->replaceScene(gmScene);
	}

	//如果滑稽高度过低，则判定坠亡
	if (huaji->getPositionY() < 10){
		GameOverScene* gmScene = (GameOverScene*)GameOverScene::createScene();
		gmScene->lastStage = 5;
		Director::getInstance()->replaceScene(gmScene);
	}

	int bulletTagBegin = this->bullet_tag_begin;
	int bulletTagEnd = this->bullet_tag_end;

	//如果滑稽触碰子弹，则判定受击身亡
	for (int i = bulletTagBegin; i < bulletTagEnd + 1; i++){
		BulletSprite * bulletSprite = (BulletSprite*)this->getChildByTag(i);
		int x_distance = huaji->getPositionX() - bulletSprite->getPositionX();
		int y_distance = huaji->getPositionY() - bulletSprite->getPositionY();
		if (sqrt(pow(x_distance, 2) + pow(y_distance, 2)) < huaji->max_bullet_hit_distance && bulletSprite->harmful_to_huaji){
			GameOverScene* gmScene = (GameOverScene*)GameOverScene::createScene();
			gmScene->lastStage = 5;
			Director::getInstance()->replaceScene(gmScene);
		}
	}

	/*
	//这段逻辑已经挪入鼠标监听了
	//如果滑稽在子弹附近时按下左键，则触发子弹时间
	if (huaji->flying && this->mouse1_down){
		for (int i = bulletTagBegin; i < bulletTagEnd + 1; i++){
			BulletSprite * bulletSprite = (BulletSprite*)this->getChildByTag(i);
			int x_distance = huaji->getPositionX() - bulletSprite->getPositionX();
			int y_distance = huaji->getPositionY() - bulletSprite->getPositionY();
			if (sqrt(pow(x_distance, 2) + pow(y_distance, 2)) < huaji->max_bullet_time_detection_distance){

				Label* btLabel = (Label *)this->getChildByTag(250);
				btLabel->setVisible(true);
				this->bullet_time = true;
				Director::getInstance()->pause();
			}
		}
	}
	*/
	if (this->bullet_time && this->mouse1_up){
		Label* btLabel = (Label *)this->getChildByTag(250);
		btLabel->setVisible(false);
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
	if (huaji->getPositionX() < this->leftBoundary){
		huaji->setPositionX(leftBoundary);
		huaji->current_speed_x_pix_by_frame = 0;
	}
	if (huaji->getPositionX() > this->rightBoundary){
		huaji->setPositionX(rightBoundary);
		huaji->current_speed_x_pix_by_frame = 0;
	}

	/*for (int i = bulletTagBegin; i < bulletTagEnd + 1; i++){
	BulletSprite * bullet = (BulletSprite *) this->getChildByTag(i);
	bullet->setPosition(bullet->getPositionX() + bullet->speed_x, bullet->getPositionY() + bullet->speed_y);
	}
	*/


	//下面是boss的动作。

	if (boss->hp > 0){
		//时时刻刻把枪口对准滑稽

		//double degree_from_boss_to_huaji_mathematical = this->angle_sprite_to_sprite_mathematical_degree(boss, huaji);
		//double degree_from_boss_to_huaji_mathematical = this->angle_mathematical_degree(boss->getPositionX(), boss->getPositionY(), huaji->getPositionX(), huaji->getPositionY());

		int deltax = huaji->getPositionX() - boss->getPositionX();
		int deltay = huaji->getPositionY() - boss->getPositionY();

		double sinTheta = (double)deltay / ((double)sqrt((pow(deltax, 2) + pow(deltay, 2))));
		double theta_in_radius = asin(sinTheta);
		if (deltax < 0 && deltay >= 0){
			theta_in_radius = M_PI - theta_in_radius;
		}
		else if (deltax < 0 && deltay < 0){
			theta_in_radius = -M_PI - theta_in_radius;
		}
		double degree_from_boss_to_huaji_mathematical = theta_in_radius * 180.0 / M_PI;


		double degree_of_boss_head_mathematical = degree_from_boss_to_huaji_mathematical - 90;
		double degree_of_boss_head_game_world = 90 - degree_of_boss_head_mathematical;
		double boss_good_aim = 1;
		boss->setRotation(boss->getRotation()*(1 - boss_good_aim) + degree_of_boss_head_game_world*boss_good_aim);


		//每隔一段时间就有一颗子弹自愿跑进他的手枪向滑稽开火
		if (this->seconds_count % 5 == 0){
			int hash_divider = this->bullet_tag_end - this->bullet_tag_begin;
			int theChosenOne = 200 + seconds_count / 5 % hash_divider;
			BulletSprite *bullet = (BulletSprite *)this->getChildByTag(theChosenOne);
			if (bullet){
				//bullet->setPosition(gun->getPositionX(), gun->getPositionY());
				//to be modified
				double distance_between_boss_heart_and_pistol = boss->getContentSize().height /2 +20;

				double distance_between_boss_and_huaji = sqrt(deltax*deltax + deltay*deltay);


				double offset_x = distance_between_boss_heart_and_pistol*deltax / distance_between_boss_and_huaji;
				double offset_y = distance_between_boss_heart_and_pistol*deltay / distance_between_boss_and_huaji;

				bullet->setPosition(boss->getPositionX() + offset_x, boss->getPositionY() + offset_y);
				bullet->setRotation(degree_of_boss_head_game_world - 90);
				bullet->current_speed_x = 4 * deltax / distance_between_boss_and_huaji;
				bullet->current_speed_y = 4 * deltay / distance_between_boss_and_huaji;
			}
		}
	}

	this->updateBulletPositionAndResetThemIfTheyFlyOutOfTheScreen();

	this->updateIfHuajiIsfOnTheGroundAndResetHuajisYPosition(0,17);

	this->updateTimerLoop();
}


void Stage5Scene::updateIfHuajiIsfOnTheGroundAndResetHuajisYPosition(int blockBegin, int blockEnd){
	bool huajiOnOneGround = false;
	HuajiSprite* huaji = (HuajiSprite*)this->getChildByTag(100);
	int huajiWidth = huaji->getContentSize().width;
	int huajiHeight = huaji->getContentSize().height;
	for (int i = blockBegin; i < blockEnd+1; i++){
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

void Stage5Scene::updateBulletPositionAndResetThemIfTheyFlyOutOfTheScreen(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	BossSprite* boss = (BossSprite*)this->getChildByTag(300);
	int bulletTagBegin = this->bullet_tag_begin;
	int bulletTagEnd = this->bullet_tag_end;
	for (int i = bulletTagBegin; i < bulletTagEnd + 1; i++){
		BulletSprite * bullet = (BulletSprite *)this->getChildByTag(i);
		bullet->setPosition(bullet->getPositionX() + bullet->current_speed_x, bullet->getPositionY() + bullet->current_speed_y);
		if (bullet->getPositionX() < -100
			|| bullet->getPositionX() > visibleSize.width + 100
			|| bullet->getPositionY() < -100
			|| bullet->getPositionY() >visibleSize.height + 100){
			bullet->setPosition(bullet->launch_position_x, bullet->launch_position_y);
			bullet->current_speed_x = 0;
			bullet->current_speed_y = 0;
			bullet->setRotation(-90);
			bullet->harmful_to_huaji = true;
		}
		if (sqrt(pow(bullet->getPositionX() - boss->getPositionX(), 2) + pow(bullet->getPositionY() - boss->getPositionY(), 2)) < boss->getContentSize().height /2 -5){
			

			Sprite* hp_box = (Sprite*) this->getChildByTag(300 + boss->hp);
			hp_box->setVisible(false);
			boss->hp--;
			Sprite * explosionSprite = Sprite::create("explosion_1_double.png");
			FadeOut* fadeOutAction = FadeOut::create(1);
			explosionSprite->setPosition(bullet->getPosition());
			this->addChild(explosionSprite,3);
			explosionSprite->runAction(fadeOutAction);
			bullet->setPosition(bullet->launch_position_x, bullet->launch_position_y);
			bullet->current_speed_x = 0;
			bullet->current_speed_y = 0;
			bullet->setRotation(-90);
			bullet->harmful_to_huaji = true;

		}
	}
}
void Stage5Scene::updateTimerLoop(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	BulletSprite* bullet = (BulletSprite*) this->getChildByTag(199);
	bullet->setPosition(bullet->getPositionX() + bullet->current_speed_x, bullet->getPositionY() + bullet->current_speed_y);
	if (bullet->getPositionX() < -100
		|| bullet->getPositionX() > visibleSize.width+100
		|| bullet->getPositionY() < -100
		|| bullet->getPositionY() >visibleSize.height + 100){
		bullet->setPosition(bullet->launch_position_x, bullet->launch_position_y);
		bullet->current_speed_x = -bullet->speed;
		bullet->current_speed_y = 0;
		bullet->setRotation(-90);
		this->seconds_count += 1;
	}
}

double Stage5Scene::angle_mathematical_radius(int begin_x, int begin_y, int end_x, int end_y){
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

double Stage5Scene::angle_mathematical_degree(int begin_x, int begin_y, int end_x, int end_y){
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
double Stage5Scene::angle_sprite_to_sprite_mathematical_degree(Sprite * begin, Sprite* end){
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

void Stage5Scene::playEpilogue(){

	BossSprite* boss = (BossSprite*)this->getChildByTag(300);

	FadeOut * fadeOutAction = FadeOut::create(2);
	boss->runAction(fadeOutAction);

	HuajiSprite * huaji = (HuajiSprite*) this->getChildByTag(100);
	Sprite * guai = (Sprite*) this->getChildByTag(101);


}