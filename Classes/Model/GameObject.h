#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "cocos2d.h"

class GameObject :
	public cocos2d::CCNode
{
public:
	GameObject();
	~GameObject();

	static GameObject* create(int objTpe);
	bool init(int objTpe);

	int getObjType(){return _objType;}
private:
	cocos2d::CCSprite* _sprite;
	int _objType;
};
#endif// _GAMEOBJECT_H_
