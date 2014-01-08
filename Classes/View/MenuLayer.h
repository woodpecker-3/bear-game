#ifndef _MENULAYER_H_
#define _MENULAYER_H_

#include "cocos2d.h"

class GameplayView;
class MenuLayer :
	public cocos2d::CCLayer
{
public:
	MenuLayer();
	~MenuLayer();

	static MenuLayer* create(GameplayView* view);
	bool init(GameplayView* view);

	void showResult();
protected:
	void resume(cocos2d::CCObject* pSender);
	
private:
	GameplayView* _parent;

	cocos2d::CCLabelTTF* _scoreLabel;
};
#endif// _MENULAYER_H_
