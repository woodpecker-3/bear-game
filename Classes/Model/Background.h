#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include "cocos2d.h"

class Background :
	public cocos2d::CCNode
{
public:
	Background();
	~Background();

	CREATE_FUNC(Background);
	bool init();

	void update(float delta);
protected:
	void fellow();
private:
	cocos2d::CCNode* _background;
	cocos2d::CCSprite* _backgroundSprite;
	cocos2d::CCSprite* _backgroundSprite2;

	cocos2d::CCNode* _mediumground;
	cocos2d::CCSprite* _mediumgroundSprite;
	cocos2d::CCSprite* _mediumgroundSprite2;

	cocos2d::CCSprite* _light;

	int _offsetX;
};
#endif// _BACKGROUND_H_
