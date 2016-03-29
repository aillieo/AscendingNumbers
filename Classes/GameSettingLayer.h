#pragma once

#include "cocos2d.h"

class GameSettingLayer : public cocos2d::Layer
{
public:

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// implement the "static create()" method manually
	CREATE_FUNC(GameSettingLayer);

	void resumeGame(cocos2d::Ref* pSender);

	void restartGame(cocos2d::Ref* pSender);

	void setSound(cocos2d::Ref* pSender);

	void showHelpInfo(cocos2d::Ref* pSender);

	void showTutorial(cocos2d::Ref* pSender);


};

