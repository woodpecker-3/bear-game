#include "Terrain.h"
#include "Hero.h"
#include "MyUtil.h"
#include "Defined.h"

USING_NS_CC;
static const char* s_MapArr[]={"down.tmx","down.tmx"};
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
	_leftMap = NULL;
	_rightMap = NULL;
	_hillKeyPointIndex = 0;
	_fromKeyPointIndex = 0;
	_toKeyPointIndex = 0;
	_hillVerticesCount = 0;
	_borderVerticesCount = 0;
}

Terrain::~Terrain()
{

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
		
		_leftMap = new MyMap;
		_leftMap->clear();
		_rightMap = new MyMap;
		_rightMap->clear();
		
		//初始化位置
		_lastHillKeyPoint = CCPointMake(0,_sceenSize.height*3/4);
		createMap();
		resetHillVertices();

		_stripes = CCSprite::create("bg3.png");
		ccTexParams tp2 = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE};
		_stripes->getTexture()->setTexParameters(&tp2);
		_stripes->retain();

		//hero在最顶层
		_hero = hero;
		addChild(_hero,11);
 		_hero->setPosition(CCPointMake(_leftMap->_map->getPosition().x + _prepareFirstHillKeyPoint.x,
 			_leftMap->_map->getPosition().y + _prepareFirstHillKeyPoint.y + _hero->getContentSize().height/4));
		_hero->createBox2dBody();
		//_hero->update(0);

		bRet = true;
	} while (0);
	return bRet;
}

void Terrain::createMap()
{
	if (_rightMap->isValid())
	{
		removeMap(_rightMap);
	}
	_rightMap->_map = CCTMXTiledMap::create(nextMapRes());
	addChild(_rightMap->_map,-1);
	_rightMap->_offsetXWhenCreated = getPosition().x;
	//提出地表第一个关键点
	prepareFirstHillKeyPoint();
	//修正位置(leftmap.x + lastpoint.x,(leftmap.y - (fistpint.y - lastpoint.y)))
	CCPoint leftPoint;
	float leftMapWidth = 0;
	if (_leftMap->isValid())
	{
		leftMapWidth = TMX_WIDTH(_leftMap->_map);
		leftPoint = _leftMap->_map->getPosition();
	}
	else
	{//首张地图
		_hillKeyPoints[_hillKeyPointIndex++] = CCPointMake(-_sceenSize.width/4,_sceenSize.height*3/4);
		leftPoint = CCPointZero;
	}
	
 	_rightMap->_map->setPosition(CCPointMake(leftPoint.x + leftMapWidth - _prepareFirstHillKeyPoint.x,
 		leftPoint.y + _lastHillKeyPoint.y - _prepareFirstHillKeyPoint.y ));

	//提取地表关键点
	prepareHillKeyPoint();

	//创建刚体
	createElementBox2DBody();

	//
	if(!_leftMap->isValid())
	{
		swap(_leftMap,_rightMap);
	}
}

void Terrain::removeMap( MyMap* myMap )
{
	removeChild(myMap->_map,true);
	vector<b2Body*>::iterator it = myMap->_bodyArr.begin();
	for (;it != myMap->_bodyArr.end();++it)
	{
		_world->DestroyBody((*it));
	}
	myMap->clear();
}

void Terrain::resetMap()
{
	//创建tiled地图
	int posX = getPosition().x;
	if (_leftMap->isValid())
	{
		if (_leftMap->_offsetXWhenCreated == 0 )
		{
			if(( -posX + _leftMap->_offsetXWhenCreated )> (TMX_WIDTH(_leftMap->_map) + _sceenSize.width/8))
			{//右边已经离开屏幕左侧
				swap(_leftMap,_rightMap);
				_rightMap->_map->setVisible(false);
				removeMap(_rightMap);
			}
			else if ( ( -posX + _leftMap->_offsetXWhenCreated )> (TMX_WIDTH(_leftMap->_map)- _sceenSize.width*9/8))
			{//右边将要进入屏幕右侧
				if (!_rightMap->isValid())
				{
					createMap();
				}
			}
		}
		else
		{
			if(( -posX + _leftMap->_offsetXWhenCreated )> (TMX_WIDTH(_leftMap->_map) + _sceenSize.width*10/8))
			{//右边已经离开屏幕左侧
				swap(_leftMap,_rightMap);
				_rightMap->_map->setVisible(false);
				removeMap(_rightMap);
			}
			else if ( ( -posX + _leftMap->_offsetXWhenCreated )> (TMX_WIDTH(_leftMap->_map)/*- _sceenSize.width*9/8*/))
			{//右边将要进入屏幕右侧
				if (!_rightMap->isValid())
				{
					createMap();
				}
			}
		}
	}
}

