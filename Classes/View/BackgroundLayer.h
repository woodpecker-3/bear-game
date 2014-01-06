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

	CC_SYNTHESIZE_RETAIN(cocos2d::CCSprite*,_backgroundSprite,BackgroundSprite);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCSprite*,_backgroundSprite2,BackgroundSprite2);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCSprite*,_mediumgroundSprite,MediumgroundSprite);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCSprite*,_mediumgroundSprite2,MediumgroundSprite2);
protected:
	float _offsetX;
};
#endif// _BACKGROUNDLAYER_H_
