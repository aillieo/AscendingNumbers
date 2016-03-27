#pragma once

#include "cocos2d.h"

class Cloud : public cocos2d::Sprite
{
public:

	//³õÊ¼»¯
	virtual bool init();  

	//createº¯Êý
	CREATE_FUNC(Cloud);

	void setHeight(float height);

private:

	float _height;

	void update(float delta);

	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;


};

