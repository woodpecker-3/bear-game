#include "Hero.h"
#include "Defined.h"

USING_NS_CC;
Hero::Hero()
{
	_world = NULL;
	_body = NULL;
	_nextVel = 0;
	_awake = false;
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

	float angle = ccpToAngle(ccp(weightedVel.x, weightedVel.y));
	if (_awake)
	{
		setRotation(-1 * CC_RADIANS_TO_DEGREES(angle));
	}
}

void Hero::createBox2dBody()
{
	float radius = 16.0f;
// 	CCSize size = CCDirector::sharedDirector()->getWinSize();
// 	int screenH = size.height;

	//CCPoint startPosition = ccp(size.width / 8, screenH / 2 + radius);
	//CCPoint startPosition = getMainLayer()->getHeroStartPos();

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

	_body->CreateFixture(&fd);
}

void Hero::limitVelocity()
{
	if (!_awake)
	{
		return;
	}

	const float minVelocityX = 5;
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
	_awake = true;
	_body->SetActive(true);
	_body->ApplyLinearImpulse(b2Vec2(1, -2), _body->GetPosition());
}

void Hero::jump()
{
	if (_awake)
	{
		b2Vec2 vel = _body->GetLinearVelocity();
		vel.y = 2;
		_body->ApplyLinearImpulse(vel, _body->GetPosition());
	}
}
