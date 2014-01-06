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

	//来自GameStartControllerDelegate的接口
public:
	//model状态事件
	virtual void didGameInitializationCompleted();

	//用户输入事件
	virtual void didGameStart();

protected:
	GameStartView* _view;
	GameStartModel* _model;
};

#endif// _GAMESTARTCONTROLLER_H_
