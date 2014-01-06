#ifndef _GAMESTARTCONTROLLERDELEGATE_H_
#define _GAMESTARTCONTROLLERDELEGATE_H_


class GameStartControllerDelegate
{
public:
	GameStartControllerDelegate(void);
	virtual ~GameStartControllerDelegate(void);

	//model状态事件
	virtual void didGameInitializationCompleted()=0;

	//用户输入事件
	virtual void didGameStart()=0;
};
#endif// _GAMESTARTCONTROLLERDELEGATE_H_


