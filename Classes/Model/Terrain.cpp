#include "Terrain.h"
#include "Hero.h"
#include "MyUtil.h"
#include "Defined.h"
#include "GameObject.h"
#include "GameplayModel.h"

#define CONST_OFFSET_Y 0.5

#define NEXT_HILLKEYPOINT_INDEX(index) (((index) < (kMaxPlatformKeyPoints -1))?(index+1):(0))

USING_NS_CC;
static const char* s_MapArr[]={"huanpo1-2.tmx","huanpo1-1.tmx"};
const char* nextMapRes()
{
	static int _nextMapIndex = -1;
	++_nextMapIndex;
	if (_nextMapIndex >= sizeof(s_MapArr)/sizeof(const char*))
	{
		_nextMapIndex = 0;
	}
	return s_MapArr[_nextMapIndex];
}

Terrain::Terrain()
{
	_hero = NULL;
	_world = NULL;
	_body = NULL;
	_hillKeyPointIndex = 0;
	_fromKeyPointIndex = 0;
	_toKeyPointIndex = 0;
	_cacheScale = 1;
	_snow = NULL;
}

Terrain::~Terrain()
{
	/*回收内存**/
// 	for (vector<MyMap*>::iterator it = _runningMapArr.begin();
// 		it != _runningMapArr.end();
// 		++it )
// 	{
// 		removeChild((*it)->_map);
// 		CC_SAFE_DELETE((*it));
// 	}
// 	_runningMapArr.clear();
// 	for (vector<MyMap*>::iterator it = _freeMapArr.begin();
// 		it != _freeMapArr.end();
// 		++it )
// 	{
// 		CC_SAFE_DELETE((*it));
// 	}
// 	_freeMapArr.clear();
}

Terrain* Terrain::create(b2World* world,Hero* hero)
{
	Terrain* terrain = new Terrain;
	if (terrain && terrain->init(world,hero))
	{
		terrain->autorelease();
		return terrain;
	}
	return NULL;
}

bool Terrain::init(b2World* world,Hero* hero)
{
	bool bRet = true;
	do 
	{
		CC_BREAK_IF(!CCNode::init());

		setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));

		_world = world;
		
		_sceenSize = CCDirector::sharedDirector()->getWinSize();
		
		//初始点
		_lastHillKeyPoint = CCPointMake(0,_sceenSize.height*CONST_OFFSET_Y);
		createMap();
		resetHillVertices();

		_stripes = CCSprite::create("stripe.png");
		ccTexParams tp2 = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE};
		_stripes->getTexture()->setTexParameters(&tp2);
		_stripes->retain();

		//hero
		_hero = hero;
		addChild(_hero,11);
		MyMap* fisrtMap = _runningMapList.front();
 		_hero->setPosition(CCPointMake(fisrtMap->_map->getPosition().x + _prepareFirstHillKeyPoint.x,
 			fisrtMap->_map->getPosition().y + _prepareFirstHillKeyPoint.y + _hero->getContentSize().height/2+32));
		_hero->createBox2dBody();

		bRet = true;
	} while (0);
	return bRet;
}

void Terrain::createMap()
{
	MyMap* myMap=NULL;
	if(_freeMapList.size() > 0)
	{
		myMap = _freeMapList.front();
		_freeMapList.pop_front();
	}
	else
	{
		myMap = new MyMap;
	}

	myMap->_map = CCTMXTiledMap::create(nextMapRes());
	addChild(myMap->_map,-1);
	/*提取第一个点**/
	prepareFirstHillKeyPoint(myMap);
	/*下一张地图第一个点和上一张地图最后一个点重合**/
	CCPoint leftPoint;
	float leftMapWidth = 0;
	if (_runningMapList.size() > 0)
	{
		MyMap* lastMap = _runningMapList.back();
		leftMapWidth = TMX_WIDTH(lastMap->_map);
		leftPoint = lastMap->_map->getPosition();
	}
	else
	{/*第一个点会左移，补上一个点**/
		_hillKeyPoints[_hillKeyPointIndex++] = CCPointMake(-_sceenSize.width/4,_sceenSize.height*3/4);
		leftPoint = CCPointZero;
	}
	
 	myMap->_map->setPosition(CCPointMake(leftPoint.x + leftMapWidth - _prepareFirstHillKeyPoint.x,
 		leftPoint.y + _lastHillKeyPoint.y - _prepareFirstHillKeyPoint.y ));

	/*提取地面关键点**/
	prepareHillKeyPoint(myMap);

	/*创建障碍物刚体**/
	createElementBox2DBody(myMap);

	/*push into running list**/
	_runningMapList.push_back(myMap);
}

