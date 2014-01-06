#include "PanelLayer.h"
#include "GameplayView.h"

USING_NS_CC;

PanelLayer::PanelLayer()
{
	_parent = NULL;
}

PanelLayer::~PanelLayer()
{

}

PanelLayer* PanelLayer::create( GameplayView* view )
{
	PanelLayer* layer = new PanelLayer;
	if (layer && layer->init(view))
	{
		layer->autorelease();
		return layer;
	}
	return NULL;
}

bool PanelLayer::init(GameplayView* view)
{
	bool bRet = false;
	do 
	{
		_parent = view;

		//ÔİÍ£°´Å¥
		CCMenuItemSprite* pause = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("CloseNormal.png"), CCSprite::createWithSpriteFrameName("CloseSelected.png"),this,menu_selector(PanelLayer::pauseOrResume));
		CCMenu* menu = CCMenu::create(pause, NULL);

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCSize menuSize = pause->getContentSize();

		menu->setPosition(winSize.width - menuSize.width,winSize.height - menuSize.height);
		addChild(menu);

		//ÌøÔ¾°´Å¥
		pause = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("CloseNormal.png"), CCSprite::createWithSpriteFrameName("CloseSelected.png"),this,menu_selector(PanelLayer::jump));
		menu = CCMenu::create(pause, NULL);
		menu->setPosition(winSize.width - menuSize.width,/*winSize.height -*/ menuSize.height);
		addChild(menu);

		bRet = true;
	} while (0);
	return  bRet;
}


void PanelLayer::pauseOrResume( cocos2d::CCObject* pSender )
{
	_parent->pauseOrResume();
}

void PanelLayer::jump( cocos2d::CCObject* pSender )
{
	_parent->jump();
}



