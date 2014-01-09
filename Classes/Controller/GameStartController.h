#ifndef _GAMESTARTCONTROLLER_H_
#define _GAMESTARTCONTROLLER_H_

#include "cocos2d.h"
#include "GameStartControllerDelegate.h"

class GameStartView;
class GameStartModel;
class GameStartController :
	public GameStartControllerDelegate,
	public cocos2d::CCScene
{
public:
	GameStartController(void);
	~GameStartController(void);

	CREATE_FUNC(GameStartController);
	bool init();

public:
	//来自GameStartControllerDelegate
	virtual void didGameInitializationCompleted();
	virtual void didGameStart();

protected:
	GameStartView* _view;
	GameStartModel* _model;
};

#endif// _GAMESTARTCONTROLLER_H_
