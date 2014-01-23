#ifndef _BEAR_DEFINE_H_
#define _BEAR_DEFINE_H_

#include "cocos2d.h"

#define PTM_RATIO 32.0

/*hero������ж�ʱ����ʹ�Ѿ��뿪�����ˣ������������뻹�Ǻ�С����Ȼ�ж��仹�ڵ���**/
#define FIXED_GROUND_PIXEL 10+16

/*�����ӽ�ʱ��Y������**/
#define CONST_OFFSET_Y 0.5

/*��Ļ���ע����С**/
#define CONST_OUTSIZDE_SCREEN 1/8

/*������������**/
#define CONST_BACKGROUND_SCALE 0.5
/*װ�β�����**/
#define SCALE_BACKGROUND(scale) (1 + ( (scale) - 1)* CONST_BACKGROUND_SCALE)
/*װ�β���������**/
#define CONST_DECORATIVE_SCALE 0.8
/*װ�β�����**/
#define SCALE_DECORATIVE(scale) (1 + ( (scale) - 1)* CONST_DECORATIVE_SCALE)

/*X������С�ٶ�**/
#define CONST_MIN_VELOCITY_X 10
/*Y������С�ٶ�**/
#define CONST_MIN_VELOCITY_Y -40


#define DEFAULT_FONTNAME "American Typewriter"

typedef enum
{
	kFixtrue_inalid = -1,
	kFixtrue_null = 0,
	kFixtrue_Hero,
	kFixtrue_Stone,
	kFixtrue_Gold,
	kFixtrue_Ground,
	kFixtrue_Board,
	kFixtrue_Tree,
}kFixtrueType;

static cocos2d::CCSize targetResourceSize = cocos2d::CCSizeMake(960, 640);
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(960, 640);

/*��ʼƫ��**/

#endif// _BEAR_DEFINE_H_
