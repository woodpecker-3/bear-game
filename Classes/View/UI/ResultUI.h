#ifndef _BEAR_RESULTUI_H_
#define _BEAR_RESULTUI_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class GameplayView;
class ResultUI :
	public cocos2d::CCNode
{
public:
	ResultUI();
	~ResultUI();

	static ResultUI* create(GameplayView* view);
	bool init(GameplayView* view);

	void showResult();
protected:
	void clickRestart(cocos2d::CCObject* sender);
private:
	cocos2d::extension::UILayer* _ul;
	cocos2d::extension::UILabel* _score;
	cocos2d::extension::UILabel* _gold;

	GameplayView* _view;
};
#endif// _BEAR_RESULTUI_H_
