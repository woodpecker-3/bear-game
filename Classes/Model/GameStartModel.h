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
	//异步加载资源完成回调
	void loadingCallback(cocos2d::CCObject* obj);
	//加载资源完成
	void loadingFinish();

	//精灵材质、精灵表等缓存
	void cacheInit();
	//音乐音效引擎和玩家数据初始化
	void audioAndUserDataInit();

	//所有预备工作完成
	void gameInitializationCompleted();
protected:
	GameStartControllerDelegate* _delegate;
};

#endif// _GAMESTARTMODEL_H_


