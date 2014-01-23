#include "GameStartView.h"
#include "GameStartControllerDelegate.h"

USING_NS_CC;
USING_NS_CC_EXT;
GameStartView::GameStartView(void)
{
	_loadingUI = NULL;
	_startUI = NULL;
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

// 		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
// 
// 		CCSprite* background = CCSprite::create("background.png");
// 		background->setPosition(CCPointMake(winSize.width*0.5, winSize.height*0.5));
// 		addChild(background);
// 
// 		CCSprite* title = CCSprite::create("title.png");
// 		title->setPosition(CCPointMake(winSize.width*0.5, winSize.height*0.7));
// 		addChild(title);
		_loadingUI =UILayer::create();
		CC_BREAK_IF(!_loadingUI);

		_loadingUI->addWidget(GUIReader::shareReader()->widgetFromJsonFile("LoadingUI_1.json"));

		_loadingUI->setAnchorPoint(ccp(0.0f,0.0f));
		addChild(_loadingUI);

		UIButton* btn = (UIButton*)_loadingUI->getWidgetByName("Button_QQ");
		btn->addReleaseEvent(this,coco_releaseselector(GameStartView::menuFrinedCallback));
		//btn->setFontName(DEFAULT_FONTNAME);

		btn = (UIButton*)_loadingUI->getWidgetByName("Button_WeChat");
		btn->addReleaseEvent(this,coco_releaseselector(GameStartView::menuFrinedCallback));
		//btn->setFontName(DEFAULT_FONTNAME);

		_delegate = theDelegate;
		bRet = true;
	} while (0);
	return bRet;
}

void GameStartView::showGameStart()
{
// 	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
// 	CCMenuItemSprite* start = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("menu_start_normal.png"), CCSprite::createWithSpriteFrameName("menu_start_selected.png"), this, menu_selector( GameStartView::menuStartCallback));
// 	CCMenu* menu = CCMenu::create(start, NULL);
// 	menu->setPosition(CCPointMake(winSize.width*0.5, winSize.height*0.4));

	//addChild(menu);
}

void GameStartView::menuStartCallback( cocos2d::CCObject* obj )
{
	_delegate->didGameStart();
}

void GameStartView::menuFrinedCallback( cocos2d::CCObject* obj )
{
	/*拉去好友信息后的界面**/
	_startUI =UILayer::create();

	_startUI->addWidget(GUIReader::shareReader()->widgetFromJsonFile("StartUI_1.json"));

	_startUI->setAnchorPoint(ccp(0.0f,0.0f));
	addChild(_startUI);

	UIButton* btn = (UIButton*)_startUI->getWidgetByName("Button_GO");
	btn->addReleaseEvent(this,coco_releaseselector(GameStartView::menuStartCallback));
	//btn->setFontName(DEFAULT_FONTNAME);
}
