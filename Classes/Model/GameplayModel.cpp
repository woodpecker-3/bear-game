#include "GameplayModel.h"
#include "Terrain.h"
#include "Hero.h"
#include "MyContactListener.h"

USING_NS_CC;
GameplayModel::GameplayModel()
{
	_world = NULL;
	_terrain = NULL;
	_contactListener = NULL;
}

GameplayModel::~GameplayModel()
{
	CC_SAFE_DELETE(_world);
}

GameplayModel* GameplayModel::sharedModel()
{
	static GameplayModel* sGameplayMode=NULL;
	if(!sGameplayMode)
	{
		GameplayModel* model = new GameplayModel;
		if (model && model->init())
		{
			sGameplayMode = model;
		}
	}
	return sGameplayMode;
}

bool GameplayModel::init()
{
	bool bRet = false;
	do 
	{
		setupWorld();

		_hero = Hero::create(_world);
		CC_BREAK_IF(!_hero);

		_terrain = Terrain::create(_world,_hero);
		CC_BREAK_IF(!_terrain);

// 		_platform = MyPlatform::create(_terrain);
// 		CC_BREAK_IF(!_platform);

		bRet = true;
	} while (0);
	return bRet;
}

void GameplayModel::setupWorld()
{
	b2Vec2 gravity = b2Vec2(0.0f, -7.0f);
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

	_hero->update(dt);

	_terrain->update(dt);

	//Åö×²
	{
		for(std::set<b2Body*>::iterator pos=_contactListener->_contacts.begin();pos!=_contactListener->_contacts.end();pos++)
		{
			//toDestroy.insert(*it);
			b2Body* body = *pos;
			CCSprite* sprite = (CCSprite*)body->GetUserData();
			if (sprite)
			{
				CCParticleSun *explosion = CCParticleSun::createWithTotalParticles(200);
				explosion->retain();
				explosion->setTexture(CCTextureCache::sharedTextureCache()->textureForKey("fire.png"));
				//explosion->initWithTotalParticles(200);
				explosion->setAutoRemoveOnFinish(true);
				explosion->setStartSizeVar(10.0f);
				explosion->setSpeed(70.0f);
				explosion->setAnchorPoint(ccp(0.5f, 0.5f));
				explosion->setPosition(sprite->getPosition());
				explosion->setDuration(1.0f);


				_terrain->addChild(explosion, 11);
				explosion->release();
				_terrain->removeChild(sprite);
				_terrain->removeBody(body);
			}
		}
		_contactListener->_contacts.clear();
	}
}