void Terrain::removeMap( MyMap* myMap )
{
	/*remove map**/
	removeChild(myMap->_map,true);

	/*remove body**/
	for (vector<b2Body*>::iterator it = myMap->_bodyArr.begin();it != myMap->_bodyArr.end();++it)
	{
		_world->DestroyBody((*it));
	}
	

	/*remove obj**/
	for (vector<GameObject*>::iterator it = myMap->_objArr.begin();it != myMap->_objArr.end();++it)
	{
		removeChild((*it),true);
	}

	/*clear**/
	myMap->clear();
}

void Terrain::resetMap()
{
	if (_runningMapList.size() > 0)
	{
		//float scale = 1;//GameplayModel::sharedModel()->getTerrainScale();
		MyMap* firstMap = _runningMapList.front();
		if((firstMap->_map->getPositionX() + TMX_WIDTH(firstMap->_map) + getPositionX() ) < ( - _sceenSize.width / 8 / _cacheScale ))
		{/*左边地图右半滑出屏幕8分之区域**/
			removeMap(firstMap);
			_runningMapList.remove(firstMap);
			_freeMapList.push_back(firstMap);
		}
		
		MyMap* lastMap = _runningMapList.back();
		if ( (lastMap->_map->getPositionX() + TMX_WIDTH(lastMap->_map) + getPositionX() ) < ( _sceenSize.width * 9 / 8 /_cacheScale ))
		{/*左边地图左半进入屏幕8分之区域**/
			createMap();
		}
	}
}

void Terrain::prepareFirstHillKeyPoint(MyMap* myMap)
{
	CCTMXObjectGroup* objGroup = myMap->_map->objectGroupNamed("platform");
	CCDictionary* dict = objGroup->objectNamed("platform");
	/*对象的位置**/
	int x = ((CCString*)dict->objectForKey("x"))->intValue();
	int y = ((CCString*)dict->objectForKey("y"))->intValue();
	/*刚体关键点**/
    CCArray* points = (CCArray*)dict->objectForKey("points");
    if (points && points->count() > 0) {
        CCDictionary* point = (CCDictionary*)(points->objectAtIndex(0));
        _prepareFirstHillKeyPoint = CCPointMake(
            x + ((CCString*)point->valueForKey("x"))->intValue(),
            y - ((CCString*)point->valueForKey("y"))->intValue()
        );
    }
}

void Terrain::prepareHillKeyPoint(MyMap* myMap)
{
	CCPoint offsetPosition = myMap->_map->getPosition();

	CCTMXObjectGroup* objGroup = myMap->_map->objectGroupNamed("platform");
	CCDictionary* dict = objGroup->objectNamed("platform");
	/*对象位置**/
	int x = ((CCString*)dict->objectForKey("x"))->intValue();
	int y = ((CCString*)dict->objectForKey("y"))->intValue();
	/*刚体关键点**/
    CCArray* points = (CCArray*)dict->objectForKey("points");
    CCObject* pObj = NULL;
    CCDictionary* point = NULL;
    CCARRAY_FOREACH(points, pObj) {
        point = (CCDictionary*)pObj;
        _hillKeyPoints[_hillKeyPointIndex++] = ccp(
            offsetPosition.x +  x + ((CCString*)point->valueForKey("x"))->intValue(),
            offsetPosition.y +  y - ((CCString*)point->valueForKey("y"))->intValue()
        );
        if (_hillKeyPointIndex >= kMaxPlatformKeyPoints)
        {
            _hillKeyPointIndex = 0;
        }
        _lastHillKeyPoint = ccp(
            x + ((CCString*)point->valueForKey("x"))->intValue(),
            y - ((CCString*)point->valueForKey("y"))->intValue()
        );
    }
}

