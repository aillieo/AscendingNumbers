#include "GameScene.h"
#include "BlocksLayer.h"
#include "GameSettingLayer.h"
#include "GameTutorialLayer.h"
#include "GameHelpLayer.h"
#include "MultiLanguagePathGetter.h"

const int settingLayerTag = 123;
const int blocksLayerTag = 12;
const int tutorialLayerTag = 18;
const int helpLayerTag = 199;
const int msgBoxTag = 129;

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////

	/*
	//设置菜单按钮
    settingItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GameScene::showSettingLayerCallback, this));
    
	settingItem->setPosition(Vec2(origin.x + visibleSize.width - settingItem->getContentSize().width/2 ,
                                origin.y + settingItem->getContentSize().height/2));

	settingItem->setVisible(false);

	// “减”菜单按钮
	reduceItem = MenuItemImage::create(
		"ButtonNormal_1.png",
		"ButtonSelected.png",
		CC_CALLBACK_1(GameScene::reduceCallback, this));

	reduceItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + reduceItem->getContentSize().height*2));

	reduceItem->setVisible(false);

	// finish按钮
	finishItem = MenuItemImage::create(
		"ButtonNormal.png",
		"ButtonSelected.png",
		CC_CALLBACK_1(GameScene::finishCallback, this));

	finishItem->setPosition(Vec2(origin.x + visibleSize.width * 0.75,
		origin.y + finishItem->getContentSize().height*2));

	finishItem->setVisible(false);

	// clear按钮
	clearItem = MenuItemImage::create(
		"ButtonNormal_2.png",
		"ButtonSelected.png",
		CC_CALLBACK_1(GameScene::clearCallback, this));

	clearItem->setPosition(Vec2(origin.x + visibleSize.width / 4,
		origin.y + clearItem->getContentSize().height*2));

	clearItem->setVisible(false);



    // create menu, it's an autorelease object
    auto menu = Menu::create(settingItem,reduceItem,finishItem,clearItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
	*/

	//创建背景
	auto background = Sprite::create("BLANK.png");
	background->setTextureRect(Rect(0,0,visibleSize.width,visibleSize.height));
	background->setColor(ccc3(60,125,255));
	background->setPosition(Vec2(origin.x + visibleSize.width/2 , origin.y + visibleSize.height / 2));
	this->addChild(background);



	//对skip的监听
	auto listenerSKT = EventListenerCustom ::create("SKIP_TUTORIAL",CC_CALLBACK_1(GameScene::skipTutorial, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerSKT,this);

	//对showTutorial的监听
	auto listenerST = EventListenerCustom ::create("SHOW_TUTORIAL",CC_CALLBACK_1(GameScene::showTutorial, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerST,this);

	//对showSetting的监听
	auto listenerSS = EventListenerCustom ::create("SHOW_SETTING",CC_CALLBACK_1(GameScene::showSetting, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerSS,this);

	//对closeSetting的监听
	auto listenerCS = EventListenerCustom ::create("RESUME_GAME",CC_CALLBACK_1(GameScene::closeSetting, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerCS,this);

	//对showHelp的监听
	auto listenerSH = EventListenerCustom ::create("SHOW_HELP",CC_CALLBACK_1(GameScene::showHelp, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerSH,this);

	//对closeHelp的监听
	auto listenerCH = EventListenerCustom ::create("CLOSE_HELP",CC_CALLBACK_1(GameScene::closeHelp, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerCH,this);

	//对restart的监听
	auto listenerRST = EventListenerCustom ::create("RESTART_GAME",CC_CALLBACK_1(GameScene::restartGame, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerRST,this);


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//对按键事件的监听
	auto listenerkeyPad = EventListenerKeyboard::create();
	listenerkeyPad->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
#endif



	if (UserDefault::getInstance()->getBoolForKey("needTutorial",true))
	{
		//根据是否第一次进入游戏 显示教程
		auto tutorialLayer = GameTutorialLayer::create();
		this->addChild(tutorialLayer, 3,tutorialLayerTag);
		UserDefault::getInstance()->setBoolForKey("needTutorial",false);
	}
	else
	{
		//创建BlockLayer
		auto blocksLayer = BlocksLayer::create();
		//blocksLayer->setPosition(Vec2(origin.x,origin.y));
		//blocksLayer->setPosition(Vec2(origin.x + visibleSize.width/2 , origin.y + visibleSize.height / 2));
		this->addChild(blocksLayer, 0,blocksLayerTag);
	}
	
    return true;
}








void GameScene::skipTutorial( cocos2d::EventCustom* event )
{
	this->removeChildByTag(tutorialLayerTag);
	
	if (nullptr != this->getChildByTag(settingLayerTag))
	{
		((GameSettingLayer*)(this->getChildByTag(settingLayerTag)))->setVisible(true);

	}


	if (nullptr == this->getChildByTag(blocksLayerTag))
	{

		//创建BlockLayer
		auto blocksLayer = BlocksLayer::create();
		//blocksLayer->setPosition(Vec2(origin.x,origin.y));
		//blocksLayer->setPosition(Vec2(origin.x + visibleSize.width/2 , origin.y + visibleSize.height / 2));
		this->addChild(blocksLayer, 0,blocksLayerTag);

	}


}

void GameScene::showSetting( cocos2d::EventCustom* event )
{

	((BlocksLayer*)(this->getChildByTag(blocksLayerTag)))->isTouchEventValid = false;
	//((BlocksLayer*)(this->getChildByTag(blocksLayerTag)))->setTouchEnabled(false);

	//this->getChildByTag(blocksLayerTag)->setTouchEnabled(false);
	this->getChildByTag(blocksLayerTag)->setVisible(false);

	auto gameSettingLayer = GameSettingLayer::create();
	this->addChild(gameSettingLayer,1,settingLayerTag);
	
}

void GameScene::showTutorial( cocos2d::EventCustom* event )
{

	if (nullptr != this->getChildByTag(settingLayerTag))
	{
		((GameSettingLayer*)(this->getChildByTag(settingLayerTag)))->setVisible(false);

	}

	auto gameTutorialLayer = GameTutorialLayer::create();
	this->addChild(gameTutorialLayer,10,tutorialLayerTag);
}

void GameScene::closeSetting( cocos2d::EventCustom* event )
{

	auto action1 = DelayTime::create(0.3);
	auto action2 = CallFunc::create([&](){

		this->removeChildByTag(settingLayerTag);

		this->getChildByTag(blocksLayerTag)->setVisible(true);
		//((BlocksLayer*)(this->getChildByTag(blocksLayerTag)))->setTouchEnabled(true);
		((BlocksLayer*)(this->getChildByTag(blocksLayerTag)))->isTouchEventValid = true;

	});

	auto action3 = Sequence::create(action1, action2, NULL);
	runAction(action3);


	auto action4 = MoveBy::create(0.3,Vec2(0,-1280));
	getChildByTag(settingLayerTag)->runAction(action4);




}

void GameScene::showHelp( cocos2d::EventCustom* event )
{
	
	((GameSettingLayer*)(this->getChildByTag(settingLayerTag)))->setVisible(false);

	auto gameHelpLayer = GameHelpLayer::create();
	this->addChild(gameHelpLayer,10,helpLayerTag);
}

void GameScene::closeHelp( cocos2d::EventCustom* event )
{
	this->removeChildByTag(helpLayerTag);

	((GameSettingLayer*)(this->getChildByTag(settingLayerTag)))->setVisible(true);

	
}

void GameScene::restartGame( cocos2d::EventCustom* event )
{

	this->removeChildByTag(settingLayerTag);
	this->removeChildByTag(blocksLayerTag);

	auto blocksLayer = BlocksLayer::create();
	this->addChild(blocksLayer, 0,blocksLayerTag);

}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void GameScene::onKeyReleased( EventKeyboard::KeyCode keycode, Event* event )
{



	if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)  

	{

		if (getChildByTag(helpLayerTag) != nullptr)
		{
			EventCustom event = EventCustom("CLOSE_HELP");
			_eventDispatcher->dispatchEvent(&event);
			return;
		}

		if (getChildByTag(tutorialLayerTag) != nullptr)
		{
			EventCustom event = EventCustom("SKIP_TUTORIAL");
			_eventDispatcher->dispatchEvent(&event);
			return;
		}



		if (getChildByTag(settingLayerTag) != nullptr)
		{
			EventCustom event = EventCustom("RESUME_GAME");
			_eventDispatcher->dispatchEvent(&event);
			return;
		}

		if (getChildByTag(msgBoxTag) != nullptr)
		{
			this->removeChildByTag(msgBoxTag);
			return;
		}


		

		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		auto msgBox = Sprite::create("BLANK.png");
		msgBox->setTextureRect(Rect(0,0,0.82*visibleSize.width,0.2* visibleSize.height));
		addChild(msgBox,1000,msgBoxTag);
		msgBox->setColor(ccc3(60,125,255));
		msgBox->setPosition(origin.x+ 0.5*visibleSize.width, origin.y + 0.5* visibleSize.height);


		std::string filename = MultiLanguagePathGetter::getPath() + "/MSGBOX.xml";
		auto dictionary = Dictionary::createWithContentsOfFile(filename.c_str());
		std::string str_title = ((String*)dictionary->objectForKey("EXIT?"))->getCString(); 
		std::string str_yes = ((String*)dictionary->objectForKey("YES"))->getCString(); 
		std::string str_no = ((String*)dictionary->objectForKey("NO"))->getCString(); 
		filename = MultiLanguagePathGetter::getPath() + "/font.fnt";

		auto label = LabelBMFont::create(str_title.c_str(),filename);
		msgBox->addChild(label);
		//label->setPosition(msgBox->getPosition());
		label->setPosition(Vec2(0.5*msgBox->getContentSize().width,0.6*msgBox->getContentSize().height));

		auto itemYES = MenuItemLabel::create(LabelBMFont::create(str_yes.c_str(),filename),CC_CALLBACK_1(GameScene::itemYESCallback,this));
		auto itemNO = MenuItemLabel::create(LabelBMFont::create(str_no.c_str(),filename),CC_CALLBACK_1(GameScene::itemNOCallback,this));
		auto menu = Menu::create(itemYES,itemNO,NULL);
		msgBox->addChild(menu);
		menu->setPosition(Vec2(0,0));
		itemYES->setPosition(Vec2(0.2*msgBox->getContentSize().width,0.2*msgBox->getContentSize().height));
		itemYES->setScale(40.0f/60.0f);
		itemNO->setPosition(Vec2(0.8*msgBox->getContentSize().width,0.2*msgBox->getContentSize().height));
		itemNO->setScale(40.0f/60.0f);

		//Director::getInstance()->end();



	}
	else if (keycode == EventKeyboard::KeyCode::KEY_MENU)
	{
		if (getChildByTag(settingLayerTag) == nullptr)
		{
			EventCustom event = EventCustom("SHOW_SETTING");
			_eventDispatcher->dispatchEvent(&event);
		}
		
	}



}

void GameScene::itemYESCallback( cocos2d::Ref* pSender )
{
	Director::getInstance()->end();
}

void GameScene::itemNOCallback( cocos2d::Ref* pSender )
{

	this->removeChildByTag(msgBoxTag);

}

#endif