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
	//friend class MyPlatform;
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

	//hero�����У�Ϊ�������ӽ�
	//Terrain���෴��x/y�����ƶ���ͬ����
	

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
	b2Body *_body;//�ر�
	vector<b2Body*> _bodyArr;//����ײ���


	//cocos2d::CCTMXTiledMap* _leftMap;
	//cocos2d::CCTMXTiledMap* _rightMap;
	MyMap* _leftMap;
	MyMap* _rightMap;
	//��ͼ1�Ŀ��
	int _LeftMapWidth;
	int _RightMapWidth;
	//��ǰ��ͼ�����һ���ؼ��㣬����һ����ͼ����ʼ��
	cocos2d::CCPoint _lastHillKeyPoint;
	//cocos2d::CCPoint _prepareLastHillKeyPoint;
	cocos2d::CCPoint _prepareFirstHillKeyPoint;

	
	//����ؼ���
	cocos2d::CCPoint _hillKeyPoints[kMaxPlatformKeyPoints];
	int _hillKeyPointIndex;//��ǰ����λ��
	//��ǰ��������
	int _fromKeyPointIndex;
	int _toKeyPointIndex;
	//���ɵ�cos���߶���
	cocos2d::CCPoint _hillVertices[kMaxPlatformVertices];
	cocos2d::CCPoint _hillTexCoords[kMaxPlatformVertices];
	int _hillVerticesCount;
	//����box2d�߶�
	cocos2d::CCPoint _borderVertices[kMaxBorderVertices];
	int _borderVerticesCount;

};
#endif// _TERRAIN_H_
