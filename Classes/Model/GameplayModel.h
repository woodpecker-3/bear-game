#ifndef _GAMEMODEL_H_
#define _GAMEMODEL_H_

#include "Box2D/Box2D.h"

class Hero;
class Terrain;
class MyContactListener;
class GameplayModel
{
public:
	GameplayModel();
	~GameplayModel();

	static GameplayModel* sharedModel();
	bool init();

	void update(float dt);


public:
	Hero* getHero(){return _hero;}
	Terrain* getTerrain(){return _terrain;}
	//MyPlatform* getPlatform(){return _platform;}
protected:
	void setupWorld();
private:
	b2World* _world;
	Hero* _hero;
	Terrain* _terrain;
	MyContactListener* _contactListener;
};
#endif// _GAMEMODEL_H_
