#include "BackgroundLayer.h"
#include "GameplayModel.h"
#include "Background.h"

USING_NS_CC;
BackgroundLayer::BackgroundLayer()
{
	_backgroundSprite = NULL;
	_backgroundSprite2 = NULL;
	_mediumgroundSprite = NULL;
	_mediumgroundSprite2 = NULL;
	_backgroundSprite = NULL;
	_offsetX= 0;
}

BackgroundLayer::~BackgroundLayer()
{

}

bool BackgroundLayer::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());

		addChild(GameplayModel::sharedModel()->getBackground());
// 
// 		//bg1
// 		_backgroundSprite = CCSprite::create("bg.png");
// 		CC_BREAK_IF(!_backgroundSprite);
// 		_backgroundSprite->setAnchorPoint(CCPointMake(0,0));
// 		addChild(_backgroundSprite);
// 		//bg2
// 		_backgroundSprite2 = CCSprite::create("bg.png");
// 		CC_BREAK_IF(!_backgroundSprite2);
// 		_backgroundSprite2->setAnchorPoint(CCPointMake(0,0));
// 		addChild(_backgroundSprite2);
// 		_backgroundSprite2->setPosition(CCPointMake(_backgroundSprite2->getContentSize().width,0));
// 
// 		//mg1
// 		_mediumgroundSprite = CCSprite::create("mg.png");
// 		CC_BREAK_IF(!_mediumgroundSprite);
// 		_mediumgroundSprite->setAnchorPoint(CCPointMake(0,0));
// 		addChild(_mediumgroundSprite);
// 
// 		//mg2
// 		_mediumgroundSprite2 = CCSprite::create("mg.png");
// 		CC_BREAK_IF(!_mediumgroundSprite2);
// 		_mediumgroundSprite2->setAnchorPoint(CCPointMake(0,0));
// 		addChild(_mediumgroundSprite2);
// 		_mediumgroundSprite2->setPosition(CCPointMake(_mediumgroundSprite2->getContentSize().width,0));
// 
// 		//light
// 		_light = CCSprite::create("light.png");
// 		_light->setAnchorPoint(CCPointMake(0,0));
// 		addChild(_light);

		bRet =true;
	} while (0);
	return bRet;
}

