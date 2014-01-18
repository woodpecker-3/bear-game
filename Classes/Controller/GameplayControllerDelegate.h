#ifndef _GAMEPLAYCONTROLLERDELEGATE_H_
#define _GAMEPLAYCONTROLLERDELEGATE_H_

class GameplayControllerDelegate
{
public:
	GameplayControllerDelegate();
	virtual ~GameplayControllerDelegate();

	//panel layer
	virtual void didPause()=0;
	virtual void didResume()=0;

	//touch layer
	virtual void didTouchBegan()=0;
	virtual void didTouchEnded()=0;
	virtual void didTouchCancelled()=0;

	//menu ayer
	virtual void didReset()=0;
};
#endif// _GAMEPLAYCONTROLLERDELEGATE_H_
