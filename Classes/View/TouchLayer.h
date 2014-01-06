#ifndef _TOUCHLAYER_H_
#define _TOUCHLAYER_H_

#include "cocos2d.h"

class GameplayView;
class TouchLayer : 
	public cocos2d::CCLayer
{
public:
	TouchLayer();
	~TouchLayer();

	static TouchLayer* create(GameplayView* view);
	bool init(GameplayView* view);

	virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

	void setTouchEnabled(bool flag);
protected:
	GameplayView* _parent;
};

#endif// _TOUCHLAYER_H_
