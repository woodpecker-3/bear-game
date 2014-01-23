#ifndef _GAMESTARTVIEW_H_
#define _GAMESTARTVIEW_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class GameStartControllerDelegate;
class GameStartView :
	public cocos2d::CCLayer
{
public:
	GameStartView(void);
	~GameStartView(void);

	static GameStartView* create(GameStartControllerDelegate* theDelegate);
	bool init(GameStartControllerDelegate* theDelegate);
public:
	void showGameStart();
protected:
	void menuFrinedCallback(cocos2d::CCObject* obj);
	void menuStartCallback(cocos2d::CCObject* obj);
protected:
	GameStartControllerDelegate* _delegate;

	cocos2d::extension::UILayer* _loadingUI;
	cocos2d::extension::UILayer* _startUI;
};

#endif// _GAMESTARTVIEW_H_
