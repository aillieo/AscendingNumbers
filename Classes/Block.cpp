#include "Block.h"

USING_NS_CC;
const float blockWidth = 128.0f;

// on "init" you need to initialize your instance
bool Block::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Sprite::init() )
    {
        return false;
    }
    
    this->setTexture("BLANK.png");
	setTextureRect(Rect(0,0,blockWidth,blockWidth));
	setColor(ccc3(0,255,0));

    label = LabelTTF::create("0", "Arial", 70);
    
	this->addChild(label, 1);

	//label->setAnchorPoint(Point(0.5,0.5));

    label->setPosition(blockWidth/2 , blockWidth/2 );

	//_pressable = true;

	isPressed = false;

    return true;
}

float Block::getContentWidth()
{
	return blockWidth;
}

void Block::setRow( int row )
{
	_row = row;
}

void Block::setCol( int col )
{
	_col = col;
}

int Block::getRow()
{
	return _row;
}

int Block::getCol()
{
	return _col;
}


void Block::setBlockValue(unsigned blockValue)
{

	_blockValue = blockValue;

	String* st = String::createWithFormat("%d",_blockValue);
	label->setString(st->_string);

	setBlockColor(_blockValue);

}

unsigned Block::getBlockValue()
{
	return _blockValue;
}

bool Block::isPressable()
{
	return _pressable;
}

void Block::setPressability( bool bIsPressable )
{
	_pressable = bIsPressable;
}

void Block::setBlockColor( unsigned blockValue )
{
	if (blockValue>25)
	{

		setColor(ccc3(255,0,0));

	}
	else
	{

		int r = (blockValue-1)*255/25;
		int g = (12.5-abs((int)(blockValue-1-12.5)))*255/12.5;
		int b = (26-blockValue)*255/25;

		setColor(ccc3(r,g,b));

	}

	return;

}

void Block::onPressed()
{
	isPressed = true;
	auto action = ScaleTo::create(0.2f,0.8);
	this->stopAllActions();
	this->runAction(action);
}

void Block::onPressCancelled()
{
	isPressed = false;
	auto action = ScaleTo::create(0.2f,1.0);
	this->stopAllActions();
	this->runAction(action);
}

void Block::onSelectionFinished( bool isLargestNumber )
{
	if (isLargestNumber)
	{
		isPressed = false;
		auto action1 = CallFunc::create([this](){setZOrder(100);});
		auto action2 = ScaleTo::create(0.1f,1.2);
		auto action3 = ScaleTo::create(0.1f,1.0);
		auto action4 = CallFunc::create([this](){setZOrder(3);});
		this->stopAllActions();
		this->runAction(Sequence::create(action1,action2,action3,action4,NULL));
	}
	else
	{
		isPressed = false;
		auto action = ScaleTo::create(0.2f,1.0);
		this->stopAllActions();
		this->runAction(action);
	}
}

void Block::onReduce()
{
	auto action1 = FadeOut::create(0.1f);
	auto action2 = FadeIn::create(0.1f);
	this->stopAllActions();
	this->runAction(Sequence::create(action1,action2,NULL));
}

