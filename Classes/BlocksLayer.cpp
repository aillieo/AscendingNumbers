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


	playSound("NewGame.mp3");


    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	hasGameBegan = false;
	isGameOver = false;
	isTouchEventValid = true;

	_maxNumberEver = 0;
	_reduceChancesLeft = 0;

	initBlocks();
	

	//飘两朵云
	auto cloud1 = Cloud::create();
	cloud1->setPositionX(origin.x);
	cloud1->setHeight(origin.y + 0.33 *holder->getContentSize().height);
	this->addChild(cloud1,2);
	auto cloud2 = Cloud::create();
	cloud1->setPositionX(origin.x + visibleSize.width * 0.5);
	cloud2->setHeight(origin.y + 0.66 *holder->getContentSize().height);
	this->addChild(cloud2,2);



	//对触摸事件的监听
	auto touchListener=EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan=CC_CALLBACK_2(BlocksLayer::onTouchBegan,this);
	touchListener->onTouchMoved=CC_CALLBACK_2(BlocksLayer::onTouchMoved,this);
	touchListener->onTouchEnded=CC_CALLBACK_2(BlocksLayer::onTouchEnded,this);
	touchListener->onTouchCancelled=CC_CALLBACK_2(BlocksLayer::onTouchCancelled,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,this);

	//对音效设置的监听
	auto listenerSS = EventListenerCustom ::create("SET_SOUND",CC_CALLBACK_1(BlocksLayer::setSound, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerSS,this);

	//恢复游戏时放音效
	auto listenerSR = EventListenerCustom ::create("RESUME_GAME",[&](EventCustom* event){playSound("Menu_hide.mp3");});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerSR,this);

	playBackgroundMusic();

/*
	//测试用开始
	for (int i = 0 ; i< 25 ; i++)
	{
		blocks.at(i)->setBlockValue(i+1);
	}
	blocks.at(2)->setBlockValue(1);
	_reduceChancesLeft = 20;
	_maxNumberEver = 28;
	//测试用结束

	*/



    return true;
}

void BlocksLayer::createBlock(int row, int col, unsigned blockValue)
{
	auto tmpBlock = FigureBlock::create();
	tmpBlock->initPosition(getCenterPoint(row,col));
	tmpBlock->setBlockValue(blockValue);
	this->addChild(tmpBlock,3);

	blocks.insert(row * matrix_width + col , tmpBlock);


	tmpBlock->setOpacity(0);
	tmpBlock->setScale(0);
	auto action1 = FadeIn::create(0.2 + 0.6 * CCRANDOM_0_1());
	auto action2 = ScaleTo::create(0.1 + 0.3 * CCRANDOM_0_1(),1);
	auto action3 = Spawn::create(action1,action2,NULL);
	tmpBlock->runAction(action3);
}

cocos2d::Point BlocksLayer::getCenterPoint( int row, int col )
{
	float x = basePoint.x + ( FigureBlock::getContentWidth() + gapSize )*(row + 1) - FigureBlock::getContentWidth()/2;
	float y = basePoint.y + ( FigureBlock::getContentWidth() + gapSize )*(col + 1) - FigureBlock::getContentWidth()/2;
	return Point(x,y);
}

FigureBlock* BlocksLayer::getBlockContainingPoint( cocos2d::Point point )
{

    Rect rect = Rect(0, 0, 0, 0);    

    for (FigureBlock* blk : blocks)
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
	FigureBlock* tmp = getBlockContainingPoint(touchBeganPoint);
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


	//如果没有按在格子上
	auto point = touch->getLocation();
	FigureBlock* tmp = getBlockContainingPoint(point);
	if (nullptr == tmp)
	{
		checkSwipeAction(point);
		return;
	}

	//如果松开的格子和按下的格子不同
	if (tmp != touchBeganBlock)
	{
		checkSwipeAction(point);
		return;
	}


	if (isGameOver)
	{
		return;
	}

	//如果按在已按的格子上
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



	//选第一个的时候 只能是1
	if ((blocksPressed.empty())&&(tmp->getBlockValue()==1))
	{
		blocksPressed.pushBack(tmp);
		tmp->onPressed();

		return;
	}



	//选第二个的时候 可以是1或2
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


	//从第三个开始
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
		playSound("Invalid.mp3");
		return;
	}

	if (_reduceChancesLeft == 0 )
	{
		playSound("Invalid.mp3");
		return;
	}


	int ct = 0;
	clearSelection();

	for (FigureBlock* blk : blocks)
	{
		if (blk->getBlockValue() != 1)
		{

			blk->onReduce();
			ct++;
			
			
		}

	}

	if (ct == 0)
	{
		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Invalid.wav",false);
		playSound("Invalid.mp3");
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

	//无选择时
	if (blocksPressed.size()==0)
	{

		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Invalid.wav",false);
		playSound("Invalid.mp3");
		return;
	}

	//全选1时
	if ((blocksPressed.size()==1)||(!selectingAscending))
	{
		for (FigureBlock* blk : blocksPressed)
		{
			blk->setBlockValue(2);
			blk->onSelectionFinished(false);
		}
		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("AllOneFinish.wav",false);
		playSound("AllOneFinish.mp3");

	}


	//递增选时
	if ((blocksPressed.size()>1)&&(selectingAscending))
	{

		currentSum = 0;
		for (FigureBlock* blk : blocksPressed)
		{
			currentSum += blk->getBlockValue();
		}

		blocksPressed.back()->setBlockValue(currentSum);
		blocksPressed.back()->onSelectionFinished(true);
		
		for (int i = 0; i<blocksPressed.size()-1;i++ )
		{
			//被消去的方块将随机变成1
			blocksPressed.at(i)->setBlockValue(1);
			blocksPressed.at(i)->onSelectionFinished(false);

		}
		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("AscendingFinish.wav",false);
		if (currentSum != 3)
		{
			playSound("AscendingFinish.mp3");
		}
		else
		{
			playSound("AscendingFinish_3.mp3");
		}
		

		
		//触发FIRST_STEP 将按钮和分数显示出来
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

	if (checkEnding())
	{
		onGameOver();
	}
	

}

void BlocksLayer::clearSelection()
{

	for (FigureBlock* blk: blocksPressed)
	{
		blk->onPressCancelled();

	}

	blocksPressed.clear();

}


void BlocksLayer::checkSwipeAction( cocos2d::Point point )
{

	//滑动距离短 不起作用
	if ((abs(point.x - touchBeganPoint.x)<128)&&(abs(point.y - touchBeganPoint.y)<128))
	{
		return;
	}

	//如果按下和放开的距离超过一定数值
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

			playSound("Menu_show.mp3");

		}
		else
		{
			log("down REDUCE");
			if (!isGameOver)
			{
				superReduce();
			}
			

		}

	}
}


