#include "OrnamentalLayer.h"
#include "Ornamental.h"
#include "GameplayModel.h"

USING_NS_CC;
OrnamentalLayer::OrnamentalLayer()
{

}

OrnamentalLayer::~OrnamentalLayer()
{

}

bool OrnamentalLayer::init()
{
	bool bRet = false;
	do 
	{
		Ornamental* decorative = GameplayModel::sharedModel()->getDecorative();
		addChild(decorative);
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		decorative->setPosition(size.width/4,0);

		bRet = true;
	} while (0);
	return bRet;
}
