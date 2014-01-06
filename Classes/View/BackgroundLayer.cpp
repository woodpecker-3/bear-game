#include "BackgroundLayer.h"


USING_NS_CC;
BackgroundLayer::BackgroundLayer()
{
	_backgroundSprite = NULL;
	_backgroundSprite2 = NULL;
	_mediumgroundSprite = NULL;
	_mediumgroundSprite2 = NULL;
	_offsetX= 0;
}

BackgroundLayer::~BackgroundLayer()
{
	CC_SAFE_RELEASE_NULL(_backgroundSprite);
	CC_SAFE_RELEASE_NULL(_backgroundSprite2);
	CC_SAFE_RELEASE_NULL(_mediumgroundSprite);
	CC_SAFE_RELEASE_NULL(_mediumgroundSprite2);
}

bool BackgroundLayer::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());

		//bg1
		CCSprite* sprite = CCSprite::create("bg.png");
		sprite->setAnchorPoint(CCPointMake(0,0));
		setBackgroundSprite(sprite);
		addChild(sprite);
		//bg2
		sprite = CCSprite::create("bg.png");
		sprite->setAnchorPoint(CCPointMake(0,0));
		setBackgroundSprite2(sprite);
		addChild(sprite);
		sprite->setPosition(CCPointMake(sprite->getContentSize().width,0));

		//mg1
		sprite = CCSprite::create("bg.png");
		sprite->setAnchorPoint(CCPointMake(0,0));
		setMediumgroundSprite(sprite);
		addChild(sprite);
		//mg2
		sprite = CCSprite::create("bg.png");
		sprite->setAnchorPoint(CCPointMake(0,0));
		setMediumgroundSprite(sprite);
		addChild(sprite);
		sprite->setPosition(CCPointMake(sprite->getContentSize().width,0));
		bRet =true;
	} while (0);
	return bRet;
}

void BackgroundLayer::setOffsetX( float offsetX )
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	//一直往左移
	_backgroundSprite->setPositionX(_backgroundSprite->getPosition().x - (offsetX - _offsetX) * 0.25f);
	float diffx;
	//移完一个宽度时，重新把位置设置为接近0的位置
	if (_backgroundSprite->getPositionX() < -(_backgroundSprite->getContentSize().width - size.width)) {
		//diffx = fabs(_backgroundSprite->getPositionX()) - _backgroundSprite->getContentSize().width;
		_backgroundSprite->setPositionX(0);
	}

	_mediumgroundSprite->setPositionX(_mediumgroundSprite->getPosition().x - (offsetX - _offsetX) * 0.15f);
	// 	if (_mediumgroundSprite->getPositionX() < -_mediumgroundSprite->getContentSize().width * 0.15f) {
	// 		diffx = fabs(_mediumgroundSprite->getPositionX()) - _mediumgroundSprite->getContentSize().width * 0.15f;
	// 		_mediumgroundSprite->setPositionX(-diffx);
	// 
	if (_mediumgroundSprite->getPositionX() < -(_mediumgroundSprite->getContentSize().width - size.width)) {
		//diffx = fabs(_backgroundSprite->getPositionX()) - _backgroundSprite->getContentSize().width;
		_mediumgroundSprite->setPositionX(0);
	}
	_offsetX = offsetX;
}
