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
     *	@brief	 release memory
     */
    void purge();
    
    /**
     *	@brief	 save data
     */
    void flush();
    
    /**
     *	@brief	 alter gold
	 *
     */
    void alterGold(int delta);
    /**
     *	@brief	 reset date
     */
    void reset();
};
#endif// _BEAR_DATA_H_
