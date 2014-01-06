#include "PlayerLayer.h"
#include "GameplayModel.h"
#include "Terrain.h"

USING_NS_CC;
PlayerLayer::PlayerLayer()
{

}

PlayerLayer::~PlayerLayer()
{

}

bool PlayerLayer::init()
{
	bool bRet = false;
	do 
	{
		Terrain* terrain = GameplayModel::sharedModel()->getTerrain();
		addChild(terrain);
 		CCSize size = CCDirector::sharedDirector()->getWinSize();
 		terrain->setPosition(size.width/4,0);

		bRet = true;
	} while (0);
	return bRet;
}
