#include "PanelLayer.h"
#include "GameplayView.h"
#include "Defined.h"

USING_NS_CC;
USING_NS_CC_EXT;
PanelLayer::PanelLayer()
{
	_parent = NULL;
	_ul = NULL;
	_scoreLabel = NULL;
	_score = -1;
	_goldLabel = NULL;
	_gold = -1;
	_tarScoreLabel =NULL;
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

		_ul =UILayer::create();
		CC_BREAK_IF(!_ul);

		_ul->addWidget(GUIReader::shareReader()->widgetFromJsonFile("GameplayUI_1.json"));

		//_ul->setAnchorPoint(ccp(0.5,0.5));
		addChild(_ul);
		//CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		//_ul->setPosition(ccp(240,160 ));

		UIButton* btn = (UIButton*)_ul->getWidgetByName("Button_Pause");
		btn->addReleaseEvent(this,coco_releaseselector(PanelLayer::clickPause));

		_scoreLabel = dynamic_cast<UILabel*>(_ul->getWidgetByName("Label_Score"));
		_scoreLabel->setFontName(DEFAULT_FONTNAME);

		_goldLabel = dynamic_cast<UILabel*>(_ul->getWidgetByName("Label_Gold"));
		_goldLabel->setFontName(DEFAULT_FONTNAME);
		_goldLabel->setColor(ccc3(126, 126, 126));

		_tarScoreLabel  = dynamic_cast<UILabel*>(_ul->getWidgetByName("Label_TargetScore"));
		_tarScoreLabel->setFontName(DEFAULT_FONTNAME);
		_tarScoreLabel->setColor(ccc3(126, 126, 126));

		setScoreNumber(0);
		setGoldNumber(0);
		bRet = true;
	} while (0);
	return  bRet;
}


void PanelLayer::clickPause( cocos2d::CCObject* pSender )
{
	_parent->clickPause();
}

void PanelLayer::setScoreNumber( int number )
{
	if (number != _score)
	{
		_score = number;
		_scoreLabel->setText(CCString::createWithFormat("%d",number)->getCString());
	}
}

void PanelLayer::setGoldNumber( int number )
{
	if (number != _gold)
	{
		_gold = number;
		_goldLabel->setText(CCString::createWithFormat("%d",number)->getCString());
	}
	
}

// void PanelLayer::jump( cocos2d::CCObject* pSender )
// {
// 	_parent->jump();
// }



