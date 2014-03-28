#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"

#define kMaxPlatformKeyPoints 50
#define kPlatformSegmentWidth 5
#define kMaxBorderVertices 1000


class Hero;
class GameObject;
class Ornamental;
class Terrain :
	public cocos2d::CCNode
{
public:
	friend class GameplayModel;
	friend class Hero;
	typedef struct  _MyMap
	{
		cocos2d::CCTMXTiledMap* _map;
		vector<b2Body*> _bodyArr;
		vector<GameObject*> _objArr;

		_MyMap(){clear();}

		void clear()
		{
			_map = NULL;
			_bodyArr.clear();
			_objArr.clear();
		}

		bool isValid(){return (_map!=NULL);}
	}MyMap;

	Terrain();
	~Terrain();

	static Terrain* create(b2World* world,Hero* hero);
	bool init(b2World* world,Hero* hero);
	bool initTerrain();

	void update(float dt);

	void draw();

	void removeGameObject(GameObject* obj);
	void removeBody(b2Body* body);

protected:
	cocos2d::CCSprite* createStripe();
	void createMap();
	void removeMap(MyMap* myMap);
	void resetMap();
	void prepareFirstHillKeyPoint(MyMap* myMap);
	void prepareHillKeyPoint(MyMap* myMap);
	void resetHillVertices();
	void resetTerrainBox2DBody();
	void createElementBox2DBody(MyMap* myMap);
	void createOrnamental(MyMap* myMap);
	
	void fellow();
protected:
	cocos2d::CCSprite* _stripes;
	cocos2d::CCSize _sceenSize;

	Ornamental* _ornamental;

	Hero* _hero;

	b2World *_world;
	/*地表**/
	b2Body *_body;
	/*可碰撞物件**/
	vector<b2Body*> _bodyArr;

	/*repeat map**/
	list<MyMap*> _runningMapList;
	list<MyMap*> _freeMapList;
//  	MyMap* _leftMap;
//  	MyMap* _rightMap;
	/*当前地图的最后一个关键点，是下一个地图的起始点**/
	cocos2d::CCPoint _lastHillKeyPoint;
	cocos2d::CCPoint _prepareFirstHillKeyPoint;

	
	/*地面关键点**/
	cocos2d::CCPoint _hillKeyPoints[kMaxPlatformKeyPoints];
	/*当前插入位置**/
	int _hillKeyPointIndex;
	/*当前绘制区域**/
	int _fromKeyPointIndex;
	int _toKeyPointIndex;
	/*生成的cos曲线顶点**/
	vector<cocos2d::CCPoint> _hillVerticesArr;
	vector<cocos2d::CCPoint> _hillTexCoordsArr;
	/*地面box2d线段**/
	vector<cocos2d::CCPoint> _borderVerticesArr;

	/*效果**/
	cocos2d::CCParticleSystemQuad* _snow;

};
#endif// _TERRAIN_H_
