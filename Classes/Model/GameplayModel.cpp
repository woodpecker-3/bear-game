#include "GameplayModel.h"
#include "Terrain.h"
#include "Hero.h"
#include "Defined.h"
#include "BearData.h"
#include "GameObject.h"
#include "Background.h"

USING_NS_CC;
static GameplayModel* s_GameplayMode=NULL;
GameplayModel::GameplayModel()
{
	_world = NULL;
	_contactListener = NULL;
	_terrain = NULL;
	_background = NULL;
	_hero = NULL;
	_tapDown = false;
	_terrainScale = 1;
	_strike = NULL;
}

GameplayModel::~GameplayModel()
{
	CC_SAFE_DELETE(_world);
	CC_SAFE_RELEASE_NULL(_background);
	CC_SAFE_RELEASE_NULL(_terrain);
	CC_SAFE_RELEASE_NULL(_hero);
	CC_SAFE_DELETE(s_GameplayMode);
}

GameplayModel* GameplayModel::sharedModel()
{
	if(!s_GameplayMode)
	{
		GameplayModel* model = new GameplayModel;
		if (model && model->init())
		{
			s_GameplayMode = model;
		}
	}
	return s_GameplayMode;
}

bool GameplayModel::init()
{
	bool bRet = false;
	do 
	{
		setupWorld();

		_hero = Hero::create(_world);
		_hero->retain();
		_hero->setAnchorPoint(CCPointZero);
		CC_BREAK_IF(!_hero);

		_terrain = Terrain::create(_world,_hero);
		_terrain->retain();
		CC_BREAK_IF(!_terrain);

		_background = Background::create();
		_background->retain();
		CC_BREAK_IF(!_background);

		bRet = true;
	} while (0);
	return bRet;
}

void GameplayModel::setupWorld()
{
	b2Vec2 gravity = b2Vec2(0.0f, -10.0f);
	bool doSleep = true;
	_world = new b2World(gravity);
	_world->SetAllowSleeping(doSleep);

	//contact
 	_contactListener=new MyContactListener();
 	_world->SetContactListener(_contactListener);
}

void GameplayModel::update( float dt )
{
	_hero->limitVelocity();

	static double UPDATE_INTERVAL = 1.0f / 60.0f;
	static double MAX_CYCLES_PER_FRAME = 5;
	static double timeAccumulator = 0;

	timeAccumulator += dt;
	if (timeAccumulator > (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL))
	{
		timeAccumulator = UPDATE_INTERVAL;
	}

	int32 velocityIterations = 3;
	int32 positionIterations = 2;
	while (timeAccumulator >= UPDATE_INTERVAL)
	{
		timeAccumulator -= UPDATE_INTERVAL;
		_world->Step(UPDATE_INTERVAL, velocityIterations, positionIterations);
		_world->ClearForces();
	}
	
	/*�ڵ���ȡ������Ч��**/
	if (isHeroOnTheGround())
	{
		setTapDown(false);
	}
	
	_hero->update(dt);

	_terrain->update(dt);

	_background->fellow(_hero->getPositionX());
	//contact
	processContact();

	//score
	float score = _hero->getPositionX()/PTM_RATIO;
	BearData::sharedData()->setScore((int)score);

	/*��β**/
	if(!isHeroOnTheGround())
	{
		if(!_strike)
		{
			_strike = CCMotionStreak::create(1.0f,/*β�ͳ�����ʱ�� **/
				16.0f,/*β�ʹ�С  **/
				16.0f,/*ͼƬ�Ĵ�С **/ 
				ccWHITE,/*��ɫ **/
				"fire.png"/*ʹ�õ�ͼƬ **/
				);  
			_strike->setAnchorPoint(CCPointZero);
			_terrain->addChild(_strike,12);  
			  
		}
		_strike->setPosition(ccp(_hero->getPositionX(),_hero->getPositionY()));
	}
	else
	{
		if(_strike)
		{
			_terrain->removeChild(_strike,true);    
			_strike = NULL;
		}
	}
}

void GameplayModel::processContact()
{
	for(std::set<MyContact>::iterator pos=_contactListener->_contacts.begin();pos!=_contactListener->_contacts.end();pos++)
	{
		//toDestroy.insert(*it);
		const MyContact& myContact = (*pos);
		int fixtureType = (int)(myContact._fixture->GetUserData());
		switch (fixtureType)
		{
		case kFixtrue_Ground:
			{
				processContact_Ground(myContact);
			}
			break;
		case kFixtrue_Stone:
			{
				processContact_Stone(myContact);
			}
			break;
		case kFixtrue_Gold:
			{
				processContact_Gold(myContact);
			}
			break;
		default:
			break;
		}
	}
	_contactListener->_contacts.clear();
}

