#ifndef _GAMEPLAYVIEW_H_
#define _GAMEPLAYVIEW_H_

#include "cocos2d.h"

class GameplayControllerDelegate;
class BackgroundLayer;
class OrnamentalLayer;
class PlayerLayer;
class PanelLayer;
class TouchLayer;
class MenuLayer;
class PauseUI;
class ResultUI;
class GameplayView :
	public cocos2d::CCNode
{
public:
	friend class PanelLayer;
	friend class TouchLayer;
	friend class MenuLayer;
	friend class PauseUI;
	friend class ResultUI;
	GameplayView(void);
	~GameplayView(void);

	static GameplayView* create(GameplayControllerDelegate* theDelegate);
	bool init(GameplayControllerDelegate* theDelegate);

	void update(float delta);

	void showGameView();
	void showResult();
	void showPauseUI();
protected:
	/*panle layer**/
	void clickPause();
	/*touck layer**/
	void touchBegan();
	void touchEnded();
	void touchCancelled();
	/*pause UI**/
	void clickResume();
	void clickRestart();/*result UI too**/
private:
	BackgroundLayer* _background;
	OrnamentalLayer* _ornamental;
	PlayerLayer* _playerLayer;
	PanelLayer* _panelLayer;
	TouchLayer *_touchLayer;
	MenuLayer* _menuLayer;

	PauseUI* _pauseUI;
	ResultUI* _resultUI;

	GameplayControllerDelegate* _delegate;
};

#endif// _GAMEPLAYVIEW_H_