void Terrain::prepareFirstHillKeyPoint()
{
	CCTMXObjectGroup* objGroup = _rightMap->_map->objectGroupNamed("box2d");
	CCDictionary* dict = objGroup->objectNamed("platform");
	//保存值的时候需要同时保存X Y 和polyline 的 points    XY为所画多边形的起始点
	//查找X Y 值 
	int x = ((CCString*)dict->objectForKey("x"))->intValue();
	int y = ((CCString*)dict->objectForKey("y"))->intValue();
	// 如果名字为 “polyline” 读取并保存points值
	CCString* pointStr = (CCString*)dict->objectForKey("polyline");
	const char* beginPos;
	const char* endPos;
	const_string_parser lineParser(pointStr->getCString(),' ');
	if (beginPos=lineParser.parse(&endPos))
	{
		const char* ppbegin;
		const char* ppend;
		const_string_parser pointParser(beginPos,',');
		ppbegin = pointParser.parse(&ppend);

		CCString* xString = CCString::createWithData((const unsigned char*)ppbegin,ppend - ppbegin);
		ppbegin = pointParser.parse(&ppend);
		CCString* yString = CCString::createWithData((const unsigned char*)ppbegin,ppend - ppbegin);

		//CCLog("X=%d,Y=%d",xString->intValue(),yString->intValue());

		_prepareFirstHillKeyPoint = CCPointMake(x + xString->intValue(),y -yString->intValue());
	}
}

void Terrain::prepareHillKeyPoint()
{
	CCPoint offsetPosition = _rightMap->_map->getPosition();

	CCTMXObjectGroup* objGroup = _rightMap->_map->objectGroupNamed("box2d");
	CCDictionary* dict = objGroup->objectNamed("platform");
	//保存值的时候需要同时保存X Y 和polyline 的 points    XY为所画多边形的起始点
	//查找X Y 值 
	int x = ((CCString*)dict->objectForKey("x"))->intValue();
	int y = ((CCString*)dict->objectForKey("y"))->intValue();
	// 如果名字为 “polyline” 读取并保存points值
	CCString* pointStr = (CCString*)dict->objectForKey("polyline");
	const char* beginPos;
	const char* endPos;
	const_string_parser lineParser(pointStr->getCString(),' ');
	while (beginPos=lineParser.parse(&endPos))
	{
		const char* ppbegin;
		const char* ppend;
		const_string_parser pointParser(beginPos,',');
		ppbegin = pointParser.parse(&ppend);

		CCString* xString = CCString::createWithData((const unsigned char*)ppbegin,ppend - ppbegin);
		ppbegin = pointParser.parse(&ppend);
		CCString* yString = CCString::createWithData((const unsigned char*)ppbegin,ppend - ppbegin);

		//CCLog("X=%d,Y=%d",xString->intValue(),yString->intValue());

		_hillKeyPoints[_hillKeyPointIndex++] = ccp(offsetPosition.x +  x + xString->intValue(),offsetPosition.y +  y -yString->intValue() );
		if (_hillKeyPointIndex >= kMaxPlatformKeyPoints)
		{
			_hillKeyPointIndex = 0;
		}
		_lastHillKeyPoint = ccp(x + xString->intValue(),y -yString->intValue());
	}
}