bool GameplayModel::isGameOver()
{
	return (_hero->getState()==Hero::kHeroState_dead);
}

bool GameplayModel::isHeroOnTheGround()
{
	int i = 0;
	CCPoint heroPos = _hero->getPosition();
	for (; i < _terrain->_borderVerticesCount - 1; ++i)
	{
		if ( heroPos.x >= _terrain->_borderVertices[i].x && heroPos.x < _terrain->_borderVertices[i+1].x)
		{
			break;
		}
	}
	if ( abs(heroPos.y - _terrain->_borderVertices[i].y )< HERO_GROUND_FIXED_PIXEL ||
		 abs(heroPos.y - _terrain->_borderVertices[i+1].y )< HERO_GROUND_FIXED_PIXEL )
	{
		return true;
	}
	return false;
}

void GameplayModel::processContact_Ground(const MyContact& myContact )
{
	/*�ٶ�Ϊ0**/
// 	float currAngleDegree = _hero->getRotation();
// 
// 	b2Vec2 vel = _hero->getBody()->GetLinearVelocity();
// 	float angle = ccpToAngle(ccp(vel.x, vel.y));
// 	float tarAngleDegree = -1 * CC_RADIANS_TO_DEGREES(angle);
// 	if (currAngleDegree < tarAngleDegree)
// 	{
// 		_hero->damage();
// 	}
}

void GameplayModel::processContact_Stone(const MyContact& myContact )
{
	b2Body* body = myContact._fixture->GetBody();
	GameObject* obj = (GameObject*)body->GetUserData();
	if (obj)
	{
		do 
		{
			/*�ٶ�Ϊ0,ʯͷ�顢hero�ܵ��˺�**/
			if ( myContact._linearVelocity.x <= 0.0f &&
				myContact._linearVelocity.y <= 0.0f)
			{
				_hero->damage();
			}
			/*��������7��ʯͷ�顢hero�����˺�**/
			else if (myContact._impulse > 7.0f)
			{
				
			}
			else
			{
				break;
			}

			CCParticleExplosion *explosion = CCParticleExplosion::createWithTotalParticles(100);
			explosion->retain();
			explosion->setTexture(CCTextureCache::sharedTextureCache()->textureForKey("fire.png"));
			//explosion->initWithTotalParticles(200);
			explosion->setAutoRemoveOnFinish(true);
			explosion->setStartSizeVar(10.0f);
			explosion->setSpeed(200.0f);
			explosion->setAnchorPoint(ccp(0.5f, 0.5f));
			explosion->setPosition(obj->getPosition());
			explosion->setDuration(0.005f);
			//explosion->setEndRadius(128.0f);

			_terrain->addChild(explosion, 11);
			explosion->release();

			_terrain->removeChild(obj);
			_terrain->removeBody(body);

		} while (0);
	}
}

void GameplayModel::processContact_Gold(const MyContact& myContact )
{
	b2Body* body = myContact._fixture->GetBody();
	GameObject* obj = (GameObject*)body->GetUserData();
	if (obj)
	{
		//
		//b2Vec2 vel = _hero->getBody()->GetLinearVelocity();
		//CCLOG("!!!!GetLinearVelocity(%f,%f)",vel.x,vel.y);

		CCParticleSun *explosion = CCParticleSun::createWithTotalParticles(100);
		explosion->retain();
		explosion->setTexture(CCTextureCache::sharedTextureCache()->textureForKey("fire.png"));
		//explosion->initWithTotalParticles(200);
		explosion->setAutoRemoveOnFinish(true);
		explosion->setStartSizeVar(10.0f);
		explosion->setSpeed(70.0f);
		explosion->setAnchorPoint(ccp(0.5f, 0.5f));
		explosion->setPosition(obj->getPosition());
		explosion->setDuration(0.5f);

// 		/*����һ��CCParticleSystemQuadϵͳ��ÿ��������4����(Quad,����)��ʾ������ϵͳ **/ 
// 		CCParticleSystemQuad *emitter = CCParticleSystemQuad::create("ExplodingRing.plist");
// 		emitter->setBlendAdditive(false);/*�Ƿ��� **/
// 		emitter->setAutoRemoveOnFinish(true);
// 		emitter->setEmissionRate(75.0f);
// 		//emitter->stopSystem();
// 		emitter->setPosition(obj->getPosition());

		_terrain->addChild(explosion, 11);
		explosion->release();

		_terrain->removeChild(obj);
		_terrain->removeBody(body);

		BearData::sharedData()->alterGold(10);
	}
}
