#ifndef _BEAR_ORNAMENTAL_H_
#define _BEAR_ORNAMENTAL_H_

#include "cocos2d.h"

class Ornamental :
	public cocos2d::CCNode
{
public:
	Ornamental();
	~Ornamental();

	CREATE_FUNC(Ornamental);
	bool init();

	void update(float delta);
};
#endif// _BEAR_ORNAMENTAL_H_