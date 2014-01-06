#ifndef _PANELLAYER_H_
#define _PANELLAYER_H_

#include "cocos2d.h"

class GameplayView;
class PanelLayer : 
	public cocos2d::CCLayer
{
public:
	PanelLayer();
	~PanelLayer();

	static PanelLayer* create(GameplayView* view);
	bool init(GameplayView* view);

protected:
	void pauseOrResume(cocos2d::CCObject* pSender);
	void jump(cocos2d::CCObject* pSender);

protected:
	GameplayView* _parent;
};
#endif// _PANELLAYER_H_
