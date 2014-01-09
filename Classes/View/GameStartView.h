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

	//按钮点击事件
	void menuStartCallback(cocos2d::CCObject* obj);
public:
	//游戏初始化完成
	void showGameStart();
protected:
	GameStartControllerDelegate* _delegate;
};

#endif// _GAMESTARTVIEW_H_
