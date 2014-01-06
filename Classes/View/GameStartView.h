#ifndef _GAMESTARTVIEW_H_
#define _GAMESTARTVIEW_H_

#include "cocos2d.h"

class GameStartControllerDelegate;
class GameStartView :
	public cocos2d::CCLayer
{
public:
	GameStartView(void);
	~GameStartView(void);

	static GameStartView* create(GameStartControllerDelegate* theDelegate);
	bool init(GameStartControllerDelegate* theDelegate);

	//��ť����¼�
	void menuStartCallback(cocos2d::CCObject* obj);
public:
	//��Ϸ��ʼ�����
	void showGameStart();
protected:
	GameStartControllerDelegate* _delegate;
};

#endif// _GAMESTARTVIEW_H_
