#pragma once

#include "cocos2d.h"
#include "Block.h"

class TutorialPage : public cocos2d::Sprite
{
public:

	//��ʼ��
	virtual bool init(unsigned index);  

	//create����
	static TutorialPage* create(unsigned index);

	//��ǩ��vec
	//cocos2d::Vector<LabelTTF*> captions;

private:

	//�ı���ǩ
	cocos2d::LabelBMFont* caption;

	cocos2d::Point getCenterPoint( int row, int col );
	cocos2d::Point basePoint;
	cocos2d::Vector<Block*> blocks;

	cocos2d::Sprite* finger;
};

