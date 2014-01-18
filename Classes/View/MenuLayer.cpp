#include "MenuLayer.h"
#include "GameplayView.h"
#include "BearData.h"

USING_NS_CC;

MenuLayer::MenuLayer()
{
	_parent = NULL;
	_scoreLabel =NULL;
}

MenuLayer::~MenuLayer()
{

}

MenuLayer* MenuLayer::create( GameplayView* view )
{
	MenuLayer* layer = new MenuLayer;
	if (layer && layer->init(view))
	{
		layer->autorelease();
		return layer;
	}
	return NULL;
}

bool MenuLayer::init(GameplayView* view)
{
	bool bRet = false;
	do 
	{
		_parent = view;

		int fontSize = 32;
		CCString* fontName = CCString::create("Thonburi");

		CCMenuItemLabel* resume = CCMenuItemLabel::create(CCLabelTTF::create("Resume", fontName->getCString(), fontSize), this, menu_selector(MenuLayer::resume));
		
		CCMenu* menu = CCMenu::create(resume, NULL);
		menu->alignItemsVerticallyWithPadding(10);
		addChild(menu);

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCLabelTTF* gameover = CCLabelTTF::create("Game Over", "American Typewriter", 64);
		addChild(gameover);
		gameover->setColor(ccc3(126, 126, 126));
		gameover->setPosition(ccp(winSize.width*0.5,winSize.height*0.5 + 84));

		_scoreLabel = CCLabelTTF::create("60", "American Typewriter", 16);
		addChild(_scoreLabel);
		_scoreLabel->setColor(ccc3(126, 126, 126));
		_scoreLabel->setPosition(ccp(winSize.width*0.5,winSize.height*0.5 + 32));

		bRet = true;
	} while (0);
	return bRet;
}

void MenuLayer::resume( cocos2d::CCObject* pSender )
{
	//_parent->resume();
}

void MenuLayer::showResult()
{
	_scoreLabel->setString(CCString::createWithFormat("Score: %d",BearData::sharedData()->getScore())->getCString());
}

