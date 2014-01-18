#include "BearData.h"
//#include "StaticData.h"
USING_NS_CC;

static BearData* _sharedBearData = NULL;
BearData* BearData::sharedData()
{
	if ( !_sharedBearData ){
		BearData* obj = new BearData();
		if ( obj && obj->init() )
		{
			_sharedBearData = obj;
		}
	}
	return _sharedBearData;
}

void BearData::purge()
{
	CC_SAFE_DELETE(_sharedBearData);
}

BearData::BearData()
{
	_gold = 0;
	_score = 0;
}

BearData::~BearData()
{
	this->flush();
}

bool BearData::init()
{
// 	_isBeginner = CCUserDefault::sharedUserDefault()->getBoolForKey("beginner",true);
// 	if(_isBeginner == true){
// 		this->reset();
// 		this->flush();
// 		this->setIsBeginner(false);
// 	}else{
// 		_isBeginner = CCUserDefault::sharedUserDefault()->getBoolForKey("beginner");
// 		_soundVolume = CCUserDefault::sharedUserDefault()->getFloatForKey("sound");
// 		_musicVolume = CCUserDefault::sharedUserDefault()->getFloatForKey("music");
// 		_gold = CCUserDefault::sharedUserDefault()->getIntegerForKey("gold");
// 		CCUserDefault::sharedUserDefault()->purgeSharedUserDefault();
// 	}
	return true;
}
void BearData::reset()
{
// 	int gold = STATIC_DATA_INT("default_gold");
	setGold(0);
	setScore(0);
// 	this->setIsBeginner(true);
// 	this->setMusicVolume(1);
// 	this->setSoundVolume(1);
// 	this->flush();
}
void BearData::alterGold(int delta)
{
	setGold(getGold()+delta);
}
void BearData::flush()
{
// 	CCUserDefault::sharedUserDefault()->setFloatForKey("sound", this->getSoundVolume());
// 	CCUserDefault::sharedUserDefault()->setBoolForKey("beginner", this->getIsBeginner());
// 	CCUserDefault::sharedUserDefault()->setIntegerForKey("gold", this->getGold());
// 	CCUserDefault::sharedUserDefault()->setFloatForKey("music", this->getMusicVolume());
// 	CCUserDefault::sharedUserDefault()->flush();
// 	CCUserDefault::sharedUserDefault()->purgeSharedUserDefault();
}
