#include "GameplayView.h"
#include "GameplayControllerDelegate.h"
#include "BackgroundLayer.h"
#include "PanelLayer.h"
#include "PlayerLayer.h"
#include "TouchLayer.h"

USING_NS_CC;
GameplayView::GameplayView(void)
{

}


GameplayView::~GameplayView(void)
{

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
		addChild(_background);

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

void GameplayView::jump()
{
	_delegate->didJump();
}
