#include "GameStartModel.h"
#include "GameStartControllerDelegate.h"

USING_NS_CC;
GameStartModel::GameStartModel(void)
{
	_delegate = NULL;
}


GameStartModel::~GameStartModel(void)
{
}

GameStartModel* GameStartModel::create( GameStartControllerDelegate* theDelegate )
{
	GameStartModel* startModel = new GameStartModel;
	if (startModel && startModel->init(theDelegate))
	{
		startModel->autorelease();
		return startModel;
	}
	return NULL;
}

bool GameStartModel::init(GameStartControllerDelegate* theDelegate)
{
	bool bRet = false;
	do 
	{
		_delegate = theDelegate;
		bRet = true;
	} while (0);
	return bRet;
}

void GameStartModel::gameInitialization()
{
	if(CCDirector::sharedDirector()->getTotalFrames()<10){
		//CCTextureCache::sharedTextureCache()->addImageAsync("fishingjoy_texture.png", this, callfuncO_selector(GameStartModel::loadingCallback));

		// progress

		audioAndUserDataInit();
		loadingFinish();
	}else{
		loadingFinish();
	}
}

void GameStartModel::loadingCallback(cocos2d::CCObject* obj)
{
	loadingFinish();
}

void GameStartModel::loadingFinish()
{
	cacheInit();
    gameInitializationCompleted();
}

void GameStartModel::cacheInit()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("bear_resource.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("bear.plist");
	CCTextureCache::sharedTextureCache()->addImage("fire.png");
}

void GameStartModel::audioAndUserDataInit()
{
	//DynamicData::sharedDynamicData();
	//CocosDenshion::SimpleAudioEngine::sharedEngine();
}

void GameStartModel::gameInitializationCompleted()
{
	_delegate->didGameInitializationCompleted();
}
