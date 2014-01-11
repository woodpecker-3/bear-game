﻿#include "Hero.h"
#include "Defined.h"
#include "MyContactListener.h"
#include "GameplayModel.h"

USING_NS_CC;
Hero::Hero()
{
	_world = NULL;
	_body = NULL;
	_nextVel = 0;
	_awake = false;
	_state = kHeroState_invalid;
	_normalAnim = NULL;
	_normalAnimate = NULL;
	_bellyAnim = NULL;
	_bellyAnimate = NULL;
}

Hero::~Hero()
{

}

Hero* Hero::create(b2World* world)
{
	Hero* hero = new Hero;
	if (hero && hero->init(world))
	{
		hero->autorelease();
		return hero;
	}
	return NULL;
}

bool Hero::init(b2World* world)
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCSprite::initWithSpriteFrameName("bear_stand1.png"));

		_world = world;

		//´´½¨¶¯»­
		_normalAnim = CCAnimation::create();
		_normalAnim->retain();
		_normalAnim->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bear_stand1.png"));
		_normalAnim->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bear_stand2.png"));
		//_normalAnim->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bear_stand3.png"));
		_normalAnim->setDelayPerUnit(0.1f);

		_bellyAnim = CCAnimation::create();
		_bellyAnim->retain();
		_bellyAnim->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bear_duck1.png"));
		_bellyAnim->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bear_duck2.png"));
		_bellyAnim->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bear_duck3.png"));
		_bellyAnim->setDelayPerUnit(0.1f);

		bRet = true;
	} while (0);
	return bRet;
}

void Hero::update( float dt )
{
	if (!_awake) return;
	setPosition(ccp(_body->GetPosition().x * PTM_RATIO, _body->GetPosition().y * PTM_RATIO));
	b2Vec2 vel = _body->GetLinearVelocity();
	b2Vec2 weightedVel = vel;

	for (int i = 0; i < NUM_PREV_VELS; ++i)
	{
		weightedVel += _prevVels[i];
	}
	weightedVel = b2Vec2(weightedVel.x / NUM_PREV_VELS, weightedVel.y / NUM_PREV_VELS);
	_prevVels[_nextVel++] = vel;
	if (_nextVel >= NUM_PREV_VELS)
	{
		_nextVel = 0;
	}

	/*在地面or在空中没有主动调整过角度时，程序要自动调整角度**/
	float currAngleDegree = getRotation();
	if (GameplayModel::sharedModel()->getTapDown())
	{
		while (currAngleDegree > 360) {
			currAngleDegree -= 360;
		}
		while (currAngleDegree < -360) {
			currAngleDegree += 360;
		}
		// 12 * 0.5 * 60 = 360
		// 12 degree per frame, 0.5 sec to turn a loop, negative means ccw
		currAngleDegree -= 3;
	}
	else
	{
		float angle = ccpToAngle(ccp(weightedVel.x, weightedVel.y));
		float tarAngleDegree = -1 * CC_RADIANS_TO_DEGREES(angle);
		float delta = (tarAngleDegree - currAngleDegree);
		if (abs(delta) > 2 )
		{
			delta = delta/abs(delta)*2;
		}
		CCLOG("delta=%f,tarAngleDegree=%f,currAngleDegree=%f",delta,tarAngleDegree,currAngleDegree);
		currAngleDegree += delta;		
	}
	setRotation(currAngleDegree);
}

void Hero::createBox2dBody()
{
	float radius = 16.0f;

	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.linearDamping = 0.1f;
	bd.fixedRotation = true;
	bd.position.Set(getPosition().x / PTM_RATIO, getPosition().y / PTM_RATIO);
	bd.userData = this;
	_body = _world->CreateBody(&bd);

	b2CircleShape shape;
	shape.m_radius = radius / PTM_RATIO;

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 1.0f / CC_CONTENT_SCALE_FACTOR();
	fd.restitution = 0.0f;
	fd.friction = 0.2f;
	fd.userData = (void*)kFixtrue_Hero;

	_body->CreateFixture(&fd);
}

void Hero::limitVelocity()
{
	if (!_awake)
	{
		return;
	}

	const float minVelocityX = 8;
	const float minVelocityY = -40;
	b2Vec2 vel = _body->GetLinearVelocity();
	if (vel.x < minVelocityX)
	{
		vel.x = minVelocityX;
	}
	if (vel.y < minVelocityY)
	{
		vel.y = minVelocityY;
	}
	_body->SetLinearVelocity(vel);
}

void Hero::wake()
{
	//_awake = true;
	setAwake(true);
	_body->SetActive(true);
	_body->ApplyLinearImpulse(b2Vec2(1, -2), _body->GetPosition());
}

void Hero::jump()
{
	if (_awake)
	{
		b2Vec2 vel;
		vel.x = 1;
		vel.y = 5;
		_body->ApplyLinearImpulse(vel, _body->GetPosition());
	}
}

void Hero::setState( int state )
{
	if (_state != state)
	{
		_state = state;
		switch (_state)
		{
		case kHeroState_normal:
			{
				if (_bellyAnimate)
				{
					stopAction(_bellyAnimate);
					_bellyAnimate = NULL;
				}
				if(!_normalAnimate)
				{
					_normalAnimate = CCRepeatForever::create(CCAnimate::create(_normalAnim));
					runAction(_normalAnimate);
				}
			}
			break;
		case kHeroState_belly:
			{
				if (_normalAnimate)
				{
					stopAction(_normalAnimate);
				}
				_normalAnimate = NULL;

				_bellyAnimate = CCRepeatForever::create(CCAnimate::create(_bellyAnim));
				runAction(_bellyAnimate);
			}
			break;
		default:
			break;
		}
	}
}

void Hero::setAwake( bool awake )
{
	_awake = awake;
	setState(kHeroState_normal);
}

void Hero::damage()
{
	switch (_state)
	{
	case kHeroState_normal:
		{
			setState(Hero::kHeroState_belly);
		}
		break;
	case kHeroState_belly:
		{
			setState(Hero::kHeroState_dead);
		}
		break;
	default:
		break;
	}
}
