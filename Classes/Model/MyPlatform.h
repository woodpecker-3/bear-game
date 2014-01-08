 #ifndef _BEAR_PLATFORM_H_
 #define _BEAR_PLATFORM_H_
 
 #include "cocos2d.h"
 
 #define kMaxPlatformKeyPoints 100
 #define kPlatformSegmentWidth 5
 #define kMaxPlatformVertices 4000
 #define kMaxBorderVertices 800
 
 class Terrain;
 class MyPlatform :
 	public cocos2d::CCNode
 {
 public:
 	MyPlatform();
 	~MyPlatform();
 
 	static MyPlatform* create(Terrain* terrain);
 	bool init(Terrain* terrain);
 
 	void update(float dt);
 
 	void draw();
 protected:
 	void resetHillVertices();
 private:
 	Terrain* _terrain;
 
 	//����ؼ���
 	cocos2d::CCPoint _hillKeyPoints[kMaxPlatformKeyPoints];
 	int _hillKeyPointCount;//��ǰ����λ��
 	//���ɵ�cos���߶���
 	cocos2d::CCPoint _hillVertices[kMaxPlatformVertices];
 	cocos2d::CCPoint _hillTexCoords[kMaxPlatformVertices];
 	int _hillVerticesCount;
 };
 #endif//_BEAR_PLATFORM_H_
