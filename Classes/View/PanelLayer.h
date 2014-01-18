#ifndef _PANELLAYER_H_
#define _PANELLAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class GameplayView;
class PanelLayer : 
	public cocos2d::CCLayer
{
public:
	PanelLayer();
	~PanelLayer();

	static PanelLayer* create(GameplayView* view);
	bool init(GameplayView* view);

	 void setScoreNumber(int number);
	 void setGoldNumber(int number);
protected:
	void clickPause(cocos2d::CCObject* pSender);
	//void jump(cocos2d::CCObject* pSender);

protected:
	GameplayView* _parent;

	cocos2d::extension::UILayer* _ul;
	cocos2d::extension::UILabel* _scoreLabel;
	int _score;
	cocos2d::extension::UILabel* _goldLabel;
	int _gold;
	cocos2d::extension::UILabel* _tarScoreLabel;
};
#endif// _PANELLAYER_H_
