#include "TutorialPage.h"
#include "MultiLanguagePathGetter.h"


const float blockWidth = 128.0f;

USING_NS_CC;


TutorialPage* TutorialPage::create(unsigned index)
{
	TutorialPage *pRet = new TutorialPage(); 
	if (pRet && pRet->init(index)) 
	{ 
		pRet->autorelease(); 
		return pRet; 
	} 
	else 
	{ 
		delete pRet; 
		pRet = NULL; 
		return NULL; 
	}

}


bool TutorialPage::init( unsigned index )
{

	if ( !Sprite::init() )
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto holder = Sprite::create();
	holder->setTexture("BLANK.png");
	holder->setTextureRect(Rect(0,0,blockWidth*3,blockWidth*3));
	holder->setColor(ccc3(0,0,0));
	this->addChild(holder,3);
	holder->setOpacity(0);
	holder->setPosition(origin.x , origin.y + 0.16 * visibleSize.height);
	basePoint = Point(holder->getPositionX() - holder->getContentSize().width /2 ,holder->getPositionY() - holder->getContentSize().height /2 );
	

	std::string filename = MultiLanguagePathGetter::getPath() + "/font.fnt";

	caption = LabelBMFont::create("CAPTION", filename.c_str());
	caption->setScale(38.0f/60.0f);
	caption->setWidth(visibleSize.width * 60.0f / 42.0f);
	caption->setAlignment(TextHAlignment::CENTER);
	caption->setPosition(Vec2(origin.x , origin.y - visibleSize.height * 0.2));
	addChild(caption, 3);

	int blockValues[] = {1,1,3,2,3,1,1,2,2};
	//int blockValues[] = {1,4,7,2,5,8,3,6,9};

	blocks = Vector<Block*>(3*3); 
	for (int row = 0; row < 3; row++) 
	{
		for (int col = 0; col < 3; col++) 
		{
			unsigned blockValue = blockValues[row * 3 + col];

			auto tmpBlock = Block::create();
			tmpBlock->setPosition(getCenterPoint(row,col));
			tmpBlock->setBlockValue(blockValue);
			this->addChild(tmpBlock,3);
			
			blocks.insert(row * 3 + col , tmpBlock);
		}

	}



	///////////////////////////////////////////
	/*
	auto label = LabelTTF::create("0", "Arial", 70);

	String* st = String::createWithFormat("%d",index);
	label->setString(st->_string);

	holder->addChild(label, 1);
	*/
	///////////////////////////////////////////

	finger = Sprite::create("Finger.png");
	finger->setAnchorPoint(Vec2(0.5,1));
	finger->setPosition(blocks.at(5)->getPosition());
	addChild(finger,4);


	/////////////////////////////////////////////////////////
	//                        p1_rf                        //
	//                        全选1                        //
	/////////////////////////////////////////////////////////

	auto p1_action1 = CallFunc::create([&](){finger->setPosition(blocks.at(5)->getPosition());});
	auto p1_action2 = CallFunc::create([&](){blocks.at(5)->onPressed();});
	auto p1_action3 = MoveBy::create(0.3,Vec2(0,0));
	auto p1_action4 = MoveTo::create(0.6,blocks.at(6)->getPosition());
	auto p1_action5 = CallFunc::create([&](){blocks.at(6)->onPressed();});
	auto p1_action9 = MoveBy::create(0.3,Vec2(0,0));
	auto p1_action10 = MoveTo::create(0.9,Vec2(origin.x - visibleSize.width * 0.4 ,origin.y + visibleSize.height * 0.04 ));
	auto p1_action11 = MoveTo::create(0.3,Vec2(origin.x + visibleSize.width * 0.4 ,origin.y + visibleSize.height * 0.04 ));
	auto p1_action12 = CallFunc::create([&](){
												blocks.at(5)->onSelectionFinished(false);
												blocks.at(5)->setBlockValue(2);
												blocks.at(6)->onSelectionFinished(false);
												blocks.at(6)->setBlockValue(2);
											});
	auto p1_action13 = MoveBy::create(1.0,Vec2(0,0));
	auto p1_action14 = CallFunc::create([&](){
												blocks.at(5)->setBlockValue(1);
												blocks.at(6)->setBlockValue(1);
											});



	auto p1_seq = Sequence::create(p1_action1,p1_action2,p1_action3,p1_action4,p1_action5,
		p1_action9,p1_action10,p1_action11,p1_action12,p1_action13,p1_action14,
		NULL);
	auto p1_rf = RepeatForever::create(p1_seq);
	


	/////////////////////////////////////////////////////////
	//                        p2_rf                        //
	//                       递增选择                      //
	/////////////////////////////////////////////////////////

	auto p2_action1 = CallFunc::create([&](){finger->setPosition(blocks.at(1)->getPosition());});
	auto p2_action2 = CallFunc::create([&](){blocks.at(1)->onPressed();});
	auto p2_action3 = MoveBy::create(0.3,Vec2(0,0));
	auto p2_action4 = MoveTo::create(0.6,blocks.at(3)->getPosition());
	auto p2_action5 = CallFunc::create([&](){blocks.at(3)->onPressed();});
	auto p2_action6 = MoveBy::create(0.3,Vec2(0,0));
	auto p2_action7 = MoveTo::create(0.6,blocks.at(4)->getPosition());
	auto p2_action8 = CallFunc::create([&](){blocks.at(4)->onPressed();});
	auto p2_action9 = MoveBy::create(0.3,Vec2(0,0));
	auto p2_action10 = MoveTo::create(0.9,Vec2(origin.x - visibleSize.width * 0.4 ,origin.y + visibleSize.height * 0.04 ));
	auto p2_action11 = MoveTo::create(0.3,Vec2(origin.x + visibleSize.width * 0.4 ,origin.y + visibleSize.height * 0.04 ));
	auto p2_action12 = CallFunc::create([&](){
		blocks.at(1)->onSelectionFinished(false);
		blocks.at(3)->onSelectionFinished(false);
		blocks.at(3)->setBlockValue(1);
		blocks.at(4)->onSelectionFinished(true);
		blocks.at(4)->setBlockValue(6);
	});
	auto p2_action13 = MoveBy::create(1.0,Vec2(0,0));
	auto p2_action14 = CallFunc::create([&](){
		blocks.at(1)->setBlockValue(1);
		blocks.at(3)->setBlockValue(2);
		blocks.at(4)->setBlockValue(3);
	});



	auto p2_seq = Sequence::create(p2_action1,p2_action2,p2_action3,p2_action4,p2_action5,p2_action6,
		p2_action7,p2_action8,p2_action9,p2_action10,p2_action11,p2_action12,p2_action13,p2_action14,
		NULL);
	auto p2_rf = RepeatForever::create(p2_seq);


	/////////////////////////////////////////////////////////
	//                        p3_rf                        //
	//                        REDUCE                       //
	/////////////////////////////////////////////////////////

	//auto p3_action1 = CallFunc::create([&](){finger->setPosition(Vec2(origin.x - visibleSize.width * 0.4 ,origin.y + visibleSize.height * 0.04 ));});
	auto p3_action1 = CallFunc::create([&,visibleSize,origin](){finger->setPosition(Vec2(origin.x + visibleSize.width * 0.3 ,origin.y + visibleSize.height * 0.3));});
	auto p3_action2 = MoveBy::create(0.6,Vec2(0.0f ,0.0f ));
	auto p3_action3 = MoveBy::create(0.6,Vec2(0.0f ,-1 * visibleSize.height * 0.3 ));
	auto p3_action4 = CallFunc::create([&](){

		for (Block* blk : blocks )
		{
			int blockValues[] = {1,1,3,2,3,1,1,2,2};
			if (blk->getBlockValue()!=1)
			{
				blk->setBlockValue(blockValues[blocks.getIndex(blk)]-1);
			}
		}
	});
	auto p3_action5 = MoveBy::create(1.0,Vec2(0,0));
	auto p3_action6 = CallFunc::create([&](){
		for (Block* blk : blocks )
		{
			int blockValues[] = {1,1,3,2,3,1,1,2,2};
			blk->setBlockValue(blockValues[blocks.getIndex(blk)]);
		}

	});



	auto p3_seq = Sequence::create(p3_action1,p3_action2,p3_action3,p3_action4,p3_action5,p3_action6,
		NULL);
	auto p3_rf = RepeatForever::create(p3_seq);




	/////////////////////////////////////////////////////////
	//                        p4_rf                        //
	//                        clear                        //
	/////////////////////////////////////////////////////////

	auto p4_action1 = CallFunc::create([&](){finger->setPosition(blocks.at(5)->getPosition());});
	auto p4_action2 = CallFunc::create([&](){blocks.at(5)->onPressed();});
	auto p4_action3 = MoveBy::create(0.3,Vec2(0,0));
	auto p4_action4 = MoveTo::create(0.6,blocks.at(6)->getPosition());
	auto p4_action5 = CallFunc::create([&](){blocks.at(6)->onPressed();});
	auto p4_action6 = MoveBy::create(0.3,Vec2(0,0));
	auto p4_action7 = MoveTo::create(0.9,Vec2(origin.x + visibleSize.width * 0.4 ,origin.y + visibleSize.height * 0.04 ));
	auto p4_action8 = MoveTo::create(0.3,Vec2(origin.x - visibleSize.width * 0.4 ,origin.y + visibleSize.height * 0.04 ));
	auto p4_action9 = CallFunc::create([&](){
		blocks.at(5)->onSelectionFinished(false);
		blocks.at(6)->onSelectionFinished(false);
	});
	auto p4_action10 = MoveBy::create(1.0,Vec2(0,0));




	auto p4_seq = Sequence::create(p4_action1,p4_action2,p4_action3,p4_action4,p4_action5,
		p4_action6,p4_action7,p4_action8,p4_action9,p4_action10,
		NULL);
	auto p4_rf = RepeatForever::create(p4_seq);





	filename = MultiLanguagePathGetter::getPath() + "/TUTORIAL.xml";
	auto dictionary = Dictionary::createWithContentsOfFile(filename.c_str());




	//初始化 具体的不同的动作
	switch(index)
	{
	case 0:

		caption->setString( ((String*)dictionary->objectForKey("0"))->getCString());
		finger->runAction(p2_rf); 

		break;
	case 1:

		caption->setString( ((String*)dictionary->objectForKey("1"))->getCString());
		finger->runAction(p1_rf); 

		break;
	case 2:

		caption->setString( ((String*)dictionary->objectForKey("2"))->getCString());
		finger->runAction(p3_rf); 

		break;
	case 3:

		caption->setString( ((String*)dictionary->objectForKey("3"))->getCString());
		finger->runAction(p4_rf); 

		break;

	default:
		break;
	}

	

	return true;
}

Point TutorialPage::getCenterPoint( int row, int col )
{
	float x = basePoint.x + ( Block::getContentWidth())*(row + 1) - Block::getContentWidth()/2;
	float y = basePoint.y + ( Block::getContentWidth())*(col + 1) - Block::getContentWidth()/2;
	return Point(x,y);
}

