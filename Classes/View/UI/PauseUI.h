#ifndef _BEAR_PAUSEUI_H_
#define _BEAR_PAUSEUI_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class GameplayView;
class PauseUI :
	public cocos2d::CCNode
{
public:
	PauseUI();
	~PauseUI();

	static PauseUI* create(GameplayView* view);
	bool init( GameplayView* view );
protected:
	void clickResume(cocos2d::CCObject* sender);
	void clickRestart(cocos2d::CCObject* sender);
private:
	GameplayView* _view;

	cocos2d::extension::UILayer* _ul;
};
#endif// _BEAR_PAUSEUI_H_
