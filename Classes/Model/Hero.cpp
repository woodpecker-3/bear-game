#include "Hero.h"
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
	_dustAnim = NULL;
	_dustAnimate = NULL;
	_strike = NULL;

	_dustSprite = NULL;
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
		CC_BREAK_IF(!CCNode::init());

		_sprite = CCSprite::createWithSpriteFrameName("bear_stand1.png");
		//_sprite->setAnchorPoint(ccp(0,0));
		CC_BREAK_IF(!_sprite);
		addChild(_sprite);

		_world = world;

		_dustSprite = CCSprite::createWithSpriteFrameName("dust1.png");
		_dustSprite->setAnchorPoint(ccp(1,1));
		addChild(_dustSprite);
		_dustSprite->setVisible(false);
		_dustSprite->setPosition(ccp(2,-9));

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

		_dustAnim = CCAnimation::create();
		_dustAnim->retain();
		//CCSpriteFrame* ppppp = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("dust1.png");
		_dustAnim->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("dust1.png"));
		_dustAnim->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("dust2.png"));
		_dustAnim->setDelayPerUnit(0.1f);

		bRet = true;
	} while (0);
	return bRet;
}

void Hero::update( float dt )
{
	CCNode::update(dt);

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
	float oldAngleDegree = currAngleDegree;
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
		//CCLOG("delta=%f,tarAngleDegree=%f,currAngleDegree=%f",delta,tarAngleDegree,currAngleDegree);
		currAngleDegree += delta;		
	}
	if (currAngleDegree != oldAngleDegree)
	{
		setRotation(currAngleDegree);
	}

	/*拖尾**/
// 	if(!GameplayModel::sharedModel()->isHeroOnTheGround())
// 	{
// 		if(!_strike)
// 		{
// 			_strike = CCMotionStreak::create(1.0f,/*尾巴持续的时间 **/
// 				16.0f,/*尾巴大小  **/
// 				16.0f,/*图片的大小 **/ 
// 				ccRED,/*颜色 **/
// 				"fire.png"/*使用的图片 **/
// 				);  
// 			addChild(_strike,1);  
// 			_strike->setPosition(ccp(240,160));  
// 		}
// 	}
// 	else
// 	{
// 		if(_strike)
// 		{
// 			removeChild(_strike,true);    
// 			_strike = NULL;
// 		}
// 	}
	if (GameplayModel::sharedModel()->isHeroOnTheGround())
	{
		if (!_dustAnimate)
		{
			_dustAnimate = CCRepeatForever::create(CCAnimate::create(_dustAnim));
			_dustSprite->runAction(_dustAnimate);
			_dustSprite->setVisible(true);
		}
	}
	else
	{
		if (_dustAnimate)
		{
			_dustSprite->stopAction(_dustAnimate);
			_dustAnimate = NULL;
			_dustSprite->setVisible(false);
		}
	}
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
					_sprite->stopAction(_bellyAnimate);
					_bellyAnimate = NULL;
				}
				if(!_normalAnimate)
				{
					_normalAnimate = CCRepeatForever::create(CCAnimate::create(_normalAnim));
					_sprite->runAction(_normalAnimate);
				}
			}
			break;
		case kHeroState_belly:
			{
				if (_normalAnimate)
				{
					_sprite->stopAction(_normalAnimate);
				}
				_normalAnimate = NULL;

				_bellyAnimate = CCRepeatForever::create(CCAnimate::create(_bellyAnim));
				_sprite->runAction(_bellyAnimate);
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
