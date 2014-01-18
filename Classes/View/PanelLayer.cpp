#include "PanelLayer.h"
#include "GameplayView.h"

USING_NS_CC;

PanelLayer::PanelLayer()
{
	_parent = NULL;
	_scoreLabel = NULL;
	_score = 0;
	_goldLabel = NULL;
	_gold = 0;
}

PanelLayer::~PanelLayer()
{

}

PanelLayer* PanelLayer::create( GameplayView* view )
{
	PanelLayer* layer = new PanelLayer;
	if (layer && layer->init(view))
	{
		layer->autorelease();
		return layer;
	}
	return NULL;
}

bool PanelLayer::init(GameplayView* view)
{
	bool bRet = false;
	do 
	{
		_parent = view;

		//ÔÝÍŁ°´ĹĽ
		CCMenuItemSprite* pause = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("CloseNormal.png"), CCSprite::createWithSpriteFrameName("CloseSelected.png"),this,menu_selector(PanelLayer::pauseOrResume));
		CCMenu* menu = CCMenu::create(pause, NULL);

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCSize menuSize = pause->getContentSize();

		menu->setPosition(winSize.width - menuSize.width,winSize.height - menuSize.height);
		addChild(menu);

		//źĆˇÖ°ĺ
		 _scoreLabel = CCLabelTTF::create("score: 0", "American Typewriter", 16);
		 addChild(_scoreLabel);
		 _scoreLabel->setColor(ccc3(126, 126, 126));
		 _scoreLabel->setPosition(ccp(winSize.width - menuSize.width - 64,winSize.height - menuSize.height));
		 setScoreNumber(0);

		 //˝đąŇ
		 _goldLabel = CCLabelTTF::create("gold: 0", "American Typewriter", 16);
		 addChild(_goldLabel);
		 _goldLabel->setColor(ccc3(126, 126, 126));
		 _goldLabel->setPosition(ccp(winSize.width - menuSize.width - 164,winSize.height - menuSize.height));
		 setScoreNumber(0);

		//ĚřÔž°´ĹĽ
// 		pause = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("CloseNormal.png"), CCSprite::createWithSpriteFrameName("CloseSelected.png"),this,menu_selector(PanelLayer::jump));
// 		menu = CCMenu::create(pause, NULL);
// 		menu->setPosition(winSize.width - menuSize.width,/*winSize.height -*/ menuSize.height);
// 		addChild(menu);

		bRet = true;
	} while (0);
	return  bRet;
}


void PanelLayer::pauseOrResume( cocos2d::CCObject* pSender )
{
	_parent->pauseOrResume();
}

void PanelLayer::setScoreNumber( int number )
{
	if (number != _score)
	{
		_score = number;
		_scoreLabel->setString(CCString::createWithFormat("score: %d",number)->getCString());
	}
}

void PanelLayer::setGoldNumber( int number )
{
	if (number != _gold)
	{
		_gold = number;
		_goldLabel->setString(CCString::createWithFormat("gold: %d",number)->getCString());
	}
	
}

// void PanelLayer::jump( cocos2d::CCObject* pSender )
// {
// 	_parent->jump();
// }



