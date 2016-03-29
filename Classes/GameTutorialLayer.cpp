#include "GameTutorialLayer.h"
#include "MultiLanguagePathGetter.h"


USING_NS_CC;


// on "init" you need to initialize your instance
bool GameTutorialLayer::init()
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

	std::string filename = MultiLanguagePathGetter::getPath() + "/TUTORIAL.xml";
	auto dictionary = Dictionary::createWithContentsOfFile(filename.c_str());
	filename = MultiLanguagePathGetter::getPath() + "/font.fnt";


	std::string str_title = ((String*)dictionary->objectForKey("GAME TUTORIAL"))->getCString(); 
	auto label = LabelBMFont::create(str_title.c_str(), filename.c_str());

	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width/2,
		origin.y + visibleSize.height - label->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(label, 1);


	//跳过教程的按钮
	std::string str_end = ((String*)dictionary->objectForKey("END TUTORIAL"))->getCString(); 
	auto skipItem = MenuItemLabel::create(LabelBMFont::create(str_end.c_str(), filename.c_str()),CC_CALLBACK_1(GameTutorialLayer::skipTutorial, this));
	auto menu = Menu::create(skipItem,NULL);
	menu->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + label->getContentSize().height));
	this->addChild(menu,1000);

	//页面的容器
	container = Node::create();
	addChild(container,2);
	container->setAnchorPoint(Point(0,0));
	//container->setPosition(Vec2(origin.x + visibleSize.width*0.5,origin.y + visibleSize.height*0.5));
	container->setPosition(origin);
	_pageIndex = 0;
	
	for (int i = 0;i<4;i++)
	{
		//小点
		auto pointt = Sprite::create("BLANK.png");
		pointt->setTextureRect(Rect(0,0,10,10));
		addChild(pointt,9);
		pointt->setPosition(Vec2(origin.x+visibleSize.width*0.41+i*visibleSize.width*0.06,origin.y+visibleSize.height*0.15));
		pageIndicators.pushBack(pointt);

		//页面
		auto page = TutorialPage::create(i);
		container->addChild(page,2);

		page->setPosition(Vec2(origin.x +visibleSize.width*0.5 + i*visibleSize.width,origin.y + visibleSize.height*0.5));
		//page->setPosition(Vec2(origin.x +visibleSize.width*0 + i*visibleSize.width,origin.y + visibleSize.height*0));

	}

	
	setPage();


	///////////////////////////////////////////////////////






	//对触摸事件的监听
	auto touchListener=EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan=CC_CALLBACK_2(GameTutorialLayer::onTouchBegan,this);
	touchListener->onTouchMoved=CC_CALLBACK_2(GameTutorialLayer::onTouchMoved,this);
	touchListener->onTouchEnded=CC_CALLBACK_2(GameTutorialLayer::onTouchEnded,this);
	touchListener->onTouchCancelled=CC_CALLBACK_2(GameTutorialLayer::onTouchCancelled,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,this);

	return true;
}

void GameTutorialLayer::skipTutorial( cocos2d::Ref* pSender )
{

	EventCustom event = EventCustom("SKIP_TUTORIAL");
	_eventDispatcher->dispatchEvent(&event);
	log("skip");

}

void GameTutorialLayer::setPage()
{

	//_pageIndex = pageIndex;

	currentContainerX = container->getPositionX();
	
	log("setpi pi = %d",_pageIndex);
	
	for (Sprite* pointt: pageIndicators)
	{
		
		if (pageIndicators.at(_pageIndex) == pointt)
		{
		
			pointt->setColor(ccc3(255,255,255));
		}
		else
		{
			pointt->setColor(ccc3(113,143,255));
		}
	
	}


}

void GameTutorialLayer::moveToPage()
{

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//move
	auto move1 = MoveTo::create(0.1f,Vec2(_pageIndex * visibleSize.width *(-1),container->getPositionY()));


	auto action2 = CallFunc::create([&](){setPage();});
	//setPage(_pageIndex);

	auto seq = Sequence::create(move1,action2,NULL);
	container->runAction(seq);

}

bool GameTutorialLayer::onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *unused )
{

	container->stopAllActions();
	touchStartingPointX = touch->getLocation().x;

	log("touch began pi = %d",_pageIndex);


	X_v_begin = touchStartingPointX;
	X_v_end = touchStartingPointX;

	return true;
}

void GameTutorialLayer::onTouchMoved( cocos2d::Touch *touch, cocos2d::Event *unused )
{

	X_v_begin = X_v_end;
	X_v_end = touch->getLocation().x;

	float X_v = X_v_end - X_v_begin;
	if (((_pageIndex == 0 )&&( X_v > 0))||((_pageIndex == 3 )&&( X_v < 0)))
	{
		X_v = X_v * 0.2;
	}


	container->setPositionX(container->getPositionX() + X_v );


}

void GameTutorialLayer::onTouchEnded( cocos2d::Touch *touch, cocos2d::Event *unused )
{


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	unsigned tmp = _pageIndex;
	float X_v = X_v_end - X_v_begin;

	currentContainerX = container->getPositionX();

	
	if (X_v < -10)
	{
		if (tmp!=3)
		{
			tmp ++ ;
		}
	}
	else if (X_v >10)
	{
		if (tmp!=0)
		{
			tmp -- ;
		}

	}
	else
	{
		for (unsigned u = 0; u<4; u++)
		{
			if (abs(currentContainerX + u* visibleSize.width)<visibleSize.width * 0.5)
			{
				tmp = u;
			}
		}

		if (currentContainerX > visibleSize.width * 0.5)
		{
			tmp = 0;
		}
		else if (currentContainerX < visibleSize.width * (-3.5))
		{
			tmp = 3;
		}
	}







	//判断位置
	_pageIndex = tmp;
	moveToPage();
	//log("touch end pi = %d",_pageIndex);
}

void GameTutorialLayer::onTouchCancelled( cocos2d::Touch *touch, cocos2d::Event *unused )
{

}







