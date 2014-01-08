#include "GameplayModel.h"
#include "Terrain.h"
#include "Hero.h"
#include "MyContactListener.h"
#include "MyPlatform.h"
#include "Defined.h"
#include "BearData.h"
#include "GameObject.h"

USING_NS_CC;
GameplayModel::GameplayModel()
{
	_world = NULL;
	_contactListener = NULL;
	_terrain = NULL;
	_hero = NULL;
	//_platform = NULL;
	_tapDown = false;
	_rotationAngle = 0;
	_terrainScale = 1;
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

	_hero->update(dt);
	if (_tapDown && !isHeroOnTheGround())
	{
		_rotationAngle += 1;
		_hero->rotation(_rotationAngle);
	}
	else
	{
		_rotationAngle = 0;
	}


	_terrain->update(dt);

	//_platform->update(dt);
	//Åö×²
	processContact();

	//»ý·Ö
	float score = GameplayModel::sharedModel()->getHero()->getPositionX();
	score /= PTM_RATIO;
	BearData::sharedData()->setScore((int)score);
}

void GameplayModel::processContact()
{
	for(std::set<b2Body*>::iterator pos=_contactListener->_contacts.begin();pos!=_contactListener->_contacts.end();pos++)
	{
		//toDestroy.insert(*it);
		b2Body* body = *pos;
		GameObject* obj = (GameObject*)body->GetUserData();
		int objType = obj->getObjType();
		if (obj)
		{
			CCParticleSun *explosion = CCParticleSun::createWithTotalParticles(200);
			explosion->retain();
			explosion->setTexture(CCTextureCache::sharedTextureCache()->textureForKey("fire.png"));
			//explosion->initWithTotalParticles(200);
			explosion->setAutoRemoveOnFinish(true);
			explosion->setStartSizeVar(10.0f);
			explosion->setSpeed(70.0f);
			explosion->setAnchorPoint(ccp(0.5f, 0.5f));
			explosion->setPosition(obj->getPosition());
			explosion->setDuration(1.0f);


			_terrain->addChild(explosion, 11);
			explosion->release();
			_terrain->removeChild(obj);
			_terrain->removeBody(body);
		}
		if (objType == kFixtrue_Stone)
		{
			_hero->damage();
		}
		else if (objType == kFixtrue_Gold)
		{
			BearData::sharedData()->alterGold(10);
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
