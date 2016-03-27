#pragma once

#include "cocos2d.h"
#include "Block.h"

class BlocksLayer : public cocos2d::Layer
{
public:

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
   
    // implement the "static create()" method manually
    CREATE_FUNC(BlocksLayer);

	//���ڴ����¼�
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused);
	virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused);

	//�����¼���Ч
	bool isTouchEventValid;

	//ʹ��reduce��ť
	void superReduce();

	//ʹ��finish��ť
	void finishSelection();

	//ʹ��clear��ť
	void clearSelection();

	//��ǰ��������������sum
	int currentSum;

	//������Ч������
	void setSound(cocos2d::EventCustom* event);



private:

	//�ж���Ч�Ƿ���
	bool isSoundEnabled;

	//��ʼ����ʱѡ���block ֻ�е����º��ɿ���block��ͬʱ �Ż��block���в���
	Block* touchBeganBlock;

	//��ʼ����ʱ�ĵ� �����жϻ�������
	cocos2d::Point touchBeganPoint; 


	//ѡ������ǵ�����
	bool selectingAscending;

	//��Ϸ�Ƿ��Ѿ���ʼ
	bool hasGameBegan;

	//�������� �����к� �к� ���������ֵ
	void createBlock(int row, int col, unsigned blockValue);

	//λ��ָ������λ�õ�block�����ĵ�
	cocos2d::Point getCenterPoint(int row, int col);

	//�����õ��block����
	Block* getBlockContainingPoint(cocos2d::Point point);


	//������Ч
	void playSound(const std::string filename);



	//���½ǵĻ�׼�� holder�����½� �������½ǵķ����м�϶
	cocos2d::Point basePoint;

	//�洢�����������е�blocks
	cocos2d::Vector<Block*> blocks; 

	//�洢ѡ�е�blocks
	cocos2d::Vector<Block*> blocksPressed; 


	void checkSwipeAction(cocos2d::Point point);



	//�������ֹ��������ֵ
	int _maxNumberEver;
	cocos2d::LabelTTF* _labelMaxNumberEver;

	//��ǰʣ��ġ������Ĵ���
	int _reduceChancesLeft;
	cocos2d::LabelTTF* _labelReduceChancesLeft;

	void firstStep();

	void updateSum(int newSum);

	//�����������ֹ������ֵ
	void setNewRecord(int newRecord);

	//���õ�ǰʣ��ġ����Ļ�������
	void setReduceChancesLeft(int reduceChancesLeft);




};

