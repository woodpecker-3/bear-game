#ifndef _GAMEPLAYCONTROLLERDELEGATE_H_
#define _GAMEPLAYCONTROLLERDELEGATE_H_

class GameplayControllerDelegate
{
public:
	GameplayControllerDelegate();
	virtual ~GameplayControllerDelegate();

	//来之panellaye的事件
	virtual void didPauseOrResume()=0;
	virtual void didJump()=0;
};
#endif// _GAMEPLAYCONTROLLERDELEGATE_H_
