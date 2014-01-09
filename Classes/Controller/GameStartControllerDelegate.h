#ifndef _GAMESTARTCONTROLLERDELEGATE_H_
#define _GAMESTARTCONTROLLERDELEGATE_H_


class GameStartControllerDelegate
{
public:
	GameStartControllerDelegate(void);
	virtual ~GameStartControllerDelegate(void);

	virtual void didGameInitializationCompleted()=0;

	virtual void didGameStart()=0;
};
#endif// _GAMESTARTCONTROLLERDELEGATE_H_