void Terrain::resetHillVertices()
{
	//CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	static int prevFromKeyPointI = -1;
	static int prevToKeyPointI = -1;

	CCPoint tmp = getPosition();
	// key points interval for drawing
	while ( (_hillKeyPoints[NEXT_HILLKEYPOINT_INDEX(_fromKeyPointIndex)].x + tmp.x ) < ( - _sceenSize.width / 8 / _cacheScale ))
	{
		++_fromKeyPointIndex;
		if (_fromKeyPointIndex >= kMaxPlatformKeyPoints)
		{
			_fromKeyPointIndex=0;
		}
	}
	int lastToKeyPointIndex = (_hillKeyPointIndex == 0)?( kMaxPlatformKeyPoints - 1):( _hillKeyPointIndex - 1);
	while ( (_hillKeyPoints[_toKeyPointIndex].x + tmp.x ) < ( _sceenSize.width * 9 / 8 / _cacheScale ) )
	{
		++_toKeyPointIndex;
		if (_toKeyPointIndex >= kMaxPlatformKeyPoints)
		{
			_toKeyPointIndex=0;
		}
		else if (_toKeyPointIndex == lastToKeyPointIndex)
		{
			break;
		}
	}


	if (prevFromKeyPointI != _fromKeyPointIndex || prevToKeyPointI != _toKeyPointIndex)
	{
		// vertices for visible area
		_borderVerticesArr.clear();
		_hillVerticesArr.clear();
		_hillTexCoordsArr.clear();

		CCPoint p0, p1, pt0, pt1;
		p0 = _hillKeyPoints[_fromKeyPointIndex];
		int nextKeyPointIndex = NEXT_HILLKEYPOINT_INDEX(_toKeyPointIndex);
		for (int i = NEXT_HILLKEYPOINT_INDEX(_fromKeyPointIndex); 
			i != nextKeyPointIndex ; )
		{
			p1 = _hillKeyPoints[i];

			// triangle strip between p0 and p1
			int hSegments = floorf((p1.x - p0.x) / kPlatformSegmentWidth);
			float dx = (p1.x - p0.x) / hSegments;
			float da = M_PI / hSegments;
			float ymid = (p0.y + p1.y) / 2;
			float ampl = (p0.y - p1.y) / 2;
			pt0 = p0;
			_borderVerticesArr.push_back(pt0);
			
			for (int j = 1; j < hSegments + 1; ++j)
			{
				pt1.x = p0.x + j * dx;
				pt1.y = ymid + ampl * cosf(da * j);
				_borderVerticesArr.push_back(pt1);

				_hillVerticesArr.push_back( ccp(pt0.x, /*0*/(pt1.y - _sceenSize.height)));
				_hillTexCoordsArr.push_back( ccp(pt0.x / 512, 1.0f));
				_hillVerticesArr.push_back( ccp(pt1.x,/* 0*/(pt1.y - _sceenSize.height)));
				_hillTexCoordsArr.push_back( ccp(pt1.x / 512, 1.0f));

				_hillVerticesArr.push_back( ccp(pt0.x, pt0.y));
				_hillTexCoordsArr.push_back( ccp(pt0.x / 512, 0));
				_hillVerticesArr.push_back( ccp(pt1.x, pt1.y));
				_hillTexCoordsArr.push_back( ccp(pt1.x / 512, 0));


				pt0 = pt1;
			}

			p0 = p1;

			++i;
			if (i >= kMaxPlatformKeyPoints)
			{
				i=0;
			}
		}

		prevFromKeyPointI = _fromKeyPointIndex;
		prevToKeyPointI = _toKeyPointIndex;
		resetTerrainBox2DBody();
	}
}