void BlocksLayer::firstStep()
{

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//根据系统语言生成一个文件路径字符串
	std::string filename = MultiLanguagePathGetter::getPath()+"/UI.xml";
	auto dictionary = Dictionary::createWithContentsOfFile(filename.c_str());


	std::string str_max = ((String*)dictionary->objectForKey("MAX SUM EVER"))->getCString(); 
	std::string str_reduce = ((String*)dictionary->objectForKey("REDUCE CHANCES"))->getCString(); 

	//filename = MultiLanguagePathGetter::getPath() + "/font.fnt";

	//创建MaxEver
	//auto label = LabelBMFont::create(str_max.c_str(), filename.c_str());
	auto label = LabelTTF::create(str_max.c_str(),"Arial",50);
	label->setScale(5.0f/6.0f);
	label->setPosition(Vec2(origin.x + visibleSize.width *0.38 ,
						origin.y + visibleSize.height * 0.75));
	this->addChild(label,2);
	_labelMaxNumberEver = LabelTTF::create("000", "Arial", 50);
	_labelMaxNumberEver->setPosition(Vec2(origin.x + visibleSize.width *0.88 ,
		origin.y + visibleSize.height * 0.75));
	this->addChild(_labelMaxNumberEver,2);




	auto label2 = LabelTTF::create(str_reduce.c_str(), "Arial",50);
	//auto label2 = LabelBMFont::create(str_reduce.c_str(), filename.c_str());
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

		if (_maxNumberEver > 3)
		{
			//动画
			ParticleSystem* ps = ParticleSystemQuad::create("newMax.plist");
			addChild(ps,10);
			ps->setPosition(_labelMaxNumberEver->getPosition());
			ps->setAutoRemoveOnFinish(true);
			playSound("New_max.mp3");
		}

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

	if (isSoundEnabled)
	{
		if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		}
		else
		{
			playBackgroundMusic();
		}
	}
	else
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	}


}

void BlocksLayer::playSound(const std::string filename)
{

	if (isSoundEnabled)
	{

		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(filename.c_str(),false);

	}

}

bool BlocksLayer::checkEnding()
{

	if (_maxNumberEver < 28)
	{
		return false;
	}

	std::set<unsigned> all;

	for (FigureBlock* b : blocks)
	{
		all.insert(b->getBlockValue());
	}
	log(" all size = %d" , all.size());


	if (all.size() == 25)
	{
		for (int i = 1 ; i<=25 ; i++)
		{
			if (all.count(i) != 1)
			{
				return false;
			}
		}
		log("GAME OVER WIN");
		return true;
		
	}

	return false;

}

void BlocksLayer::onGameOver()
{

	isGameOver = true;

	//按照从小到大的顺序 重新排列blocks索引
	std::sort(blocks.begin(),blocks.end(),[](FigureBlock* b1, FigureBlock* b2){return b1->getBlockValue()<b2->getBlockValue();});

	//动画
	ParticleSystem* ps = ParticleSystemQuad::create("particle.plist");
	addChild(ps,10);

	Vector<FiniteTimeAction*> actions;

	for (int i=0 ; i<25 ; i++)
	{
		MoveTo* mt = MoveTo::create(0.2,blocks.at(i)->getPosition());
		actions.pushBack(mt);
		DelayTime* dt = DelayTime::create(0.2);
		actions.pushBack(dt);
	}
	Sequence* seq = Sequence::create(actions);
	ps->runAction(RepeatForever::create(seq));

	//音乐


}

void BlocksLayer::initBlocks()
{


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//创建方格阵的背景 430x430 两个格子块之间间隙5
	holder = Sprite::create("BLANK.png");
	holder->setTextureRect(Rect(0,0,640,640));
	holder-> setPosition(origin.x + visibleSize.width / 2 , origin.y + holder->getContentSize().height/2 );
	holder->setOpacity(0);
	this->addChild(holder,0);

	//获取basePoint
	basePoint = Point(holder->getPositionX() - holder->getContentSize().width /2 ,holder->getPositionY() - holder->getContentSize().height /2 );

	/*
	
	//根据当前时间 产生随机数种子

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

	*/

	//除了2以外的数中 “3”或 “1”的个数
	int rest1 = (matrix_height*matrix_width)/4;
	int rest3 = (matrix_height*matrix_width)/4;

	//初始化方阵 
	blocks = Vector<FigureBlock*>(matrix_height * matrix_width); 
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

				//随机生成1或3 并确保1和3的个数
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




}

void BlocksLayer::playBackgroundMusic()
{
	if (isSoundEnabled)
	{
		if (!CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("bg_music.mp3",true);
		}
		
	}

}



