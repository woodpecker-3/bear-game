#include "GameplayModel.h"
#include "Terrain.h"
#include "Hero.h"
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
		//_rotationAngle += 1;
		//_hero->rotation(_rotationAngle);
        float currAngleDegree = _hero->getRotation();
        while (currAngleDegree > 360) {
            currAngleDegree -= 360;
        }
        while (currAngleDegree < -360) {
            currAngleDegree += 360;
        }
        // 12 * 0.5 * 60 = 360
        // 12 degree per frame, 0.5 sec to turn a loop, negative means ccw
        _hero->setRotation(currAngleDegree + (-12));
	}
	else
	{
		_rotationAngle = 0;
	}


	_terrain->update(dt);

	//_platform->update(dt);
	//contact
	processContact();

	//score
	float score = GameplayModel::sharedModel()->getHero()->getPositionX();
	score /= PTM_RATIO;
	BearData::sharedData()->setScore((int)score);
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
	/*速度为0**/
	if(!myContact._linearVelocity.IsValid())
	{
		_hero->damage();
	}
}

void GameplayModel::processContact_Stone(const MyContact& myContact )
{
	b2Body* body = myContact._fixture->GetBody();
	GameObject* obj = (GameObject*)body->GetUserData();
	if (obj)
	{
		/*速度为0,石头碎、hero受到伤害**/
		if ( !myContact._linearVelocity.IsValid() )
		{
			b2Vec2 vel = _hero->getBody()->GetLinearVelocity();
			CCLOG("!!!!GetLinearVelocity(%f,%f)",vel.x,vel.y);

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

			_hero->damage();
		}
		/*冲力大于7，石头碎、hero不受伤害**/
		else if (myContact._impulse > 7.0f)
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
		/*hero滑行而过**/
		else
		{

		}

		//
		
	}
}

void GameplayModel::processContact_Gold(const MyContact& myContact )
{
	b2Body* body = myContact._fixture->GetBody();
	GameObject* obj = (GameObject*)body->GetUserData();
	if (obj)
	{
		//
		b2Vec2 vel = _hero->getBody()->GetLinearVelocity();
		CCLOG("!!!!GetLinearVelocity(%f,%f)",vel.x,vel.y);

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

		BearData::sharedData()->alterGold(10);
	}
}
