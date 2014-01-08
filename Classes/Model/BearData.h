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
     *	@brief	 内存不足时调用
     */
    void purge();
    
    /**
     *	@brief	 将数据从内存保存到外部文件中
     */
    void flush();
    
    /**
     *	@brief	 对Gold的操作
     *
     *	@param 	delta 	正负值均可
     */
    void alterGold(int delta);
    /**
     *	@brief	 重置游戏数据为默认数值
     */
    void reset();
};
#endif// _BEAR_DATA_H_
