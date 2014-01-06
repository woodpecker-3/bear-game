#ifndef _GAMEPLAYVIEW_H_
#define _GAMEPLAYVIEW_H_

#include "cocos2d.h"

class GameplayControllerDelegate;
class BackgroundLayer;
class PlayerLayer;
class PanelLayer;
class TouchLayer;
class MenuLayer;
class GameplayView :
	public cocos2d::CCNode
{
public:
	friend class PanelLayer;
	GameplayView(void);
	~GameplayView(void);

	static GameplayView* create(GameplayControllerDelegate* theDelegate);
	bool init(GameplayControllerDelegate* theDelegate);

	void setTouchEnabled(bool flag);

protected:
	void pauseOrResume();
	void jump();

private:
	BackgroundLayer* _background;
	PlayerLayer* _playerLayer;
	PanelLayer* _panelLayer;
	TouchLayer *_touchLayer;
	MenuLayer* _menuLayer;


	GameplayControllerDelegate* _delegate;
};

#endif// _GAMEPLAYVIEW_H_

