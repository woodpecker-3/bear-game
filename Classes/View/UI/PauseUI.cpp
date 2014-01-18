#include "PauseUI.h"
#include "GameplayView.h"
#include "Defined.h"

USING_NS_CC;
USING_NS_CC_EXT;
PauseUI::PauseUI()
{
	_view = NULL;
}

PauseUI::~PauseUI()
{

}

PauseUI* PauseUI::create( GameplayView* view )
{
	PauseUI* ui = new PauseUI;
	if (ui && ui->init(view))
	{
		ui->autorelease();
		return ui;
	}
	return NULL;
}

bool PauseUI::init( GameplayView* view )
{
	bool bRet = false;
	do 
	{
		_view = view;

		_ul =UILayer::create();
		CC_BREAK_IF(!_ul);

		_ul->addWidget(GUIReader::shareReader()->widgetFromJsonFile("PauseUI_1.json"));
		
		_ul->setAnchorPoint(ccp(0.0f,0.0f));
		addChild(_ul);

		UIButton* btn = (UIButton*)_ul->getWidgetByName("Button_Restart");
		btn->addReleaseEvent(this,coco_releaseselector(PauseUI::clickRestart));
		btn->setFontName(DEFAULT_FONTNAME);

		btn = (UIButton*)_ul->getWidgetByName("Button_Resume");
		btn->addReleaseEvent(this,coco_releaseselector(PauseUI::clickResume));
		btn->setFontName(DEFAULT_FONTNAME);
		

		bRet = true;
	} while (0);
	return bRet;
}

void PauseUI::clickResume( cocos2d::CCObject* sender )
{
	_view->clickResume();
}

void PauseUI::clickRestart( cocos2d::CCObject* sender )
{
	_view->clickRestart();
}