void Terrain::resetTerrainBox2DBody()
{
	if (_body)
	{
		_world->DestroyBody(_body);
	}

	b2BodyDef bd;
	bd.position.Set(0, 0);
	bd.userData = this;
	_body = _world->CreateBody(&bd);

	b2ChainShape shape;
	b2Vec2 p1;//, p2;
	b2Vec2* pointesArr = new b2Vec2[_borderVerticesArr.size()];
	int i = 0;
	for (; i < _borderVerticesArr.size(); ++i)
	{
		p1 = b2Vec2( (_borderVerticesArr[i].x) / PTM_RATIO, (_borderVerticesArr[i].y ) / PTM_RATIO);
		
		pointesArr[i] = p1;
	}
	shape.CreateChain(pointesArr,i);
	b2FixtureDef fd;
	fd.shape = &shape;
	fd.userData = (void*)kFixtrue_Ground;
	_body->CreateFixture(&fd);

	//release
	delete[] pointesArr;
}

void Terrain::createElementBox2DBody(MyMap* myMap)
{
	CCPoint offsetPosition = myMap->_map->getPosition();
	/*障碍物layer**/
	CCTMXObjectGroup* objGroup = myMap->_map->objectGroupNamed("colls");
	CCArray* objArr = objGroup->getObjects();
	CCObject* obj=NULL;
	CCDictionary* dict = NULL;
	int count = objArr->count();
	//CCARRAY_FOREACH(objArr,obj);
	for(int i=0;i<count;i++)
	{
		obj = objArr->objectAtIndex(i);
		dict = (CCDictionary*)obj;
		/*对象位置**/
		int x = ((CCString*)dict->objectForKey("x"))->intValue();
		int y = ((CCString*)dict->objectForKey("y"))->intValue();
		GameObject* obj = NULL;
		bool canDestroy = dict->valueForKey("canDestroy")->boolValue();
		int objType = dict->valueForKey("objType")->intValue();
		bool stickSprite = dict->valueForKey("stickSprite")->boolValue();
		if (stickSprite)
		{
			obj = GameObject::create(objType);
			addChild(obj);
			obj->setPosition(CCPointMake(offsetPosition.x + x,offsetPosition.y + y));
		}

		/*刚体关键关键点**/
        CCArray* points = (CCArray*)dict->objectForKey("points");

		b2BodyDef bd;
		bd.position.Set(0, 0);
		bd.userData = obj;
		b2Body* body = _world->CreateBody(&bd);

		
		b2Vec2 p1, p2;
		int flag = 0;
		CCObject* pObj = NULL;
        CCDictionary* point = NULL;
		if (points->count() <= 2)
		{
			point = (CCDictionary*)points->objectAtIndex(0);
			p1 = b2Vec2(
				(x + ((CCString*)point->valueForKey("x"))->intValue() + offsetPosition.x) / PTM_RATIO,
				(y - ((CCString*)point->valueForKey("y"))->intValue() + offsetPosition.y) / PTM_RATIO
				);
			point = (CCDictionary*)points->objectAtIndex(1);
			p2 = b2Vec2(
				(x + ((CCString*)point->valueForKey("x"))->intValue() + offsetPosition.x) / PTM_RATIO,
				(y - ((CCString*)point->valueForKey("y"))->intValue() + offsetPosition.y) / PTM_RATIO
				);

#define CREATE_ELE_BODY_DEF() \
	do \
	{\
		b2FixtureDef fd;\
		fd.shape = &shape;\
		fd.density = 1.0f ;\
		fd.restitution = 0.0f;\
		fd.friction = 0.0f;\
		fd.userData = (void*)objType;\
		body->CreateFixture(&fd);\
	} while (0);
			
			b2EdgeShape shape;
			shape.Set(p1, p2);

			CREATE_ELE_BODY_DEF();
		}
		else
		{
			b2ChainShape shape;
			b2Vec2 pointes[32];
			int pointCount = 0;
			CCARRAY_FOREACH(points, pObj) {
				point = (CCDictionary*)pObj;
				pointes[pointCount++] = b2Vec2(
					(x + ((CCString*)point->valueForKey("x"))->intValue() + offsetPosition.x) / PTM_RATIO,
					(y - ((CCString*)point->valueForKey("y"))->intValue() + offsetPosition.y) / PTM_RATIO
					);
			}
			shape.CreateChain(pointes,pointCount);

			CREATE_ELE_BODY_DEF();
		}
		

		myMap->_bodyArr.push_back(body);
		if(obj)
		{
			myMap->_objArr.push_back(obj);
		}
	}
}

