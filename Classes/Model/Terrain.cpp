#include "Terrain.h"
#include "Hero.h"
#include "MyUtil.h"
#include "Defined.h"
#include "GameObject.h"
#include "GameplayModel.h"

#define CONST_OFFSET_Y 0.5

#define NEXT_HILLKEYPOINT_INDEX(index) (((index) < (kMaxPlatformKeyPoints -1))?(index+1):(0))

USING_NS_CC;
static const char* s_MapArr[]={"huanpo1-1.tmx","huanpo1-2.tmx"};
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
// 	_leftMap = NULL;
// 	_rightMap = NULL;
	_hillKeyPointIndex = 0;
	_fromKeyPointIndex = 0;
	_toKeyPointIndex = 0;
	_hillVerticesCount = 0;
	_borderVerticesCount = 0;
	_cacheScale = 1;
}

Terrain::~Terrain()
{
// 	CC_SAFE_DELETE(_leftMap);
// 	CC_SAFE_DELETE(_rightMap);

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
		
// 		_leftMap = new MyMap;
// 		_leftMap->clear();
// 		_rightMap = new MyMap;
// 		_rightMap->clear();
		
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
		//_hero->update(0);

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

	//
// 	if(!_leftMap->isValid())
// 	{
// 		swap(_leftMap,_rightMap);
// 	}
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
// 	int posX = getPosition().x;
// 	if (_leftMap->isValid())
// 	{
// 		if((_leftMap->_map->getPositionX() + TMX_WIDTH(_leftMap->_map) + posX ) < ( - _sceenSize.width/4))
// 		{/*左边地图右半滑出屏幕8分之区域**/
// 			swap(_leftMap,_rightMap);
// 			_rightMap->_map->setVisible(false);
// 			removeMap(_rightMap);
// 		}
// 		else if ( (_leftMap->_map->getPositionX() + TMX_WIDTH(_leftMap->_map) + posX) < ( _sceenSize.width*5/4))
// 		{/*左边地图左半进入屏幕8分之区域**/
// 			if (!_rightMap->isValid())
// 			{
// 				createMap();
// 			}
// 		}
// 	}
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
	//float scale = GameplayModel::sharedModel()->getTerrainScale();
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
		//CCLog("_fromKeyPointIndex(%d,%d)",_fromKeyPointIndex,_toKeyPointIndex);
		// vertices for visible area
		_hillVerticesCount = 0;
		//_borderVerticesCount = 0;
		_borderVerticesArr.clear();
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
// 			_borderVertices[_borderVerticesCount++] = pt0;
// 			if (_borderVerticesCount >= 990)
// 			{
// 				CCLog("_borderVerticesCount=%d",_borderVerticesCount);
// 			}
			_borderVerticesArr.push_back(pt0);
			
			for (int j = 1; j < hSegments + 1; ++j)
			{
				pt1.x = p0.x + j * dx;
				pt1.y = ymid + ampl * cosf(da * j);
// 				_borderVertices[_borderVerticesCount++] = pt1;
// 				if (_borderVerticesCount >= 990)
// 				{
// 					CCLog("_borderVerticesCount=%d",_borderVerticesCount);
// 				}
				_borderVerticesArr.push_back(pt1);

//  				_hillVertices[_hillVerticesCount] = ccp(pt0.x, /*0*/(pt1.y - _sceenSize.height));
//  				_hillTexCoords[_hillVerticesCount++] = ccp(pt0.x / 512, 1.0f);
//  				_hillVertices[_hillVerticesCount] = ccp(pt1.x,/* 0*/(pt1.y - _sceenSize.height));
//  				_hillTexCoords[_hillVerticesCount++] = ccp(pt1.x / 512, 1.0f);
//  
//  				_hillVertices[_hillVerticesCount] = ccp(pt0.x, pt0.y);
//  				_hillTexCoords[_hillVerticesCount++] = ccp(pt0.x / 512, 0);
//  				_hillVertices[_hillVerticesCount] = ccp(pt1.x, pt1.y);
//  				_hillTexCoords[_hillVerticesCount++] = ccp(pt1.x / 512, 0);
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
			//CCLog("_fromKeyPointIndex_i(%d)",i);

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

// 	b2EdgeShape shape;
// 	b2Vec2 p1, p2;
// 	for (int i = 0; i < _borderVerticesCount - 1; ++i)
// 	{
// 		p1 = b2Vec2( (_borderVertices[i].x) / PTM_RATIO, (_borderVertices[i].y ) / PTM_RATIO);
// 		p2 = b2Vec2( (_borderVertices[i + 1].x) / PTM_RATIO, (_borderVertices[i + 1].y ) / PTM_RATIO);
// 		shape.Set(p1, p2);
// 		//_body->CreateFixture(&shape, 0);
// 		b2FixtureDef fd;
// 		fd.shape = &shape;
// 		fd.userData = (void*)kFixtrue_Ground;
// 		_body->CreateFixture(&fd);
// 	}
	b2ChainShape shape;
	b2Vec2 p1;//, p2;
	b2Vec2 pointes[kMaxBorderVertices];
	int i = 0;
	for (; i < _borderVerticesArr.size()/*_borderVerticesCount*/ /*- 1*/; ++i)
	{
		if (i == 1016)
		{
			CCLog("ddd");
		}
		p1 = b2Vec2( (/*_borderVertices*/_borderVerticesArr[i].x) / PTM_RATIO, (_borderVerticesArr/*_borderVertices*/[i].y ) / PTM_RATIO);
		//p2 = b2Vec2( (_borderVertices[i + 1].x) / PTM_RATIO, (_borderVertices[i + 1].y ) / PTM_RATIO);
		
		pointes[i] = p1;
		//_body->CreateFixture(&shape, 0);
		
	}
	shape.CreateChain(pointes,i);
	b2FixtureDef fd;
	fd.shape = &shape;
	fd.userData = (void*)kFixtrue_Ground;
	_body->CreateFixture(&fd);
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
// 		if (strcmp(dict->valueForKey("name")->getCString(),"stone") == 0 ||
// 			strcmp(dict->valueForKey("name")->getCString(),"gold"))
		{
			string str = dict->valueForKey("canDestroy")->getCString();
			int objType = dict->valueForKey("objType")->intValue();

			obj = GameObject::create(objType);
			//sprite->setTag(kTagStone);
			addChild(obj);
			obj->setPosition(CCPointMake(offsetPosition.x + x,offsetPosition.y + y));
		}

		//刚体关键关键点
        CCArray* points = (CCArray*)dict->objectForKey("points");

		b2BodyDef bd;
		bd.position.Set(0, 0);
		bd.userData = obj;
		b2Body* body = _world->CreateBody(&bd);

		b2EdgeShape shape;
		b2Vec2 p1, p2;
		int flag = 0;
		CCObject* pObj = NULL;
        CCDictionary* point = NULL;
        CCARRAY_FOREACH(points, pObj) {
            point = (CCDictionary*)pObj;
			if (flag == 0)
			{
				flag++;
				p1 = b2Vec2(
                    (x + ((CCString*)point->valueForKey("x"))->intValue() + offsetPosition.x) / PTM_RATIO,
                    (y - ((CCString*)point->valueForKey("y"))->intValue() + offsetPosition.y) / PTM_RATIO
                );
			}
			else
			{
				p2 = b2Vec2(
                    (x + ((CCString*)point->valueForKey("x"))->intValue() + offsetPosition.x) / PTM_RATIO,
                    (y - ((CCString*)point->valueForKey("y"))->intValue() + offsetPosition.y) / PTM_RATIO
                );
				shape.Set(p1, p2);

				b2FixtureDef fd;
				fd.shape = &shape;
				fd.density = 1.0f ;
				fd.restitution = 0.0f;
				fd.friction = 0.2f;
				fd.userData = (void*)obj->getObjType();
				body->CreateFixture(&fd);

				p1 = p2;
			}	
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
	//GameplayModel::sharedModel()->setTerrainScale(scale);
	//setScale(scale);

	resetMap();
	resetHillVertices();
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

	glDrawArrays(GL_TRIANGLE_STRIP/*GL_LINES*/, 0, (GLsizei)/*_hillVerticesCount*/_hillVerticesArr.size());
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
// 		if (_leftMap->isValid())
// 		{
// 			vector<b2Body*>::iterator it = find(_leftMap->_bodyArr.begin(),_leftMap->_bodyArr.end(),body);
// 			if (it!= _leftMap->_bodyArr.end())
// 			{
// 				_leftMap->_bodyArr.erase(it);
// 				break;
// 			}
// 		}
// 
// 		if (_rightMap->isValid())
// 		{
// 			vector<b2Body*>::iterator it = find(_rightMap->_bodyArr.begin(),_rightMap->_bodyArr.end(),body);
// 			if (it!= _rightMap->_bodyArr.end())
// 			{
// 				_rightMap->_bodyArr.erase(it);
// 			}
// 		}
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
// 		if (_leftMap->isValid())
// 		{
// 			vector<GameObject*>::iterator it = find(_leftMap->_objArr.begin(),_leftMap->_objArr.end(),obj);
// 			if (it!= _leftMap->_objArr.end())
// 			{
// 				_leftMap->_objArr.erase(it);
// 				break;
// 			}
// 		}
// 
// 		if (_rightMap->isValid())
// 		{
// 			vector<GameObject*>::iterator it = find(_rightMap->_objArr.begin(),_rightMap->_objArr.end(),obj);
// 			if (it!= _rightMap->_objArr.end())
// 			{
// 				_rightMap->_objArr.erase(it);
// 			}
// 		}
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
