#ifndef _GAMEPLAYCONTROLLERDELEGATE_H_
#define _GAMEPLAYCONTROLLERDELEGATE_H_

class GameplayControllerDelegate
{
public:
	GameplayControllerDelegate();
	virtual ~GameplayControllerDelegate();

	//��֮panellaye���¼�
	virtual void didPauseOrResume()=0;
	virtual void didTouchBegan()=0;
	virtual void didTouchEnded()=0;
	virtual void didTouchCancelled()=0;

	//����menulayer���¼�
	virtual void didReset()=0;
};
#endif// _GAMEPLAYCONTROLLERDELEGATE_H_
