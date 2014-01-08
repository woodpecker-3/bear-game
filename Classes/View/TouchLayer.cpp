#include "TouchLayer.h"
#include "GameplayView.h"

USING_NS_CC;
TouchLayer::TouchLayer()
{
	_parent = NULL;
}

TouchLayer::~TouchLayer()
{
}

TouchLayer* TouchLayer::create(GameplayView* view)
{
	TouchLayer* layer = new TouchLayer;
	if (layer && layer->init(view))
	{
		layer->autorelease();
		return layer;
	}
	return NULL;
}

bool TouchLayer::init(GameplayView* view)
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(! CCLayer::init());

		_parent = view;

		setTouchEnabled(true);
		bRet = true;
	} while (0);

	return bRet;
}

bool TouchLayer::ccTouchBegan( CCTouch* touch, CCEvent* event )
{
	_parent->touchBegan();
	return true;
}

void TouchLayer::ccTouchEnded( CCTouch* touch, CCEvent* event )
{
	_parent->touchEnded();
}

void TouchLayer::ccTouchCancelled( CCTouch* touch, CCEvent* event )
{
	_parent->touchCancelled();
}

void TouchLayer::setTouchEnabled( bool flag )
{
	if (m_bTouchEnabled != flag)
	{
		m_bTouchEnabled = flag;
		if(flag)
		{
			CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
		}
		else
		{
			CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
		}
	}
}

