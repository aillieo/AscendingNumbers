#pragma once

#include "cocos2d.h"

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);



private:

	void skipTutorial(cocos2d::EventCustom* event);

	void showSetting(cocos2d::EventCustom* event);

	void showTutorial(cocos2d::EventCustom* event);

	void closeSetting(cocos2d::EventCustom* event);

	void showHelp(cocos2d::EventCustom* event);

	void closeHelp(cocos2d::EventCustom* event);

	void restartGame(cocos2d::EventCustom* event);



#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//响应按键
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);

	void itemYESCallback(cocos2d::Ref* pSender);

	void itemNOCallback(cocos2d::Ref* pSender);

#endif

};


