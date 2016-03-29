#pragma once

#include "cocos2d.h"

class Cloud : public cocos2d::Sprite
{
public:

	//初始化
	virtual bool init();  

	//create函数
	CREATE_FUNC(Cloud);

	void setHeight(float height);

private:

	float _height;

	void update(float delta);

	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;


};

