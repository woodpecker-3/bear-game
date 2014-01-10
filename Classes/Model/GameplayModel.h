#ifndef _GAMEMODEL_H_
#define _GAMEMODEL_H_

#include "Box2D/Box2D.h"
#include "cocos2d.h"
#include "MyContactListener.h"

class Hero;
class Terrain;
//class MyPlatform;
class GameplayModel
{
public:
	GameplayModel();
	~GameplayModel();

	static GameplayModel* sharedModel();
	bool init();

	void update(float dt);

	bool isGameOver();
	bool isHeroOnTheGround();
	CC_SYNTHESIZE(bool, _tapDown, TapDown);
	CC_SYNTHESIZE(float,_terrainScale,TerrainScale);
public:
	Hero* getHero(){return _hero;}
	Terrain* getTerrain(){return _terrain;}
	//MyPlatform* getPlatform(){return _platform;}
	MyContactListener* getContactListener(){return _contactListener;}
protected:
	void setupWorld();
	void processContact();
	void processContact_Ground(const MyContact& myContact);
	void processContact_Stone(const MyContact& myContact);
	void processContact_Gold(const MyContact& myContact);
private:
	b2World* _world;
	MyContactListener* _contactListener;

	Hero* _hero;
	Terrain* _terrain;
	//MyPlatform* _platform;

	float _rotationAngle;
};
#endif// _GAMEMODEL_H_
