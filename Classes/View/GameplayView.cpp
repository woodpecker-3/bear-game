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

USING_NS_CC;
GameplayView::GameplayView(void)
{
	_background = NULL;
	_playerLayer = NULL;
	_panelLayer = NULL;
	_touchLayer = NULL;
	_menuLayer = NULL;
	_delegate = NULL;
}


GameplayView::~GameplayView(void)
{
	CC_SAFE_RELEASE(_menuLayer);
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
		_background->setScale(0.88f);

		//_platformLayer
		_playerLayer = PlayerLayer::create();
		CC_BREAK_IF(!_playerLayer);
		addChild(_playerLayer);

		_panelLayer = PanelLayer::create(this);
		CC_BREAK_IF(!_panelLayer);
		addChild(_panelLayer);

		_touchLayer = TouchLayer::create(this);
		CC_BREAK_IF(!_touchLayer);
		addChild(_touchLayer);

		_menuLayer = MenuLayer::create(this);
		CC_BREAK_IF(!_touchLayer);
		_menuLayer->retain();

		bRet = true;
	} while (0);
	return bRet;
}

void GameplayView::setTouchEnabled( bool flag )
{
	_touchLayer->setTouchEnabled(flag);
}

void GameplayView::pauseOrResume()
{
	_delegate->didPauseOrResume();
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

	_playerLayer->setScale(GameplayModel::sharedModel()->getTerrainScale());
}

void GameplayView::showResult()
{
	_panelLayer->setVisible(false);
	_touchLayer->setVisible(false);
	_menuLayer->showResult();
	addChild(_menuLayer);
}

void GameplayView::resume()
{
	_panelLayer->setVisible(true);
	_touchLayer->setVisible(true);
	removeChild(_menuLayer);

	_delegate->didReset();
}
