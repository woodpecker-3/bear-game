#include "ResultUI.h"
#include "BearData.h"
#include "GameplayView.h"

USING_NS_CC;
USING_NS_CC_EXT;
ResultUI::ResultUI()
{
	_ul = NULL;
	_score = NULL;
	_gold = NULL;
}

ResultUI::~ResultUI()
{

}

ResultUI* ResultUI::create( GameplayView* view )
{
	ResultUI* ui = new ResultUI;
	if (ui && ui->init(view))
	{
		ui->autorelease();
		return ui;
	}
	return NULL;
}

bool ResultUI::init(GameplayView* view)
{
	bool bRet = false;
	do 
	{
		_view = view;

		_ul =UILayer::create();
		CC_BREAK_IF(!_ul);

		_ul->addWidget(GUIReader::shareReader()->widgetFromJsonFile("ResultUI_1.json"));

		_ul->setAnchorPoint(ccp(0.0f,0.0f));
		addChild(_ul);


		UIButton* btn = (UIButton*)_ul->getWidgetByName("Restart_Button");
		btn->addReleaseEvent(this,coco_releaseselector(ResultUI::clickRestart));


		//_score = (UILabel*)_ul->getWidgetByName("ScoreLabel");
		//_gold = (UILabel*)_ul->getWidgetByName("GoldLabel");

		bRet = true;
	} while (0);
	return bRet;
}

void ResultUI::showResult()
{
	//_score->setText(CCString::createWithFormat("%d",BearData::sharedData()->getScore())->getCString());
	//_gold->setText(CCString::createWithFormat("%d",BearData::sharedData()->getGold())->getCString());
}

void ResultUI::clickRestart( cocos2d::CCObject* sender )
{
	_view->clickRestart();
}
