#include "PrologueScene.h"
#include "Stage1Scene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* PrologueScene::createScene()
{
	return PrologueScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool PrologueScene::init()
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
		CC_CALLBACK_1(PrologueScene::menuCloseCallback, this));

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

	auto label = Label::createWithTTF("Prologue", "fonts/Marker Felt.ttf", 24);
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

	auto retryLabel1 = Label::createWithTTF("Retry", "fonts/Marker Felt.ttf", 18);
	auto menuItemRetry = MenuItemLabel::create(retryLabel1, CC_CALLBACK_1(PrologueScene::restartGameCallBack, this));
	auto middleMenu = Menu::create(menuItemRetry, nullptr);
	middleMenu->setPosition(250, 150);
	//this->addChild(middleMenu);

	Label * skipLabel = Label::createWithTTF("Skip", "fonts/Marker Felt.ttf", 24);
	MenuItemLabel * menuItemSkip = MenuItemLabel::create(skipLabel,CC_CALLBACK_1(PrologueScene::goToStage1SceneCallback,this));
	Menu * topRightMenu = Menu::create(menuItemSkip,nullptr);
	topRightMenu->setPosition(visibleSize.width - skipLabel->getContentSize().width / 2, visibleSize.height - skipLabel->getContentSize().height / 2);
	this->addChild(topRightMenu);

	
	Label * line1 = Label::createWithTTF("Huaji and Xiaoguai once lived a happy life.", "fonts/Marker Felt.ttf", 36);
	Label * line2 = Label::createWithTTF("However, Baonu ruined everything.", "fonts/Marker Felt.ttf", 36);
	Label * line3 = Label::createWithTTF("He KIDNAPPED Xiaoguai.", "fonts/Marker Felt.ttf", 36);
	Label * line4 = Label::createWithTTF("Get Xiaoguai back!", "fonts/Marker Felt.ttf", 48);


	line1->setPosition(visibleSize.width / 2 , visibleSize.height * 70/100);
	line1->setOpacity(0);
	FadeIn * line1FadeInAction = FadeIn::create(3);
	TargetedAction * line1FadeIn = TargetedAction::create(line1,line1FadeInAction);

	line2->setPosition(visibleSize.width / 2 , visibleSize.height * 60 / 100);
	line2->setOpacity(0);
	FadeIn * line2FadeInAction = FadeIn::create(3);
	TargetedAction * line2FadeIn = TargetedAction::create(line2, line2FadeInAction);

	line3->setPosition(visibleSize.width / 2 , visibleSize.height * 50 / 100);
	line3->setOpacity(0);
	FadeIn * line3FadeInAction = FadeIn::create(3);
	TargetedAction * line3FadeIn = TargetedAction::create(line3, line3FadeInAction);

	//line4->setPosition(visibleSize.width / 2 , visibleSize.height * 30 / 100);
	line4->setOpacity(0);

	MenuItemLabel * menuItemStartGame = MenuItemLabel::create(line4, CC_CALLBACK_1(PrologueScene::goToStage1SceneCallback, this));
	Menu * startGameMenu = Menu::create(menuItemStartGame, nullptr);
	startGameMenu->setPosition(visibleSize.width / 2, visibleSize.height * 30 / 100);
	FadeIn * line4FadeInAction = FadeIn::create(3);
	TargetedAction * line4FadeIn = TargetedAction::create(line4, line4FadeInAction);


	Sequence * sequence = Sequence::create(line1FadeIn, line2FadeIn, line3FadeIn, line4FadeIn,NULL);
	this->runAction(sequence);

	this->addChild(line1);
	this->addChild(line2);
	this->addChild(line3);
	this->addChild(startGameMenu);

	return true;
}


void PrologueScene::menuCloseCallback(Ref* pSender)
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

void PrologueScene::restartGameCallBack(Ref* pSender){
	Director::getInstance()->replaceScene(Stage1Scene::createScene());
}

void PrologueScene::goToStage1SceneCallback(Ref * pSender){
	Director::getInstance()->replaceScene(Stage1Scene::createScene());
}