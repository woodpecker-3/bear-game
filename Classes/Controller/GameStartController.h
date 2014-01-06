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

	//����GameStartControllerDelegate�Ľӿ�
public:
	//model״̬�¼�
	virtual void didGameInitializationCompleted();

	//�û������¼�
	virtual void didGameStart();

protected:
	GameStartView* _view;
	GameStartModel* _model;
};

#endif// _GAMESTARTCONTROLLER_H_
