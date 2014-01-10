#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"

#define kMaxPlatformKeyPoints 100
#define kPlatformSegmentWidth 5
#define kMaxPlatformVertices 4000
#define kMaxBorderVertices 800


class Hero;
class Terrain :
	public cocos2d::CCNode
{
public:
	friend class MyPlatform;
	friend class GameplayModel;
	typedef struct  
	{
		cocos2d::CCTMXTiledMap* _map;
		vector<b2Body*> _bodyArr;
		int _offsetXWhenCreated;

		void clear()
		{
			_map = NULL;
			_bodyArr.clear();
			_offsetXWhenCreated = 0;
		}

		bool isValid(){return (_map!=NULL);}
	}MyMap;

	Terrain();
	~Terrain();

	static Terrain* create(b2World* world,Hero* hero);
	bool init(b2World* world,Hero* hero);

	void update(float dt);

	void draw();

	void removeBody(b2Body* body);
protected:
	void createMap();
	void removeMap(MyMap* myMap);
	void resetMap();
	void prepareFirstHillKeyPoint();
	void prepareHillKeyPoint();
	void resetHillVertices();
	void resetTerrainBox2DBody();
	void createElementBox2DBody();
	
	void fellow();
protected:
	cocos2d::CCSprite* _stripes;
	cocos2d::CCSize _sceenSize;

	Hero* _hero;

	b2World *_world;
	b2Body *_body;/*地表**/
	vector<b2Body*> _bodyArr;/*可碰撞物件**/


	//cocos2d::CCTMXTiledMap* _leftMap;
	//cocos2d::CCTMXTiledMap* _rightMap;
	MyMap* _leftMap;
	MyMap* _rightMap;
	/*地图1的宽度**/
	int _LeftMapWidth;
	int _RightMapWidth;
	/*当前地图的最后一个关键点，是下一个地图的起始点**/
	cocos2d::CCPoint _lastHillKeyPoint;
	//cocos2d::CCPoint _prepareLastHillKeyPoint;
	cocos2d::CCPoint _prepareFirstHillKeyPoint;

	
	/*地面关键点**/
	cocos2d::CCPoint _hillKeyPoints[kMaxPlatformKeyPoints];
	int _hillKeyPointIndex;/*当前插入位置**/
	/*当前绘制区域**/
	int _fromKeyPointIndex;
	int _toKeyPointIndex;
	/*生成的cos曲线顶点**/
	cocos2d::CCPoint _hillVertices[kMaxPlatformVertices];
	cocos2d::CCPoint _hillTexCoords[kMaxPlatformVertices];
	int _hillVerticesCount;
	/*地面box2d线段**/
	cocos2d::CCPoint _borderVertices[kMaxBorderVertices];
	int _borderVerticesCount;

};
#endif// _TERRAIN_H_
