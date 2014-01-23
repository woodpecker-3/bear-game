#ifndef _HERO_H_
#define _HERO_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"

#define NUM_PREV_VELS 5
class Hero : 
	public cocos2d::CCNode
{
public:
	enum
	{
		kHeroState_invalid = -1,
		kHeroState_normal = 0,
		kHeroState_belly,
		kHeroState_dead
	};
	Hero();
	~Hero();

	static Hero* create(b2World* world);
	bool init(b2World* world);

	bool initHero();

	void update(float dt);

	void limitVelocity();
	void wake();
	void jump();

	void damage();
public:
	int getState(){return _state;}
	void setState(int state);

	bool getAwake(){return _awake;}
	void setAwake(bool awake);

	b2Body* getBody(){return _body;}
protected:
	void createBox2dBody();
protected:
	cocos2d::CCSprite* _sprite;
	b2World* _world;
	b2Body* _body;
	b2Vec2 _prevVels[NUM_PREV_VELS];
	int _nextVel;

	bool _awake;
	int _state;
	cocos2d::CCAnimation *_normalAnim;
	cocos2d::CCAction *_normalAnimate;
	cocos2d::CCAnimation *_bellyAnim;
	cocos2d::CCAction *_bellyAnimate;

	cocos2d::CCSprite* _dustSprite;
	cocos2d::CCAnimation *_dustAnim;
	cocos2d::CCAction *_dustAnimate;

	cocos2d::CCMotionStreak* _strike;
};
#endif// _HERO_H_
