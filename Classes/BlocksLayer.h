#pragma once

#include "cocos2d.h"
#include "FigureBlock.h"

class BlocksLayer : public cocos2d::Layer
{
public:

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
   
    // implement the "static create()" method manually
    CREATE_FUNC(BlocksLayer);

	//关于触摸事件
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused);
	virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused);

	//触摸事件有效
	bool isTouchEventValid;

	//使用reduce按钮
	void superReduce();

	//使用finish按钮
	void finishSelection();

	//使用clear按钮
	void clearSelection();

	//当前步操作产生的新sum
	int currentSum;

	//关于音效的设置
	void setSound(cocos2d::EventCustom* event);



private:


	//方格阵的背景
	cocos2d::Sprite* holder;


	//初始化方格阵
	void initBlocks();


	//判断音效是否开启
	bool isSoundEnabled;

	//开始按下时选择的block 只有当按下和松开的block相同时 才会对block进行操作
	FigureBlock* touchBeganBlock;

	//开始按下时的点 用来判断滑动方向
	cocos2d::Point touchBeganPoint; 


	//选择的数是递增的
	bool selectingAscending;

	//游戏是否已经开始
	bool hasGameBegan;
	//游戏是否已经结束
	bool isGameOver;

	//创建方块 根据行号 列号 方格的数字值
	void createBlock(int row, int col, unsigned blockValue);

	//位于指定行列位置的block的中心点
	cocos2d::Point getCenterPoint(int row, int col);

	//包含该点的block对象
	FigureBlock* getBlockContainingPoint(cocos2d::Point point);


	//播放音效
	void playSound(const std::string filename);

	//播放背景音乐
	void playBackgroundMusic();


	//左下角的基准点 holder的左下角 距离左下角的方格有间隙
	cocos2d::Point basePoint;

	//存储格子阵中所有的blocks
	cocos2d::Vector<FigureBlock*> blocks; 

	//存储选中的blocks
	cocos2d::Vector<FigureBlock*> blocksPressed; 


	void checkSwipeAction(cocos2d::Point point);



	//曾经出现过的最大数值
	int _maxNumberEver;
	cocos2d::LabelTTF* _labelMaxNumberEver;

	//当前剩余的“减”的次数
	int _reduceChancesLeft;
	cocos2d::LabelTTF* _labelReduceChancesLeft;

	void firstStep();

	void updateSum(int newSum);

	//设置曾经出现过的最大值
	void setNewRecord(int newRecord);

	//设置当前剩余的“减的机会数”
	void setReduceChancesLeft(int reduceChancesLeft);



	//检查是否已游戏结束
	bool checkEnding();


	//播放游戏结束动画
	void onGameOver();

};

