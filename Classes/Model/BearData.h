#ifndef _BEAR_DATA_H_
#define _BEAR_DATA_H_

#include "cocos2d.h"
class BearData
{
protected:
	BearData();
	~BearData();
	bool init();

public:
    static BearData* sharedData();
    CC_SYNTHESIZE(int , _gold, Gold);
	CC_SYNTHESIZE(int , _score, Score);
    //CC_SYNTHESIZE(bool, _isBeginner, IsBeginner);
    CC_SYNTHESIZE(float, _soundVolume, SoundVolume);
    CC_SYNTHESIZE(float, _musicVolume, MusicVolume);
    
    /**
     *	@brief	 �ڴ治��ʱ����
     */
    void purge();
    
    /**
     *	@brief	 �����ݴ��ڴ汣�浽�ⲿ�ļ���
     */
    void flush();
    
    /**
     *	@brief	 ��Gold�Ĳ���
     *
     *	@param 	delta 	����ֵ����
     */
    void alterGold(int delta);
    /**
     *	@brief	 ������Ϸ����ΪĬ����ֵ
     */
    void reset();
};
#endif// _BEAR_DATA_H_
