#ifndef _PLAYERLAYER_H_
#define _PLAYERLAYER_H_

#include "cocos2d.h"

class PlayerLayer : 
	public cocos2d::CCLayer
{
public:
	PlayerLayer();
	~PlayerLayer();

	CREATE_FUNC(PlayerLayer);
	bool init();
};
#endif// _PLAYERLAYER_H_
