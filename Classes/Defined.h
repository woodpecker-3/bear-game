#ifndef _BEAR_DEFINE_H_
#define _BEAR_DEFINE_H_

#include "cocos2d.h"

#define PTM_RATIO 32.0

/*hero离地面判断时，即使已经离开地面了，但在离地面距离还是很小，仍然判断其还在地面**/
#define FIXED_GROUND_PIXEL 10+16

typedef enum
{
	kFixtrue_inalid = -1,
	kFixtrue_null = 0,
	kFixtrue_Hero,
	kFixtrue_Stone,
	kFixtrue_Gold,
	kFixtrue_Ground,
	kFixtrue_Board,
}kFixtrueType;

static cocos2d::CCSize targetResourceSize = cocos2d::CCSizeMake(960, 640);
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(960, 640);

/*初始偏移**/

#endif// _BEAR_DEFINE_H_