void Terrain::resetHillVertices()
{
	//CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	static int prevFromKeyPointI = -1;
	static int prevToKeyPointI = -1;

	CCPoint tmp = getPosition();
	// key points interval for drawing
	while (_hillKeyPoints[_fromKeyPointIndex + 1].x < (-getPosition().x - _sceenSize.width / 8 / getScale()))
	{
		_fromKeyPointIndex++;
		if (_fromKeyPointIndex >= kMaxPlatformKeyPoints)
		{
			_fromKeyPointIndex=0;
		}
	}
	while (_hillKeyPoints[_toKeyPointIndex].x < ( -getPosition().x + _sceenSize.width * 9 / 8 / getScale()) )
	{
		_toKeyPointIndex++;
		if (_toKeyPointIndex >= kMaxPlatformKeyPoints)
		{
			_toKeyPointIndex=0;
		}
		else if (_toKeyPointIndex == _hillKeyPointIndex)
		{
			break;
		}
	}

	if (prevFromKeyPointI != _fromKeyPointIndex || prevToKeyPointI != _toKeyPointIndex)
	{
		CCLog("_fromKeyPointIndex(%d,%d)",_fromKeyPointIndex,_toKeyPointIndex);
		// vertices for visible area
		_hillVerticesCount = 0;
		_borderVerticesCount = 0;
		CCPoint p0, p1, pt0, pt1;
		p0 = _hillKeyPoints[_fromKeyPointIndex];
		int nextKeyPointIndex = _toKeyPointIndex + 1;
		if (nextKeyPointIndex >= kMaxPlatformKeyPoints)
		{
			nextKeyPointIndex = 0;
		}
		for (int i = _fromKeyPointIndex + 1; i != nextKeyPointIndex ; )
		{
			
			p1 = _hillKeyPoints[i];

			// triangle strip between p0 and p1
			int hSegments = floorf((p1.x - p0.x) / kPlatformSegmentWidth);
			float dx = (p1.x - p0.x) / hSegments;
			float da = M_PI / hSegments;
			float ymid = (p0.y + p1.y) / 2;
			float ampl = (p0.y - p1.y) / 2;
			pt0 = p0;
			_borderVertices[_borderVerticesCount++] = pt0;
			for (int j = 1; j < hSegments + 1; ++j)
			{
				pt1.x = p0.x + j * dx;
				pt1.y = ymid + ampl * cosf(da * j);
				_borderVertices[_borderVerticesCount++] = pt1;

				_hillVertices[_hillVerticesCount] = ccp(pt0.x, 0);
				_hillTexCoords[_hillVerticesCount++] = ccp(pt0.x / 512, 1.0f);
				_hillVertices[_hillVerticesCount] = ccp(pt1.x, 0);
				_hillTexCoords[_hillVerticesCount++] = ccp(pt1.x / 512, 1.0f);

				_hillVertices[_hillVerticesCount] = ccp(pt0.x, pt0.y);
				_hillTexCoords[_hillVerticesCount++] = ccp(pt0.x / 512, 0);
				_hillVertices[_hillVerticesCount] = ccp(pt1.x, pt1.y);
				_hillTexCoords[_hillVerticesCount++] = ccp(pt1.x / 512, 0);

				pt0 = pt1;
			}

			p0 = p1;

			++i;
			if (i >= kMaxPlatformKeyPoints)
			{
				i=0;
			}
			CCLog("_fromKeyPointIndex_i(%d)",i);

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

	b2EdgeShape shape;
	b2Vec2 p1, p2;
	for (int i = 0; i < _borderVerticesCount - 1; ++i)
	{
		p1 = b2Vec2( (_borderVertices[i].x) / PTM_RATIO, (_borderVertices[i].y ) / PTM_RATIO);
		p2 = b2Vec2( (_borderVertices[i + 1].x) / PTM_RATIO, (_borderVertices[i + 1].y ) / PTM_RATIO);
		shape.Set(p1, p2);
		_body->CreateFixture(&shape, 0);
	}
}

void Terrain::createElementBox2DBody()
{
	CCPoint offsetPosition = _rightMap->_map->getPosition();
	//其他刚体
	CCTMXObjectGroup* objGroup = _rightMap->_map->objectGroupNamed("box2d2");
	CCArray* objArr = objGroup->getObjects();
	CCObject* obj=NULL;
	CCDictionary* dict = NULL;
	int count = objArr->count();
	//CCARRAY_FOREACH(objArr,obj);
	for(int i=0;i<count;i++)
	{
		obj = objArr->objectAtIndex(i);
		dict = (CCDictionary*)obj;
		//保存值的时候需要同时保存X Y 和polyline 的 points    XY为所画多边形的起始点
		//查找X Y 值 
		int x = ((CCString*)dict->objectForKey("x"))->intValue();
		int y = ((CCString*)dict->objectForKey("y"))->intValue();
		//创建精灵
		CCSprite* sprite = NULL;
		if (strcmp(dict->valueForKey("name")->getCString(),"stone") == 0)
		{
			string str = dict->valueForKey("canDestroy")->getCString();

			sprite = CCSprite::create("stone.png");
			//sprite->setTag(kTagStone);
			addChild(sprite);
			sprite->setPosition(CCPointMake(offsetPosition.x + x,offsetPosition.y + y));
		}

		//创建刚体
		// 如果名字为 “polyline” 读取并保存points值
		CCString* pointStr = (CCString*)dict->objectForKey("polyline");
		const char* beginPos;
		const char* endPos;
		const_string_parser lineParser(pointStr->getCString(),' ');

		b2BodyDef bd;
		bd.position.Set(0, 0);
		bd.userData = sprite;
		b2Body* body = _world->CreateBody(&bd);

		b2EdgeShape shape;
		b2Vec2 p1, p2;
		int flag = 0;
		while (beginPos=lineParser.parse(&endPos))
		{
			const char* ppbegin;
			const char* ppend;
			const_string_parser pointParser(beginPos,',');
			ppbegin = pointParser.parse(&ppend);

			CCString* xString = CCString::createWithData((const unsigned char*)ppbegin,ppend - ppbegin);
			ppbegin = pointParser.parse(&ppend);
			CCString* yString = CCString::createWithData((const unsigned char*)ppbegin,ppend - ppbegin);

			//CCLog("X=%d,Y=%d",xString->intValue(),yString->intValue());

			if (flag == 0)
			{
				flag++;
				p1 = b2Vec2( (x+xString->intValue()+offsetPosition.x) / PTM_RATIO, (y-yString->intValue() + offsetPosition.y) / PTM_RATIO);
			}
			else
			{
				p2 = b2Vec2( (x+xString->intValue()+offsetPosition.x) / PTM_RATIO, (y-yString->intValue() + offsetPosition.y) / PTM_RATIO);
				shape.Set(p1, p2);

				b2FixtureDef fd;
				fd.shape = &shape;
				fd.density = 1.0f ;
				fd.restitution = 0.0f;
				fd.friction = 0.2f;
				fd.userData = (void*)1;
				//fd.isSensor = true;
				//body->CreateFixture(&shape, 0);
				body->CreateFixture(&fd);
				//fd.isSensor = false;
				//body->CreateFixture(&fd);
				p1 = p2;
			}	
		}
		_rightMap->_bodyArr.push_back(body);
	}
}

void Terrain::update( float dt )
{
	CCNode::update(dt);

	fellow();

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
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, _hillVertices);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, _hillTexCoords);

	glDrawArrays(/*GL_TRIANGLE_STRIP*/GL_LINES, 0, (GLsizei)_hillVerticesCount);
}

void Terrain::fellow()
{
	float offsetX = _sceenSize.width/4 - _hero->getPosition().x * getScale();
	float offsetY = (_sceenSize.height*3/4 - (_hero->getPosition().y + getPosition().y));
	if (offsetY < 0)
	{
		offsetY = 0;
	}
	offsetY = getPosition().y + offsetY*getScale();

	setPosition(ccp(offsetX,offsetY));
}

void Terrain::removeBody( b2Body* body )
{
	_leftMap->_bodyArr.erase(find(_leftMap->_bodyArr.begin(),_leftMap->_bodyArr.end(),body));
	_world->DestroyBody(body);
}
