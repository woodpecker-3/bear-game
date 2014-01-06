#include "GameStartView.h"
#include "GameStartControllerDelegate.h"

USING_NS_CC;
GameStartView::GameStartView(void)
{
}


GameStartView::~GameStartView(void)
{
}

GameStartView* GameStartView::create( GameStartControllerDelegate* theDelegate )
{
	GameStartView* view = new GameStartView;
	if (view && view->init(theDelegate))
	{
		view->autorelease();
		return view;
	}
	return NULL;
}

bool GameStartView::init( GameStartControllerDelegate* theDelegate )
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		CCSprite* background = CCSprite::create("background.png");
		background->setPosition(CCPointMake(winSize.width*0.5, winSize.height*0.5));
		addChild(background);

		CCSprite* title = CCSprite::create("title.png");
		title->setPosition(CCPointMake(winSize.width*0.5, winSize.height*0.7));
		addChild(title);

		_delegate = theDelegate;
		bRet = true;
	} while (0);
	return bRet;
}

void GameStartView::showGameStart()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCMenuItemSprite* start = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("menu_start_normal.png"), CCSprite::createWithSpriteFrameName("menu_start_selected.png"), this, menu_selector( GameStartView::menuStartCallback));
	CCMenu* menu = CCMenu::create(start, NULL);
	menu->setPosition(CCPointMake(winSize.width*0.5, winSize.height*0.4));

	addChild(menu);
}

void GameStartView::menuStartCallback( cocos2d::CCObject* obj )
{
	_delegate->didGameStart();
}
