#include "GameObject.h"
#include "Defined.h"

USING_NS_CC;
GameObject::GameObject()
{
	_sprite = NULL;
	_objType = kFixtrue_inalid;
}

GameObject::~GameObject()
{

}

GameObject* GameObject::create( int objTpe )
{
	GameObject* obj = new GameObject;
	if (obj && obj->init(objTpe))
	{
		obj->autorelease();
		return obj;
	}
	return NULL;
}

bool GameObject::init( int objTpe )
{
	bool bRet = false;
	do 
	{
		if (objTpe == kFixtrue_Stone)
		{
			_sprite = CCSprite::create("stone.png");
		}
		else if (objTpe == kFixtrue_Gold)
		{
			_sprite = CCSprite::create("gold3.png");
		}
		CC_BREAK_IF(!_sprite);
		addChild(_sprite);

		_objType = objTpe;

		bRet = true;
	} while (0);
	return bRet;
}


