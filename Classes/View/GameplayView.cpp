#include "GameplayView.h"
#include "GameplayControllerDelegate.h"
#include "BackgroundLayer.h"
#include "PanelLayer.h"
#include "PlayerLayer.h"
#include "TouchLayer.h"
#include "GameplayModel.h"
#include "Defined.h"
#include "Hero.h"
#include "BearData.h"
#include "MenuLayer.h"
#include "Terrain.h"
#include "cocos-ext.h"
#include "PauseUI.h"
#include "ResultUI.h"
#include "OrnamentalLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;
GameplayView::GameplayView(void)
{
	_background = NULL;
	_playerLayer = NULL;
	_panelLayer = NULL;
	_touchLayer = NULL;
	_menuLayer = NULL;
	_delegate = NULL;
	_pauseUI = NULL;
	_resultUI = NULL;
}


GameplayView::~GameplayView(void)
{
	CC_SAFE_RELEASE(_menuLayer);

	CC_SAFE_RELEASE(_pauseUI);
	CC_SAFE_RELEASE(_resultUI);
}

GameplayView* GameplayView::create( GameplayControllerDelegate* theDelegate )
{
	GameplayView* view = new GameplayView;
	if (view && view->init(theDelegate))
	{
		view->autorelease();
		return view;
	}
	return NULL;
}

bool GameplayView::init( GameplayControllerDelegate* theDelegate )
{
	bool bRet = false;
	do 
	{
		_delegate = theDelegate;
		_background = BackgroundLayer::create();
		CC_BREAK_IF(!_background);
		_background->setAnchorPoint(CCPointZero);
		addChild(_background);

		_ornamental = OrnamentalLayer::create();
		CC_BREAK_IF(!_ornamental);
		addChild(_ornamental);

		_playerLayer = PlayerLayer::create();
		CC_BREAK_IF(!_playerLayer);
		addChild(_playerLayer);

		_panelLayer = PanelLayer::create(this);
		CC_BREAK_IF(!_panelLayer);
		addChild(_panelLayer);

		_touchLayer = TouchLayer::create(this);
		CC_BREAK_IF(!_touchLayer);
		addChild(_touchLayer);
		_touchLayer->setTouchEnabled(true);

		_menuLayer = MenuLayer::create(this);
		CC_BREAK_IF(!_touchLayer);
		_menuLayer->retain();

		_pauseUI = PauseUI::create(this);
		CC_BREAK_IF(!_pauseUI);
		_pauseUI->retain();


		_resultUI = ResultUI::create(this);
		CC_BREAK_IF(!_resultUI);
		_resultUI->retain();

		bRet = true;
	} while (0);
	return bRet;
}

void GameplayView::clickPause()
{
	_delegate->didPause();
}

void GameplayView::touchBegan()
{
	_delegate->didTouchBegan();
}

void GameplayView::touchEnded()
{
	_delegate->didTouchEnded();
}

void GameplayView::touchCancelled()
{
	_delegate->didTouchCancelled();
}

void GameplayView::update( float delta )
{
	CCNode::update(delta);

	_panelLayer->setScoreNumber(BearData::sharedData()->getScore());
	_panelLayer->setGoldNumber(BearData::sharedData()->getGold());

	/*����**/
	float cacheScale = GameplayModel::sharedModel()->getCacheScale();
	_background->setScale(SCALE_BACKGROUND(cacheScale) );
	_ornamental->setScale(SCALE_DECORATIVE(cacheScale));
	_playerLayer->setScale(cacheScale);
	
}

void GameplayView::showResult()
{
	_resultUI->showResult();
	addChild(_resultUI);
}

void GameplayView::showGameView()
{
	if (_pauseUI->getParent() == this)
	{
		removeChild(_pauseUI);
	}
	if (_resultUI->getParent() == this)
	{
		removeChild(_resultUI);
	}
}

void GameplayView::showPauseUI()
{
	addChild(_pauseUI);
}


void GameplayView::clickResume()
{
	_delegate->didResume();
}


void GameplayView::clickRestart()
{
	_delegate->didReset();
}
