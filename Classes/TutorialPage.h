#pragma once

#include "cocos2d.h"
#include "Block.h"

class TutorialPage : public cocos2d::Sprite
{
public:

	//初始化
	virtual bool init(unsigned index);  

	//create函数
	static TutorialPage* create(unsigned index);

	//标签的vec
	//cocos2d::Vector<LabelTTF*> captions;

private:

	//文本标签
	cocos2d::LabelTTF* caption;

	cocos2d::Point getCenterPoint( int row, int col );
	cocos2d::Point basePoint;
	cocos2d::Vector<Block*> blocks;

	cocos2d::Sprite* finger;
};

