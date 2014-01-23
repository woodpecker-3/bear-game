#include "Ornamental.h"
#include "GameplayModel.h"
#include "Hero.h"
#include "Defined.h"
#include "GameObject.h"

USING_NS_CC;
Ornamental::Ornamental()
{

}

Ornamental::~Ornamental()
{

}

bool Ornamental::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCNode::init());

		bRet = true;
	} while (0);
	return bRet;
}

void Ornamental::update( float delta )
{
	CCNode::update(delta);

	//fellow();

	/*É¾³ýÆÁÄ»ÍâµÄsprite**/
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	float cacheScale = SCALE_DECORATIVE(GameplayModel::sharedModel()->getCacheScale());
	CCArray* childsArr = getChildren();
	CCObject* obj=NULL;
	GameObject* sprite=NULL;
	CCARRAY_FOREACH(childsArr,obj)
	{
		sprite = (GameObject*)obj;

		if ( (sprite->getPositionX() + getPositionX() ) < (-1 * winSize.width * CONST_OUTSIZDE_SCREEN / cacheScale ) )
		{
			removeChild(sprite,true);
		}
		else if( (sprite->getPositionY() + getPositionY() ) > ( winSize.height * (CONST_OUTSIZDE_SCREEN + 1) / cacheScale ) )
		{
			removeChild(sprite,true);
		}
	}
}