void Terrain::update( float dt )
{
	CCNode::update(dt);

	fellow();

	//scale
	_cacheScale = abs((_sceenSize.height*CONST_OFFSET_Y)/(getPositionY() + _hero->getPositionY()));

	resetMap();
	resetHillVertices();

	if (GameplayModel::sharedModel()->isHeroOnTheGround())
	{
		if (!_snow)
		{
			_snow = CCParticleSystemQuad::create("partilce_snow.plist");
			_snow->setAnchorPoint(CCPointZero);
			_snow->setPositionType(kCCPositionTypeRelative);
			addChild(_snow);
		}
		_snow->setPosition(GameplayModel::sharedModel()->getHero()->getPosition());
	}
	else
	{
		if (_snow)
		{
		 	_hero->removeChild(_snow);
		 	_snow = NULL;
		}
	}
}

void Terrain::draw()
{
	CCNode::draw();
	CC_NODE_DRAW_SETUP();

	ccGLBindTexture2D(_stripes->getTexture()->getName());
	ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords);

	ccDrawColor4F(1.0f, 1.0f, 1.0f, 1.0f);
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, (void*)&_hillVerticesArr[0]);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, (void*)&_hillTexCoordsArr[0]);

	glDrawArrays(GL_TRIANGLE_STRIP/*GL_LINES*/, 0, (GLsizei)_hillVerticesArr.size());
}

void Terrain::fellow()
{
	float offsetX = _sceenSize.width/4 - _hero->getPosition().x * getScale();
	float offsetY = (_sceenSize.height*CONST_OFFSET_Y - (_hero->getPosition().y + getPosition().y));
	if (offsetY < 0)
	{
		offsetY = 0;
	}
	offsetY = getPosition().y + offsetY*getScale();

	setPosition(ccp(offsetX,offsetY));
}

void Terrain::removeBody( b2Body* body )
{
	do 
	{

		MyMap* myMap = NULL;
		for (list<MyMap*>::iterator it = _runningMapList.begin();
			 it!=_runningMapList.end();
			 ++it)
		{
			myMap = (*it);
			if ( myMap->isValid())
			{
				vector<b2Body*>::iterator it = find(myMap->_bodyArr.begin(),myMap->_bodyArr.end(),body);
				if (it!= myMap->_bodyArr.end())
				{
					myMap->_bodyArr.erase(it);
				}
			}
		}

	} while (0);
	
	_world->DestroyBody(body);
}

void Terrain::removeGameObject( GameObject* obj )
{
	do 
	{
		MyMap* myMap = NULL;
		for (list<MyMap*>::iterator it = _runningMapList.begin();
			it!=_runningMapList.end();
			++it)
		{
			myMap = (*it);
			if ( myMap->isValid())
			{
				vector<GameObject*>::iterator it = find(myMap->_objArr.begin(),myMap->_objArr.end(),obj);
				if (it!= myMap->_objArr.end())
				{
					myMap->_objArr.erase(it);
				}
			}
		}
	} while (0);
	removeChild(obj);
}
