#pragma once

#include "cocos2d.h"

class GameHelpLayer : public cocos2d::Layer
{
public:

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// implement the "static create()" method manually
	CREATE_FUNC(GameHelpLayer);

	//跳过教程
	void closeHelp(cocos2d::Ref* pSender);


};

