#include "BlocksLayer.h"
#include "GameScene.h"
#include "Cloud.h"
#include "MultiLanguagePathGetter.h"

USING_NS_CC;

const float gapSize = 0.0f;

const int matrix_height = 5;
const int matrix_width = 5;


// on "init" you need to initialize your instance
bool BlocksLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    

	isSoundEnabled = UserDefault::getInstance()->getBoolForKey("Sound",true);


	playSound("NewGame.wav");
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("NewGame.wav",false);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	hasGameBegan = false;
	isTouchEventValid = true;

	//����������ı��� 430x430 �������ӿ�֮���϶5
	auto holder = Sprite::create("BLANK.png");
	holder->setTextureRect(Rect(0,0,640,640));
	holder-> setPosition(origin.x + visibleSize.width / 2 , origin.y + holder->getContentSize().height/2 );
	holder->setOpacity(0);
	this->addChild(holder,0);

	//��ȡbasePoint
	basePoint = Point(holder->getPositionX() - holder->getContentSize().width /2 ,holder->getPositionY() - holder->getContentSize().height /2 );

	//���ݵ�ǰʱ�� �������������
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	struct tm * tm;
	time_t time_sec ;
	time_sec = nowTimeval.tv_sec;
	tm = localtime(&time_sec);
	int nSec = tm->tm_sec;
	int nMin = tm->tm_min;
	int nHour = tm->tm_hour;
	int nDay = tm->tm_yday;
	int nSeed = ((nDay*24+nHour)*60+nMin)*60 + nSec ;
	//log("%d", nSeed);
	srand(nSeed); 

	//����2��������� ��3���� ��1���ĸ���
	int rest1 = (matrix_height*matrix_width)/4;
	int rest3 = (matrix_height*matrix_width)/4;

	//��ʼ������ 
	blocks = Vector<Block*>(matrix_height * matrix_width); 
	for (int row = 0; row < matrix_height; row++) 
	{
		for (int col = 0; col < matrix_width; col++) 
		{
			unsigned blockValue = 3;

			if ((col + row)%2 == 0)
			{
				blockValue = 2;
			}
			else
			{
				
				//�������1��3 ��ȷ��1��3�ĸ���
				if(CCRANDOM_0_1() > 0.5f )
				{
				
					if (rest1>0)
					{
						blockValue = 1;
						rest1--;
					}
					else
					{
						blockValue = 3;
						rest3--;
					}
					
				}
				else
				{


					
					if (rest3>0)
					{
						blockValue = 3;
						rest3--;
					}
					else
					{
						blockValue = 1;
						rest1--;
					}


				}

			}

			createBlock(row, col, blockValue );

		}
	}
	

	

	
	//Ʈ������
	auto cloud1 = Cloud::create();
	cloud1->setPositionX(origin.x);
	cloud1->setHeight(origin.y + 0.33 *holder->getContentSize().height);
	this->addChild(cloud1,2);
	auto cloud2 = Cloud::create();
	cloud1->setPositionX(origin.x + visibleSize.width * 0.5);
	cloud2->setHeight(origin.y + 0.66 *holder->getContentSize().height);
	this->addChild(cloud2,2);



	//�Դ����¼��ļ���
	auto touchListener=EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan=CC_CALLBACK_2(BlocksLayer::onTouchBegan,this);
	touchListener->onTouchMoved=CC_CALLBACK_2(BlocksLayer::onTouchMoved,this);
	touchListener->onTouchEnded=CC_CALLBACK_2(BlocksLayer::onTouchEnded,this);
	touchListener->onTouchCancelled=CC_CALLBACK_2(BlocksLayer::onTouchCancelled,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,this);

	//����Ч���õļ���
	auto listenerSS = EventListenerCustom ::create("SET_SOUND",CC_CALLBACK_1(BlocksLayer::setSound, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerSS,this);

    return true;
}

void BlocksLayer::createBlock(int row, int col, unsigned blockValue)
{
	auto tmpBlock = Block::create();
	tmpBlock->setPosition(getCenterPoint(row,col));
	tmpBlock->setBlockValue(blockValue);
	this->addChild(tmpBlock,3);

	blocks.insert(row * matrix_width + col , tmpBlock);
}

cocos2d::Point BlocksLayer::getCenterPoint( int row, int col )
{
	float x = basePoint.x + ( Block::getContentWidth() + gapSize )*(row + 1) - Block::getContentWidth()/2;
	float y = basePoint.y + ( Block::getContentWidth() + gapSize )*(col + 1) - Block::getContentWidth()/2;
	return Point(x,y);
}

