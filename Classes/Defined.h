#ifndef _BEAR_DEFINE_H_
#define _BEAR_DEFINE_H_

#include "cocos2d.h"

#define PTM_RATIO 32.0

/*hero离地面判断时，即使已经离开地面了，但在离地面距离还是很小，仍然判断其还在地面**/
#define FIXED_GROUND_PIXEL 10+16

/*锁定视角时，Y轴坐标**/
#define CONST_OFFSET_Y 0.5

/*屏幕外关注区大小**/
#define CONST_OUTSIZDE_SCREEN 1/8

/*背景缩放因子**/
#define CONST_BACKGROUND_SCALE 0.5
/*装饰层缩放**/
#define SCALE_BACKGROUND(scale) (1 + ( (scale) - 1)* CONST_BACKGROUND_SCALE)
/*装饰层缩放因子**/
#define CONST_DECORATIVE_SCALE 0.8
/*装饰层缩放**/
#define SCALE_DECORATIVE(scale) (1 + ( (scale) - 1)* CONST_DECORATIVE_SCALE)

/*X方向最小速度**/
#define CONST_MIN_VELOCITY_X 10
/*Y方向最小速度**/
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

/*初始偏移**/

#endif// _BEAR_DEFINE_H_
