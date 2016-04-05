#include "GameSettingLayer.h"
#include "MultiLanguagePathGetter.h"

USING_NS_CC;


// on "init" you need to initialize your instance
bool GameSettingLayer::init()
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
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	/*
	auto label = LabelTTF::create("This is settingLayer", "Arial", 24);

	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width/2,
		origin.y + visibleSize.height - label->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(label, 1);
	*/

	//根据系统语言生成一个文件路径字符串
	std::string filename = MultiLanguagePathGetter::getPath() + "/SETTING.xml";
	auto dictionary = Dictionary::createWithContentsOfFile(filename.c_str());


	std::string str_resume = ((String*)dictionary->objectForKey("RESUME"))->getCString(); 
	std::string str_restart = ((String*)dictionary->objectForKey("RESTART"))->getCString(); 
	std::string str_sound_on = ((String*)dictionary->objectForKey("SOUND ON"))->getCString(); 
	std::string str_sound_off = ((String*)dictionary->objectForKey("SOUND OFF"))->getCString(); 
	std::string str_help = ((String*)dictionary->objectForKey("HELP"))->getCString(); 
	std::string str_tutorial = ((String*)dictionary->objectForKey("TUTORIAL"))->getCString(); 

	filename = MultiLanguagePathGetter::getPath() + "/font.fnt";



	auto resumeItem = MenuItemLabel::create(LabelBMFont::create(str_resume.c_str(), filename.c_str()),CC_CALLBACK_1(GameSettingLayer::resumeGame, this));
	auto restartItem = MenuItemLabel::create(LabelBMFont::create(str_restart, filename.c_str()),CC_CALLBACK_1(GameSettingLayer::restartGame, this));
	auto soundItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(GameSettingLayer::setSound, this),
													MenuItemLabel::create(LabelBMFont::create(str_sound_on, filename.c_str())),
													MenuItemLabel::create(LabelBMFont::create(str_sound_off, filename.c_str())),
													NULL);
	if (UserDefault::getInstance()->getBoolForKey("Sound",true))
	{
		soundItem->setSelectedIndex(0);
	}
	else
	{
		soundItem->setSelectedIndex(1);
	}



	auto helpItem = MenuItemLabel::create(LabelBMFont::create(str_help, filename.c_str()),CC_CALLBACK_1(GameSettingLayer::showHelpInfo, this));
	auto tutorialItem = MenuItemLabel::create(LabelBMFont::create(str_tutorial, filename.c_str()),CC_CALLBACK_1(GameSettingLayer::showTutorial, this));


	menu = Menu::create(resumeItem,restartItem,soundItem,helpItem,tutorialItem,NULL);
	menu->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
	menu->alignItemsVerticallyWithPadding(10);
	this->addChild(menu,1);

	menuEnter();


	return true;
}

void GameSettingLayer::resumeGame(cocos2d::Ref* pSender)
{
	EventCustom event = EventCustom("RESUME_GAME");
	_eventDispatcher->dispatchEvent(&event);
	menu->setEnabled(false);

}

void GameSettingLayer::restartGame( cocos2d::Ref* pSender )
{
	EventCustom event = EventCustom("RESTART_GAME");
	_eventDispatcher->dispatchEvent(&event);
}

void GameSettingLayer::setSound( cocos2d::Ref* pSender )
{

	if (((MenuItemToggle*)pSender)->getSelectedIndex() == 1)
	{

		UserDefault::getInstance()->setBoolForKey("Sound",false);
		log("off");

	}else if (((MenuItemToggle*)pSender)->getSelectedIndex() == 0)
	{

		UserDefault::getInstance()->setBoolForKey("Sound",true);
		log("on");

	}

	EventCustom event = EventCustom("SET_SOUND");
	_eventDispatcher->dispatchEvent(&event);

}

void GameSettingLayer::showHelpInfo( cocos2d::Ref* pSender )
{
	EventCustom event = EventCustom("SHOW_HELP");
	_eventDispatcher->dispatchEvent(&event);
}

void GameSettingLayer::showTutorial( cocos2d::Ref* pSender )
{
	EventCustom event = EventCustom("SHOW_TUTORIAL");
	_eventDispatcher->dispatchEvent(&event);
}

void GameSettingLayer::menuEnter()
{

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	auto items =  menu->getChildren();

	//std::vector<float> pYs = std::vector<float>();
	float pX = 0;

	for (int i = 0 ; i<5 ; i++)
	{
		//pYs.push_back(items.at(i)->getPositionY());
		float pY = items.at(i)->getPositionY();
		items.at(i)->setPositionY(visibleSize.height * (- 0.5));

		items.at(i)->stopAllActions();
		JumpTo* jt = JumpTo::create(0.4,Vec2(pX,pY),300 - 50* i,1);
		items.at(i)->runAction(jt);

	}





}

void GameSettingLayer::menuExit()
{

}