Block* BlocksLayer::getBlockContainingPoint( cocos2d::Point point )
{

    Rect rect = Rect(0, 0, 0, 0);    

    for (Block* blk : blocks)
	{
		rect.origin.x = blk->getPositionX() - (blk->getContentSize().width / 2);
		rect.origin.y = blk->getPositionY() - (blk->getContentSize().height / 2);
		rect.size = blk->getContentSize();
		if (rect.containsPoint(point)) 
		{
			return blk;
        }
    }    
    return nullptr;
}


bool BlocksLayer::onTouchBegan( Touch *touch, Event *unused )
{

	if (!isTouchEventValid)
	{
		return false;
	}

	touchBeganPoint = touch->getLocation();
	Block* tmp = getBlockContainingPoint(touchBeganPoint);
	if (nullptr == tmp)
	{
		touchBeganBlock = nullptr;
	}

	touchBeganBlock = tmp;

	return true;

}


void BlocksLayer::onTouchMoved( Touch *touch, Event *unused )
{


}


void BlocksLayer::onTouchEnded( Touch *touch, Event *unused )
{
	if (!isTouchEventValid)
	{
		return;
	}


	//���û�а��ڸ�����
	auto point = touch->getLocation();
	Block* tmp = getBlockContainingPoint(point);
	if (nullptr == tmp)
	{
		checkSwipeAction(point);
		return;
	}

	//����ɿ��ĸ��ӺͰ��µĸ��Ӳ�ͬ
	if (tmp != touchBeganBlock)
	{
		checkSwipeAction(point);
		return;
	}




	//��������Ѱ��ĸ�����
	if (tmp->isPressed)
	{
		if((tmp==blocksPressed.back())||(!selectingAscending))
		{
			//tmp->isPressed = false;
			tmp->onPressCancelled();
			blocksPressed.eraseObject(tmp);

		}


		return;

	}



	//ѡ��һ����ʱ�� ֻ����1
	if ((blocksPressed.empty())&&(tmp->getBlockValue()==1))
	{
		blocksPressed.pushBack(tmp);
		tmp->onPressed();

		return;
	}



	//ѡ�ڶ�����ʱ�� ������1��2
	if (blocksPressed.size()==1)
	{
		if (tmp->getBlockValue()==1)
		{
			selectingAscending = false;
			blocksPressed.pushBack(tmp);
			tmp->onPressed();;
		}
		else if (tmp->getBlockValue()==2)
		{
			selectingAscending = true;
			blocksPressed.pushBack(tmp);
			tmp->onPressed();

		}


		return;
		

	}


	//�ӵ�������ʼ
	if (blocksPressed.size()>1)
	{

		if ((selectingAscending)&&(tmp->getBlockValue()== blocksPressed.back()->getBlockValue()+1))
		{
			blocksPressed.pushBack(tmp);
			tmp->onPressed();
		}
		else if ((!selectingAscending)&&(tmp->getBlockValue()== 1))
		{
			blocksPressed.pushBack(tmp);
			tmp->onPressed();
		}

		return;

	}




}

void BlocksLayer::onTouchCancelled( Touch *touch, Event *unused )
{

}



void BlocksLayer::superReduce()
{


	if (!hasGameBegan)
	{
		playSound("Invalid.wav");
		return;
	}

	if (_reduceChancesLeft == 0 )
	{
		playSound("Invalid.wav");
		return;
	}


	int ct = 0;
	clearSelection();

	for (Block* blk : blocks)
	{
		if (blk->getBlockValue() != 1)
		{
			blk->setBlockValue(blk->getBlockValue()-1);
			ct++;
			
			
		}

	}

	if (ct == 0)
	{
		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Invalid.wav",false);
		playSound("Invalid.wav");
	}
	else
	{
		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Reduce.wav",false);
		playSound("Reduce.mp3");
		setReduceChancesLeft(_reduceChancesLeft - 1);
	}

	

}



void BlocksLayer::finishSelection()
{

	//��ѡ��ʱ
	if (blocksPressed.size()==0)
	{

		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Invalid.wav",false);
		playSound("Invalid.wav");
		return;
	}

	//ȫѡ1ʱ
	if ((blocksPressed.size()==1)||(!selectingAscending))
	{
		for (Block* blk : blocksPressed)
		{
			blk->setBlockValue(2);
			blk->onSelectionFinished(false);
		}
		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("AllOneFinish.wav",false);
		playSound("AllOneFinish.wav");
	}


	//����ѡʱ
	if ((blocksPressed.size()>1)&&(selectingAscending))
	{

		currentSum = 0;
		for (Block* blk : blocksPressed)
		{
			currentSum += blk->getBlockValue();
		}

		blocksPressed.back()->setBlockValue(currentSum);
		blocksPressed.back()->onSelectionFinished(true);
		
		for (int i = 0; i<blocksPressed.size()-1;i++ )
		{
			//����ȥ�ķ��齫������1
			blocksPressed.at(i)->setBlockValue(1);
			blocksPressed.at(i)->onSelectionFinished(false);

		}
		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("AscendingFinish.wav",false);
		playSound("AscendingFinish.mp3");

		
		//����FIRST_STEP ����ť�ͷ�����ʾ����
		if (!hasGameBegan)
		{
			//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("firstblood.mp3",false);
			firstStep();

			hasGameBegan = true;
		}

		updateSum(currentSum);

	}


	blocksPressed.clear();
	//clearSelection();
}

