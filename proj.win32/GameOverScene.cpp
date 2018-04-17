#include "GameOverScene.h"
#include "Stage1Scene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* GameOverScene::createScene()
{
	return GameOverScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameOverScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(GameOverScene::menuCloseCallback, this));

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
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	auto label = Label::createWithTTF("Abandon All Hope", "fonts/Marker Felt.ttf", 48);
	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		// position the label on the center of the screen
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height/2 ));
		ScaleTo * scaleTo = ScaleTo::create(2,1.2);
		label->runAction(scaleTo);
		// add the label as a child to this layer
		this->addChild(label, 1);
	}

	auto retryLabel1 = Label::createWithTTF("Restart", "fonts/Marker Felt.ttf",36);
	auto menuItemRestart = MenuItemLabel::create(retryLabel1, CC_CALLBACK_1(GameOverScene::restartGameCallBack, this));
	menuItemRestart->setPositionY(-48);

	auto retryStage = Label::createWithTTF("Retry current stage", "fonts/Marker Felt.ttf", 36);
	auto menuItemRetryStage = MenuItemLabel::create(retryStage, CC_CALLBACK_1(GameOverScene::gotoLastStage,this));

	auto middleMenu = Menu::create(menuItemRestart, menuItemRetryStage, nullptr);
	middleMenu->setPosition(visibleSize.width / 2, visibleSize.height * 30 / 100);
	this->addChild(middleMenu);

	return true;
}

void GameOverScene::gotoLastStage(Ref* pSender){
	Scene * stageScene;
	switch (this->lastStage){
	case 1:
		stageScene = Stage1Scene::createScene();
		break;
	case 2:
		stageScene = Stage2Scene::createScene();
		break;
	case 3:
		stageScene = Stage3Scene::createScene();
		break;
	case 4:
		stageScene = Stage4Scene::createScene();
		break;
	case 5:
		stageScene = Stage5Scene::createScene();
		break;
	default:
		stageScene = Stage1Scene::createScene();

	}
	Director::getInstance()->replaceScene(stageScene);
}

void GameOverScene::menuCloseCallback(Ref* pSender)
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

void GameOverScene::restartGameCallBack(Ref* pSender){
	Director::getInstance()->replaceScene(Stage1Scene::createScene());
}