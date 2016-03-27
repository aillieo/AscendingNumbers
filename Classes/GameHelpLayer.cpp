#include "GameHelpLayer.h"
//#include "tinyxml2/tinyxml2.h"
#include "MultiLanguagePathGetter.h"

//using namespace tinyxml2;
USING_NS_CC;


// on "init" you need to initialize your instance
bool GameHelpLayer::init()
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

	
	//根据系统语言生成一个文件路径字符串
	std::string filename = MultiLanguagePathGetter::getPath() + "/HELP.xml";
	auto dictionary = Dictionary::createWithContentsOfFile(filename.c_str());


	std::string str_1 = ((String*)dictionary->objectForKey("1"))->getCString(); 
	std::string str_2 = ((String*)dictionary->objectForKey("2"))->getCString(); 
	std::string str_3 = ((String*)dictionary->objectForKey("3"))->getCString(); 
	std::string str_4 = ((String*)dictionary->objectForKey("4"))->getCString(); 
	std::string str_5 = ((String*)dictionary->objectForKey("5"))->getCString(); 
	std::string str_6 = ((String*)dictionary->objectForKey("6"))->getCString(); 


	filename = MultiLanguagePathGetter::getPath() + "/font.fnt";

	auto helpInfo = LabelBMFont::create((str_1 + "\n\n"+ str_2 + "\n"+str_3 + "\n"+str_4 + "\n"+str_5 + "\n\n"+str_6).c_str() , filename.c_str());
	helpInfo->setScale(42.0f/60.0f);
	
	helpInfo->setLineBreakWithoutSpace(false);

	helpInfo->setWidth(visibleSize.width * 60.0f / 42.0f);
	helpInfo->setAlignment(TextHAlignment::CENTER);

	helpInfo->setPosition(Vec2(origin.x + visibleSize.width * 0.5 + (0*visibleSize.width*18.0f/84.0f),
		origin.y + visibleSize.height/2 + visibleSize.height*0.05));
	this->addChild(helpInfo, 1);

	
	std::string str_back = ((String*)dictionary->objectForKey("BACK"))->getCString(); 

	auto skipItem = MenuItemLabel::create(LabelBMFont::create(str_back.c_str(), filename.c_str()),CC_CALLBACK_1(GameHelpLayer::closeHelp, this));
	auto menu = Menu::create(skipItem,NULL);
	menu->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height*0.05));
	this->addChild(menu,1000);

	return true;
}

void GameHelpLayer::closeHelp( cocos2d::Ref* pSender )
{

	EventCustom event = EventCustom("CLOSE_HELP");
	_eventDispatcher->dispatchEvent(&event);


}

