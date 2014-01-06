// #include "MyPlatform.h"
// #include "Terrain.h"
// 
// USING_NS_CC;
// MyPlatform::MyPlatform()
// {
// 	_terrain = NULL;
// 	_hillKeyPointCount = 0;
// 	_hillVerticesCount = 0;
// }
// 
// MyPlatform::~MyPlatform()
// {
// 
// }
// 
// MyPlatform* MyPlatform::create( Terrain* terrain )
// {
// 	MyPlatform* obj = new MyPlatform;
// 	if (obj && obj->init(terrain))
// 	{
// 		obj->autorelease();
// 		return obj;
// 	}
// 	return NULL;
// }
// 
// bool MyPlatform::init( Terrain* terrain )
// {
// 	bool bRet = false;
// 	do 
// 	{
// 		CC_BREAK_IF(!CCNode::init());
// 		setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));
// 
// 		_terrain = terrain;
// 		addChild(_terrain);
// 		CCSize size = CCDirector::sharedDirector()->getWinSize();
// 		_terrain->setPosition(size.width/4,0);
// 
// 		bRet = true;
// 	} while (0);
// 	return bRet;
// }
// 
// void MyPlatform::update( float dt )
// {
// 	_hillKeyPointCount = 0;
// 	_hillVerticesCount = 0;
// 
// 	for (int i= 0;i < _terrain->_hillKeyPointIndex;++i)
// 	{
// 		_hillKeyPoints[_hillKeyPointCount++] = ccp(_terrain->_hillKeyPoints[i].x + _terrain->getPosition().x,
// 			_terrain->_hillKeyPoints[i].y - _terrain->getPosition().y);
// 	}
// 
// 	resetHillVertices();
// }
// 
// void MyPlatform::draw()
// {
// 	CCNode::draw();
// // 	CC_NODE_DRAW_SETUP();
// // 
// // 	ccGLBindTexture2D(_terrain->_stripes->getTexture()->getName());
// // 	ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords);
// // 
// // 	ccDrawColor4F(1.0f, 1.0f, 1.0f, 1.0f);
// // 	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, _hillVertices);
// // 	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, _hillTexCoords);
// // 
// // 	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)_hillVerticesCount);
// }
// 
// void MyPlatform::resetHillVertices()
// {
// 	int _fromKeyPointIndex = 0;
// 	int _toKeyPointIndex = _hillKeyPointCount;
// 	{
// 		// vertices for visible area
// 		_hillVerticesCount = 0;
// 		CCPoint p0, p1, pt0, pt1;
// 		p0 = _hillKeyPoints[_fromKeyPointIndex];
// 		for (int i = _fromKeyPointIndex + 1; i < _toKeyPointIndex + 1; ++i)
// 		{
// 			p1 = _hillKeyPoints[i];
// 
// 			// triangle strip between p0 and p1
// 			int hSegments = floor((p1.x - p0.x) / kPlatformSegmentWidth);
// 			float dx = (p1.x - p0.x) / hSegments;
// 			float da = M_PI / hSegments;
// 			float ymid = (p0.y + p1.y) / 2;
// 			float ampl = (p0.y - p1.y) / 2;
// 			pt0 = p0;
// 			for (int j = 1; j < hSegments + 1; ++j)
// 			{
// 				pt1.x = p0.x + j * dx;
// 				pt1.y = ymid + ampl * cosf(da * j);
// 
// 				_hillVertices[_hillVerticesCount] = ccp(pt0.x, 0);
// 				_hillTexCoords[_hillVerticesCount++] = ccp(pt0.x / 512, 1.0f);
// 				_hillVertices[_hillVerticesCount] = ccp(pt1.x, 0);
// 				_hillTexCoords[_hillVerticesCount++] = ccp(pt1.x / 512, 1.0f);
// 
// 				_hillVertices[_hillVerticesCount] = ccp(pt0.x, pt0.y);
// 				_hillTexCoords[_hillVerticesCount++] = ccp(pt0.x / 512, 0);
// 				_hillVertices[_hillVerticesCount] = ccp(pt1.x, pt1.y);
// 				_hillTexCoords[_hillVerticesCount++] = ccp(pt1.x / 512, 0);
// 
// 				pt0 = pt1;
// 			}
// 
// 			p0 = p1;
// 		}
// 
// 		// 		prevFromKeyPointI = _fromKeyPointI;
// 		// 		prevToKeyPointI = _toKeyPointI;
// 		//resetTerrainBox2DBody();
// 	}
// }
// 
// 
