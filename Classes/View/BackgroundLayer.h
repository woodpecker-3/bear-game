#ifndef _BACKGROUNDLAYER_H_
#define _BACKGROUNDLAYER_H_

#include "cocos2d.h"

class BackgroundLayer :
	public cocos2d::CCLayer
{
public:
	BackgroundLayer();
	~BackgroundLayer();

	CREATE_FUNC(BackgroundLayer);

	bool init();

	void setOffsetX(float offsetX);

private:
	cocos2d::CCSprite* _backgroundSprite;
	cocos2d::CCSprite* _backgroundSprite2;
	cocos2d::CCSprite* _mediumgroundSprite;
	cocos2d::CCSprite* _mediumgroundSprite2;
	cocos2d::CCSprite* _light;
protected:
	float _offsetX;
};
#endif// _BACKGROUNDLAYER_H_
