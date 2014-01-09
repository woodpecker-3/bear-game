#ifndef _GAMESTARTMODEL_H_
#define _GAMESTARTMODEL_H_

#include "cocos2d.h"

class GameStartControllerDelegate;
class GameStartModel : 
	public cocos2d::CCObject
{
public:
	GameStartModel(void);
	~GameStartModel(void);

	static GameStartModel* create(GameStartControllerDelegate* theDelegate);
	bool init(GameStartControllerDelegate* theDelegate);

	void gameInitialization();
protected:
	//sync load
	void loadingCallback(cocos2d::CCObject* obj);
	void loadingFinish();

	//cache sprite sheet
	void cacheInit();
	void audioAndUserDataInit();

	void gameInitializationCompleted();
protected:
	GameStartControllerDelegate* _delegate;
};

#endif// _GAMESTARTMODEL_H_


