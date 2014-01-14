#ifndef _BEAR_DEFINE_H_
#define _BEAR_DEFINE_H_

#include "cocos2d.h"

#define PTM_RATIO 32.0

/*hero������ж�ʱ����ʹ�Ѿ��뿪�����ˣ������������뻹�Ǻ�С����Ȼ�ж��仹�ڵ���**/
#define FIXED_GROUND_PIXEL 10+16

typedef enum
{
	kFixtrue_inalid = -1,
	kFixtrue_Ground = 0,
	kFixtrue_Hero,
	kFixtrue_Stone,
	kFixtrue_Gold,
}kFixtrueType;

static cocos2d::CCSize targetResourceSize = cocos2d::CCSizeMake(960, 640);
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(960, 640);

/*��ʼƫ��**/

#endif// _BEAR_DEFINE_H_
