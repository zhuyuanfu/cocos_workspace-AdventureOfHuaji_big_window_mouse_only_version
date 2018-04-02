#include "CoverScene.h"
#include "Stage1Scene.h"
#include "PrologueScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* CoverScene::createScene()
{
	return CoverScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool CoverScene::init()
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
		CC_CALLBACK_1(CoverScene::menuCloseCallback, this));

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

	// create and initialize a label

	auto label = Label::createWithTTF("Cover", "fonts/Marker Felt.ttf", 24);
	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		// position the label on the center of the screen
		label->setPosition(Vec2(origin.x + label->getContentSize().width / 2,
			origin.y + visibleSize.height - label->getContentSize().height/2));

		// add the label as a child to this layer
		this->addChild(label, 1);
	}


	Sprite * coverSprite = Sprite::create("cover2_small.png");
	coverSprite->setPosition(visibleSize.width / 2, visibleSize.height * 70 / 100);
	coverSprite->setScale(1.5);
	this->addChild(coverSprite);


	Label * skipLabel = Label::createWithTTF("Start the story", "fonts/Marker Felt.ttf", 48);
	MenuItemLabel * menuItemSkip = MenuItemLabel::create(skipLabel, CC_CALLBACK_1(CoverScene::goToPrologueSceneCallback, this));
	Menu * middleMenu = Menu::create(menuItemSkip, nullptr);
	middleMenu->setPosition(visibleSize.width  / 2, visibleSize.height * 30 / 100);
	this->addChild(middleMenu);



	return true;
}


void CoverScene::menuCloseCallback(Ref* pSender)
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

void CoverScene::restartGameCallBack(Ref* pSender){
	Director::getInstance()->replaceScene(Stage1Scene::createScene());
}

void CoverScene::goToPrologueSceneCallback(Ref * pSender){
	Director::getInstance()->replaceScene(PrologueScene::createScene());
}