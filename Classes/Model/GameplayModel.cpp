#include "GameplayModel.h"
#include "Terrain.h"
#include "Hero.h"
#include "Defined.h"
#include "BearData.h"
#include "GameObject.h"
#include "Background.h"
#include "Ornamental.h"

USING_NS_CC;
static GameplayModel* s_GameplayMode=NULL;


GameplayModel::GameplayModel()
{
	_world = NULL;
	_contactListener = NULL;
	_terrain = NULL;
	_background = NULL;
	_hero = NULL;
	_ornamental = NULL;
	_tapDown = false;
	_cacheScale = 1;
	//_terrainScale = 1;
	_strike = NULL;
	//_snow = NULL;
	_velocity = NULL;
	_wind = NULL;

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

			if (!s_GameplayMode->initModel())
			{
				CC_SAFE_DELETE(s_GameplayMode);
				s_GameplayMode =NULL;
			}
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

		_ornamental = Ornamental::create();
		_ornamental->retain();
		CC_BREAK_IF(!_ornamental);

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

bool GameplayModel::initModel()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!_terrain->initTerrain());

		CC_BREAK_IF(!_hero->initHero());

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
	
	/*在地面取消按下效果**/
	if (isHeroOnTheGround())
	{
		setTapDown(false);
	}

	//contact
	processContact();
	
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	//scale
	//CCPoint heroWorldPos = _terrain->convertToWorldSpace(_hero->getPosition());
	//_cacheScale = (winSize.height*CONST_OFFSET_Y)/heroWorldPos.y;
	//float YYYY = _terrain->getPositionY() + _hero->getPositionY();
	_cacheScale = (winSize.height*CONST_OFFSET_Y)/(_terrain->getPositionY() + _hero->getPositionY());
	

	_hero->update(dt);

	_terrain->update(dt);

	_ornamental->update(dt);

	_background->update(dt);
	
	//score
	float score = _hero->getPositionX()/PTM_RATIO;
	BearData::sharedData()->setScore((int)score);

	/*拖尾**/
	if(!isHeroOnTheGround() && !_velocity)
	{
		if(!_strike)
		{
			_strike = CCMotionStreak::create(1.0f,/*尾巴持续的时间 **/
				16.0f,/*尾巴大小  **/
				8.0f,/*图片的大小 **/ 
				ccWHITE,/*颜色 **/
				"strike.png"/*使用的图片 **/
				);  
			_strike->setAnchorPoint(CCPointZero);
			_terrain->addChild(_strike,12);  
		}
		_strike->setPosition(_hero->getPosition());
	}
	else
	{
		if(_strike)
		{
			_terrain->removeChild(_strike,true);    
			_strike = NULL;
		}
	}
	{
		float angle = _hero->getRotation();
		b2Vec2 linearVelocity = _hero->getBody()->GetLinearVelocity();
		if ( (angle > 0 && angle < 90)  && !_tapDown && linearVelocity.LengthSquared() >= 150)
		{
			if (!_velocity)
			{
				_velocity = CCParticleSystemQuad::create("particle_streak.plist");
				_velocity->setAnchorPoint(CCPointZero);
				_velocity->setPositionType(kCCPositionTypeFree);
				_hero->addChild(_velocity);
			}
		}
		else
		{
			if (_velocity)
			{
				_hero->removeChild(_velocity);
				_velocity = NULL;
			}
		}

		if ( (angle > 0 && angle < 90) && !_tapDown && linearVelocity.LengthSquared() >= 120)
		{
			if (!_wind)
			{
				_wind = CCParticleSystemQuad::create("particle_wind.plist");
				_wind->setAnchorPoint(CCPointZero);
				_hero->addChild(_wind);
				//_wind->setRotation(_hero->getSprite()->getRotation());
			}
		}
		else
		{
			if (_wind)
			{
				_hero->removeChild(_wind);
				_wind = NULL;
			}
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
		case kFixtrue_Board:
			{
				processContact_Board(myContact);
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
	for (; i < _terrain->_borderVerticesArr.size() - 1; ++i)
	{
		if ( heroPos.x >= _terrain->_borderVerticesArr[i].x && heroPos.x < _terrain->_borderVerticesArr[i+1].x)
		{
			break;
		}
	}
	if ( abs(heroPos.y - _terrain->_borderVerticesArr[i].y )< FIXED_GROUND_PIXEL ||
		abs(heroPos.y - _terrain->_borderVerticesArr[i+1].y )< FIXED_GROUND_PIXEL )
	{
		return true;
	}
	return false;
}

void GameplayModel::processContact_Ground(const MyContact& myContact )
{
	/*速度为0**/
	float currAngleDegree = _hero->getRotation();
	float angle = ccpToAngle(ccp(myContact._linearVelocity.x, myContact._linearVelocity.y));
	float tarAngleDegree = -1 * CC_RADIANS_TO_DEGREES(angle);

	/*速度为0,石头碎、hero受到伤害**/
	if( abs(currAngleDegree - tarAngleDegree) >= 60)
	{
		CCParticleSystemQuad* particle = CCParticleSystemQuad::create("particle_snow2.plist");
		particle->setAutoRemoveOnFinish(true);
		particle->setPosition(_hero->getPosition());
		particle->setPositionType(kCCPositionTypeFree);

		_terrain->addChild(particle, 11);

		_hero->damage();
	}
}

void GameplayModel::processContact_Stone(const MyContact& myContact )
{
	b2Body* body = myContact._fixture->GetBody();
	GameObject* obj = (GameObject*)body->GetUserData();
	if (obj)
	{
		do 
		{
			float currAngleDegree = _hero->getRotation();
			float angle = ccpToAngle(ccp(myContact._linearVelocity.x, myContact._linearVelocity.y));
			float tarAngleDegree = -1 * CC_RADIANS_TO_DEGREES(angle);

			/*速度为0,石头碎、hero受到伤害**/
			if( abs(tarAngleDegree - currAngleDegree) >= 90 )
			{
				_hero->damage();
			}
			/*冲力大于7，石头碎、hero不受伤害**/
			else if (myContact._impulse > 7.0f)
			{
				
			}
			else
			{
				break;
			}

			CCParticleSystemQuad* particle = CCParticleSystemQuad::create("exploding.plist");
			particle->setAutoRemoveOnFinish(true);
			particle->setPosition(obj->getPosition());
			particle->setPositionType(kCCPositionTypeGrouped);

			_terrain->addChild(particle, 11);

			_terrain->removeGameObject(obj);
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
		CCParticleSystemQuad* particle = CCParticleSystemQuad::create("exploding_ring.plist");
		particle->setAutoRemoveOnFinish(true);
		particle->setPosition(obj->getPosition());
		particle->setPositionType(kCCPositionTypeGrouped);

		_terrain->addChild(particle, 11);

		_terrain->removeGameObject(obj);
		_terrain->removeBody(body);

		BearData::sharedData()->alterGold(10);
	}
}

void GameplayModel::processContact_Board( const MyContact& myContact )
{
// 	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
// 	GameObject* obj = (GameObject*)myContact._fixture->GetBody()->GetUserData();
// 	CCPoint nowPos = obj->getPosition();
// 	CCPoint worldPpos = _terrain->convertToWorldSpace(nowPos );
// 	if (worldPpos.y <  (winSize.height * 3 / 4 ))
// 	{
// 		//worldPpos.y /*+= (size.height * 3 / 4 - worldPpos.y + 3)/3*/ = size.height * 3 / 4;
// 
// 		//CCPoint newPos = _terrain->convertToNodeSpace(worldPpos);
// 		float offsetY =(winSize.height * 3 / 4 ) - worldPpos.y;
// 
// 		_terrain->setPosition(ccp(_terrain->getPositionX(),(_terrain->getPositionY() + offsetY)));
// 	}
}
