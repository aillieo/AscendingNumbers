#pragma once

#include "cocos2d.h"
#include "TutorialPage.h"

class GameTutorialLayer : public cocos2d::Layer
{
public:

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// implement the "static create()" method manually
	CREATE_FUNC(GameTutorialLayer);

	//跳过教程
	void skipTutorial(cocos2d::Ref* pSender);

private:

	//显示当前是第x页 的进度小点 共4个
	cocos2d::Vector<cocos2d::Sprite*> pageIndicators;

	//各个页面 共4个
	//cocos2d::Vector<TutorialPage*> pages;

	//容器
	cocos2d::Node* container;

	//当前的进度 是第x页
	unsigned _pageIndex;

	//设置页面
	void setPage();

	//移动至页面
	void moveToPage();

	//关于触摸事件
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused);
	virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused);

	//触摸起始点的x坐标
	float touchStartingPointX;

	//当前container的x坐标
	float currentContainerX;

	
	
};

