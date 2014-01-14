#ifndef _GAMEMODEL_H_
#define _GAMEMODEL_H_

#include "Box2D/Box2D.h"
#include "cocos2d.h"
#include "MyContactListener.h"

class Hero;
class Terrain;
class Background;
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
public:
	Background* getBackground(){return _background;}
	Hero* getHero(){return _hero;}
	Terrain* getTerrain(){return _terrain;}
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

	Background* _background;
	Hero* _hero;
	Terrain* _terrain;

	//
	cocos2d::CCMotionStreak* _strike;
};
#endif// _GAMEMODEL_H_
