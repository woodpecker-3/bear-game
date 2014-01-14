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

		bRet =true;
	} while (0);
	return bRet;
}

