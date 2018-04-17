#include "Stage2Scene.h"
#include "Stage3Scene.h"
#include "HuajiSprite.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* Stage2Scene::createScene()
{
	return Stage2Scene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in Stage1Scene.cpp\n");
}

// on "init" you need to initialize your instance
bool Stage2Scene::init()
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
		CC_CALLBACK_1(Stage2Scene::menuCloseCallback, this));

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

	auto label = Label::createWithTTF("When on the ground, left click to jump.", "fonts/Marker Felt.ttf", 48);
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
	this->addChild(backgroundSprite,-1);


	//add stage1 label on the top left of the window
	auto stage2Label = Label::createWithTTF("Stage2", "fonts/Marker Felt.ttf", 24);
	stage2Label->setPosition(stage2Label->getContentSize().width/2, visibleSize.height - stage2Label->getContentSize().height/2);
	this->addChild(stage2Label);

	//add "huaji" on the bottom left of the window
	auto huajiSprite = HuajiSprite::create("huaji_double.png");
	huajiSprite->setPosition(huajiSprite->getContentSize().width*1.5, 90);
	huajiSprite->setTag(100);
	this->addChild(huajiSprite, 2);

	Sprite * btCircle = Sprite::create("bullet_time_circle_double.png");
	btCircle->setTag(99);
	btCircle->setPosition(huajiSprite->getPosition());
	this->addChild(btCircle, 2);

	//add grounds on the bottom left of the window
	for (int i = 0; i < 8; i++){
		auto groundSprite = Sprite::create("ground_double.png");
		groundSprite->setPosition(i * groundSprite->getContentSize().width, groundSprite->getContentSize().height/2);
		groundSprite->setTag(i);
		this->addChild(groundSprite, 1);
	}

	//add grounds on the bottom right of the window, leaving a gap between huaji and the rose.
	for (int i = 8; i < 18; i++){
		auto groundSprite = Sprite::create("ground_double.png");
		groundSprite->setPosition(i * groundSprite->getContentSize().width + 2 * groundSprite->getContentSize().width, groundSprite->getContentSize().height / 2);
		groundSprite->setTag(i);
		this->addChild(groundSprite, 1);
	}

	//add "meigui" on the bottom left of the window
	auto meiguiSprite = Sprite::create("meigui_double.png");
	meiguiSprite->setPosition(visibleSize.width - 1.5 * meiguiSprite->getContentSize().width, 2.5 * meiguiSprite->getContentSize().height);
	meiguiSprite->setTag(101);
	this->addChild(meiguiSprite, 1);





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
	mouseListener1->onMouseDown = [=](Event * event){


		EventMouse* e = (EventMouse*)event;
		EventMouse::MouseButton  mouseButton = e->getMouseButton();

		if (mouseButton == EventMouse::MouseButton::BUTTON_LEFT){

			this->mouse1_down = true;
			this->mouse1_up = false;
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
	};
	mouseListener1->onMouseUp = [=](Event * event){
		EventMouse* e = (EventMouse*)event;
		EventMouse::MouseButton  mouseButton = e->getMouseButton();

		if (mouseButton == EventMouse::MouseButton::BUTTON_LEFT){

			this->mouse1_down = false;
			this->mouse1_up = true;
			//EventMouse* e = (EventMouse*)event;
			//huajiSprite->setPosition(e->getCursorX(), e->getCursorY());
		}
		if (mouseButton == EventMouse::MouseButton::BUTTON_MIDDLE){
			this->key_A_down = false;
		}

		if (mouseButton == EventMouse::MouseButton::BUTTON_RIGHT){
			this->key_D_down = false;
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener1, this);

	this->schedule(schedule_selector(Stage2Scene::update));
	return true;
}


void Stage2Scene::menuCloseCallback(Ref* pSender)
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

void Stage2Scene::update(float dt){
	HuajiSprite * huaji = (HuajiSprite *) this->getChildByTag(100);
	Sprite * meigui = (Sprite*) this->getChildByTag(101);
	Sprite * btCircle = (Sprite*) this->getChildByTag(99);

	btCircle->setPosition(huaji->getPosition());

	if (std::abs(huaji->getPositionX() - meigui->getPositionX()) <20 && std::abs(huaji->getPositionY() - meigui->getPositionY()) < 20){
		huaji->setPosition(huaji->getPositionX(), huaji->getPositionY() + 30);
		Director::getInstance()->replaceScene(Stage3Scene::create());
	}


	if (huaji->getPositionY() < 10){
		GameOverScene* gmScene = (GameOverScene* ) GameOverScene::createScene();
		gmScene->lastStage = 2;
		Director::getInstance()->replaceScene(gmScene);
	}


	if (huaji->flying){
		huaji->setPositionY( huaji->getPositionY() + huaji->current_speed_y_pix_by_frame * 1);
		huaji->current_speed_y_pix_by_frame = huaji->current_speed_y_pix_by_frame + this->gravity_acc_by_squareframe * 1;
		if (huaji->current_speed_y_pix_by_frame < -abs(huaji->max_speed_y_pix_by_frame)){
			huaji->current_speed_y_pix_by_frame = -abs(huaji->max_speed_y_pix_by_frame);
		}

		huaji->setPositionX(huaji->getPositionX() + huaji->current_speed_x_pix_by_frame);
	}

	if (this->key_D_down){
		huaji->current_speed_x_pix_by_frame = huaji->max_speed_x_pix_by_frame;
		if (huaji->on_the_ground){
			huaji->setPositionX(huaji->getPositionX() + huaji->current_speed_x_pix_by_frame);
		}
	}
	if (this->key_A_down){
		huaji->current_speed_x_pix_by_frame = -huaji->max_speed_x_pix_by_frame;
		if (huaji->on_the_ground){
			huaji->setPositionX(huaji->getPositionX() + huaji->current_speed_x_pix_by_frame);
		}
	}

	if (huaji->on_the_ground){
		huaji->current_speed_y_pix_by_frame = 0;
		huaji->current_speed_x_pix_by_frame = 0;
	}


	if (huaji->getPositionX() < this->leftBoundary){
		huaji->setPositionX(leftBoundary);
		huaji->current_speed_x_pix_by_frame = 0;
	}
	if (huaji->getPositionX() > this->rightBoundary){
		huaji->setPositionX(rightBoundary);
		huaji->current_speed_x_pix_by_frame = 0;
	}

	this->updateIfHuajiIsfOnTheGroundAndResetHuajisYPosition();
}


void Stage2Scene::updateIfHuajiIsfOnTheGroundAndResetHuajisYPosition(){
	bool huajiOnOneGround = false;
	HuajiSprite* huaji = (HuajiSprite*)this->getChildByTag(100);
	int huajiWidth = huaji->getContentSize().width;
	int huajiHeight = huaji->getContentSize().height;
	for (int i = 0; i < 18; i++){
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
