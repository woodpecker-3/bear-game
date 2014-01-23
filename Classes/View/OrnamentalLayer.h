#ifndef _BEAR_DECORATIVELAYER_H_
#define _BEAR_DECORATIVELAYER_H_

#include "cocos2d.h"

class OrnamentalLayer : 
	public cocos2d::CCLayer
{
public:
	OrnamentalLayer();
	~OrnamentalLayer();

	CREATE_FUNC(OrnamentalLayer);
	bool init();
};
#endif// _BEAR_DECORATIVELAYER_H_
