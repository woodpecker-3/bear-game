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

		//ÔİÍ£°´Å¥
		CCMenuItemSprite* pause = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("CloseNormal.png"), CCSprite::createWithSpriteFrameName("CloseSelected.png"),this,menu_selector(PanelLayer::pauseOrResume));
		CCMenu* menu = CCMenu::create(pause, NULL);

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCSize menuSize = pause->getContentSize();

		menu->setPosition(winSize.width - menuSize.width,winSize.height - menuSize.height);
		addChild(menu);

		//¼Æ·Ö°å
		 _scoreLabel = CCLabelTTF::create("score: 0", "American Typewriter", 16);
		 addChild(_scoreLabel);
		 _scoreLabel->setColor(ccc3(126, 126, 126));
		 _scoreLabel->setPosition(ccp(winSize.width - menuSize.width - 64,winSize.height - menuSize.height));
		 setScoreNumber(0);

		 //½ğ±Ò
		 _goldLabel = CCLabelTTF::create("gold: 0", "American Typewriter", 16);
		 addChild(_goldLabel);
		 _goldLabel->setColor(ccc3(126, 126, 126));
		 _goldLabel->setPosition(ccp(winSize.width - menuSize.width - 164,winSize.height - menuSize.height));
		 setScoreNumber(0);

		//ÌøÔ¾°´Å¥
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
		CCParticleFlower* particle = /*CCParticleFire*/CCParticleFlower::createWithTotalParticles(50);//
		particle->retain();
		particle->setTexture( CCTextureCache::sharedTextureCache()->addImage("stars.png") );//.pvr");
		particle->setAutoRemoveOnFinish(true);
		particle->setStartSizeVar(10.0f);
		particle->setSpeed(70.0f);
		particle->setAnchorPoint(ccp(0.5f,0.5f));
		particle->setPosition(getContentSize().width,getContentSize().height/2);
		particle->setDuration(1.0f);

		_goldLabel->setString(CCString::createWithFormat("gold: %d",number)->getCString());
		_goldLabel->addChild(particle, 10);
		particle->release();
	}
	
}

// void PanelLayer::jump( cocos2d::CCObject* pSender )
// {
// 	_parent->jump();
// }



