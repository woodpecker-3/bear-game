#ifndef _HERO_H_
#define _HERO_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"

#define NUM_PREV_VELS 5
class Hero : 
	public cocos2d::CCSprite
{
public:
	Hero();
	~Hero();

	static Hero* create(b2World* world);
	bool init(b2World* world);

	void update(float dt);

	void createBox2dBody();

	void wake();
	void jump();
	void limitVelocity();

	CC_SYNTHESIZE_READONLY(bool, _awake, Awake);
protected:
	b2World* _world;
	b2Body* _body;
	b2Vec2 _prevVels[NUM_PREV_VELS];
	int _nextVel;
};
#endif// _HERO_H_