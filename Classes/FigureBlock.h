#pragma once

#include "cocos2d.h"

class FigureBlock : public cocos2d::Sprite
{
public:

	//初始化
    virtual bool init();  
    
	//create函数
	CREATE_FUNC(FigureBlock);

	//获取图片宽度 高度和宽度相等
	static float getContentWidth();

	//行号 列号 的读写
	void setRow(int row);
	void setCol(int col);
	int getRow();
	int getCol();

	//方块上的数字值 读写
	void setBlockValue(unsigned blockValue);
	unsigned getBlockValue();

	//方块是否可按
	bool isPressable();
	//设置方块是否可按
	void setPressability(bool bIsPressable);

	//方块是否已被按下
	bool isPressed;

	//当被按下时
	void onPressed();

	//当被取消按下时
	void onPressCancelled();

	//当选择完成时变成非按下的状态
	void onSelectionFinished(bool isLargestNumber);

	//使用reduce时
	void onReduce();

	//初始化位置
	void initPosition(cocos2d::Point p );

private:

	//行号 列号
	int _row;
	int _col;


	//方块上的数字值
	unsigned _blockValue;

	//方块上数字的标签
	cocos2d::LabelTTF* label;

	//是否可按
	bool _pressable;

	//设置格子的颜色
	void setBlockColor(unsigned blockValue);

	//位置坐标
	cocos2d::Point _initPos;
};

