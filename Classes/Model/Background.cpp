#include "Background.h"
#include <algorithm>

USING_NS_CC;
Background::Background()
{
	_backgroundSprite = NULL;
	_backgroundSprite2 = NULL;
	_mediumgroundSprite = NULL;
	_mediumgroundSprite2 = NULL;
	_backgroundSprite = NULL;
	_offsetX = 0;
}

Background::~Background()
{

}

bool Background::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCNode::init());

		_background = CCNode::create();
		addChild(_background);
		//bg1
		_backgroundSprite = CCSprite::create("bg.png");
		CC_BREAK_IF(!_backgroundSprite);
		_backgroundSprite->setAnchorPoint(CCPointMake(0,0));
		_background->addChild(_backgroundSprite);
		//bg2
		_backgroundSprite2 = CCSprite::create("bg.png");
		CC_BREAK_IF(!_backgroundSprite2);
		_backgroundSprite2->setAnchorPoint(CCPointMake(0,0));
		_background->addChild(_backgroundSprite2);
		_backgroundSprite2->setPosition(CCPointMake(_backgroundSprite2->getContentSize().width,0));

		_mediumground = CCNode::create();
		addChild(_mediumground);
		//mg1
		_mediumgroundSprite = CCSprite::create("mg.png");
		CC_BREAK_IF(!_mediumgroundSprite);
		_mediumgroundSprite->setAnchorPoint(CCPointMake(0,0));
		_mediumground->addChild(_mediumgroundSprite);

		//mg2
		_mediumgroundSprite2 = CCSprite::create("mg.png");
		CC_BREAK_IF(!_mediumgroundSprite2);
		_mediumgroundSprite2->setAnchorPoint(CCPointMake(0,0));
		_mediumground->addChild(_mediumgroundSprite2);
		_mediumgroundSprite2->setPosition(CCPointMake(_mediumgroundSprite2->getContentSize().width,0));

		//light
		_light = CCSprite::create("light.png");
		_light->setAnchorPoint(CCPointMake(0,0));
		addChild(_light);

		bRet = true;
	} while (0);
	return bRet;
}

void Background::fellow(int offsetX)
{
	CCSize winsSize = CCDirector::sharedDirector()->getWinSize();
	_background->setPositionX(_background->getPosition().x - (offsetX - _offsetX) * 0.25f);
	_mediumground->setPositionX(_mediumground->getPosition().x - (offsetX - _offsetX) * 0.15f);

	/*转换本地坐标在屏幕视口坐标**/
	if ( (_backgroundSprite->getPositionX() + _background->getPositionX()) < (-1*(_backgroundSprite->getContentSize().width + winsSize.width/8)))
	{
		_backgroundSprite->setPositionX(_backgroundSprite2->getPositionX() + _backgroundSprite2->getContentSize().width);
		std::swap(_backgroundSprite,_backgroundSprite2);
	}
	if ( (_mediumgroundSprite->getPositionX() + _mediumground->getPositionX()) < (-1*(_mediumgroundSprite->getContentSize().width + winsSize.width/8)))
	{
		_mediumgroundSprite->setPositionX(_mediumgroundSprite2->getPositionX() + _mediumgroundSprite2->getContentSize().width);
		std::swap(_mediumgroundSprite,_mediumgroundSprite2);
	}
	_offsetX = offsetX;
}

