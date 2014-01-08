#ifndef _GAMEPLAYCONTROLLER_H_
#define _GAMEPLAYCONTROLLER_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "GameplayControllerDelegate.h"

typedef enum
{
	k_Operate_Pause = 0,
	k_Operate_Resume
}OperateFlag;

class GameplayView;
class GameplayController : 
	public GameplayControllerDelegate,
	public cocos2d::CCScene
{
public:
	GameplayController(void);
	~GameplayController(void);

	CREATE_FUNC(GameplayController);

	bool init();
	void update(float dt);

	//来自GameplayControllerDelegate的接口
public:
	virtual void didPauseOrResume();
	virtual void didTouchBegan();
	virtual void didTouchEnded();
	virtual void didTouchCancelled();

	virtual void didReset();
protected:
	void pause();
	void resume();
	void operateAllSchedulerAndActions(cocos2d::CCNode* node, OperateFlag flag);
protected:
	GameplayView* _view;
	

	bool _pauseFlag;
};

#endif// _GAMEPLAYCONTROLLER_H_

