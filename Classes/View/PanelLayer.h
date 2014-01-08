#ifndef _PANELLAYER_H_
#define _PANELLAYER_H_

#include "cocos2d.h"

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
	void pauseOrResume(cocos2d::CCObject* pSender);
	//void jump(cocos2d::CCObject* pSender);

protected:
	GameplayView* _parent;

	cocos2d::CCLabelTTF* _scoreLabel;
	int _score;
	cocos2d::CCLabelTTF* _goldLabel;
	int _gold;
};
#endif// _PANELLAYER_H_
