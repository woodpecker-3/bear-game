#include "GameStartController.h"
#include "GameStartView.h"
#include "GameStartModel.h"
#include "GameplayController.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
GameStartController::GameStartController(void)
{
	_view = NULL;
	_model = NULL;
}


GameStartController::~GameStartController(void)
{
	CC_SAFE_RELEASE(_view);
	CC_SAFE_RELEASE(_model);
}

bool GameStartController::init()
{
	bool bRet = false;
	do 
	{
		GameStartView* view = GameStartView::create(this);
		CC_BREAK_IF(!view);

		_view = view;
		_view->retain();
		addChild(_view);

		GameStartModel* model = GameStartModel::create(this);
		CC_BREAK_IF(!model);

		_model = model;
		_model->retain();

		_model->gameInitialization();

		bRet = true;
	} while (0);
	return bRet;
}

void GameStartController::didGameInitializationCompleted()
{
	_view->showGameStart();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bear.mp3", true);
	//CC_SAFE_RELEASE_NULL(_model);
}

void GameStartController::didGameStart()
{
	CCScene* scene = CCTransitionFadeDown::create(1.0f, GameplayController::create());
	CCDirector::sharedDirector()->replaceScene(scene);
}
