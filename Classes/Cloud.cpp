#include "Cloud.h"

USING_NS_CC;

bool Cloud::init()
{

	if ( !Sprite::init() )
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	_height = 100;

	setTexture("cloud.png");

	scheduleUpdate();

	return true;
}


void Cloud::update( float delta )
{
	if (getPositionX()>origin.x - 100)
	{
		this->setPositionX(getPositionX()-0.25);
	}
	else
	{
		setPosition(Vec2((origin.x + visibleSize.width + 100),_height + 128*(CCRANDOM_0_1()-0.5)));
	}
}

void Cloud::setHeight( float height )
{
	_height = height;
	this->setPositionY(_height);
}
