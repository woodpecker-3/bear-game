#ifndef _GAMESTARTMODEL_H_
#define _GAMESTARTMODEL_H_

#include "cocos2d.h"

class GameStartControllerDelegate;
class GameStartModel : 
	public cocos2d::CCObject
{
public:
	GameStartModel(void);
	~GameStartModel(void);

	static GameStartModel* create(GameStartControllerDelegate* theDelegate);
	bool init(GameStartControllerDelegate* theDelegate);

	void gameInitialization();
protected:
	//�첽������Դ��ɻص�
	void loadingCallback(cocos2d::CCObject* obj);
	//������Դ���
	void loadingFinish();

	//������ʡ������Ȼ���
	void cacheInit();
	//������Ч�����������ݳ�ʼ��
	void audioAndUserDataInit();

	//����Ԥ���������
	void gameInitializationCompleted();
protected:
	GameStartControllerDelegate* _delegate;
};

#endif// _GAMESTARTMODEL_H_