void BlocksLayer::clearSelection()
{

	for (Block* blk: blocksPressed)
	{
		blk->onPressCancelled();

	}

	blocksPressed.clear();

}


void BlocksLayer::checkSwipeAction( cocos2d::Point point )
{

	//��������� ��������
	if ((abs(point.x - touchBeganPoint.x)<128)&&(abs(point.y - touchBeganPoint.y)<128))
	{
		return;
	}

	//������ºͷſ��ľ��볬��һ����ֵ
	if (abs(point.x - touchBeganPoint.x)>abs(point.y - touchBeganPoint.y))
	{

		if (point.x > touchBeganPoint.x)
		{
			log("right FINISH");
			finishSelection();


		}
		else
		{
			log("left CLEAR");
			clearSelection();

		}

	}
	else
	{
		if (point.y > touchBeganPoint.y)
		{
			log("up MENU");
			//this->setTouchEnabled(false);
			EventCustom event = EventCustom("SHOW_SETTING");
			_eventDispatcher->dispatchEvent(&event);
		}
		else
		{
			log("down REDUCE");
			superReduce();

		}

	}
}





void BlocksLayer::firstStep()
{

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//����ϵͳ��������һ���ļ�·���ַ���
	std::string filename = MultiLanguagePathGetter::getPath()+"/UI.xml";
	auto dictionary = Dictionary::createWithContentsOfFile(filename.c_str());


	std::string str_max = ((String*)dictionary->objectForKey("MAX SUM EVER"))->getCString(); 
	std::string str_reduce = ((String*)dictionary->objectForKey("REDUCE CHANCES"))->getCString(); 

	filename = MultiLanguagePathGetter::getPath() + "/font.fnt";

	//����MaxEver
	auto label = LabelBMFont::create(str_max.c_str(), filename.c_str());
	label->setScale(5.0f/6.0f);
	label->setPosition(Vec2(origin.x + visibleSize.width *0.38 ,
						origin.y + visibleSize.height * 0.75));
	this->addChild(label,2);
	_labelMaxNumberEver = LabelTTF::create("000", "Arial", 50);
	_labelMaxNumberEver->setPosition(Vec2(origin.x + visibleSize.width *0.88 ,
		origin.y + visibleSize.height * 0.75));
	this->addChild(_labelMaxNumberEver,2);




	auto label2 = LabelBMFont::create(str_reduce.c_str(), filename.c_str());
	label2->setScale(5.0f/6.0f);
	label2->setPosition(Vec2(origin.x + visibleSize.width *0.38 ,
		origin.y + visibleSize.height * 0.85));
	this->addChild(label2,2);
	_labelReduceChancesLeft = LabelTTF::create("000", "Arial", 50);
	_labelReduceChancesLeft->setPosition(Vec2(origin.x + visibleSize.width *0.88 ,
		origin.y + visibleSize.height * 0.85));
	this->addChild(_labelReduceChancesLeft,2);


}



void BlocksLayer::updateSum(int newSum)
{
	if (_maxNumberEver >= newSum)
	{
		log("same");
	}
	else
	{
		log("new max");
		setReduceChancesLeft( _reduceChancesLeft + newSum - _maxNumberEver);
		_maxNumberEver = newSum;
		String* st = String::createWithFormat("%3d",_maxNumberEver);
		_labelMaxNumberEver->setString(st->_string);
	}
}



void BlocksLayer::setReduceChancesLeft( int reduceChancesLeft )
{
	_reduceChancesLeft = reduceChancesLeft;
	String* st = String::createWithFormat("%3d",_reduceChancesLeft);
	_labelReduceChancesLeft->setString(st->_string);
}




void BlocksLayer::setSound(EventCustom* event)
{

	isSoundEnabled = UserDefault::getInstance()->getBoolForKey("Sound");

}

void BlocksLayer::playSound(const std::string filename)
{
	if (isSoundEnabled)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(filename.c_str(),false);
	}

}



