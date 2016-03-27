#pragma once

#include "cocos2d.h"

class Block : public cocos2d::Sprite
{
public:

	//��ʼ��
    virtual bool init();  
    
	//create����
	CREATE_FUNC(Block);

	//��ȡͼƬ��� �߶ȺͿ�����
	static float getContentWidth();

	//�к� �к� �Ķ�д
	void setRow(int row);
	void setCol(int col);
	int getRow();
	int getCol();

	//�����ϵ�����ֵ ��д
	void setBlockValue(unsigned blockValue);
	unsigned getBlockValue();

	//�����Ƿ�ɰ�
	bool isPressable();
	//���÷����Ƿ�ɰ�
	void setPressability(bool bIsPressable);

	//�����Ƿ��ѱ�����
	bool isPressed;

	//��������ʱ
	void onPressed();

	//����ȡ������ʱ
	void onPressCancelled();

	//��ѡ�����ʱ��ɷǰ��µ�״̬
	void onSelectionFinished(bool isLargestNumber);

	//ʹ��reduceʱ
	void onReduce();


private:

	//�к� �к�
	int _row;
	int _col;


	//�����ϵ�����ֵ
	unsigned _blockValue;

	//���������ֵı�ǩ
	cocos2d::LabelTTF* label;

	//�Ƿ�ɰ�
	bool _pressable;

	//���ø��ӵ���ɫ
	void setBlockColor(unsigned blockValue);
};

