#ifndef _MYCONTACTLISTENER_H_
#define _MYCONTACTLISTENER_H_
#include "Box2D/Box2D.h"
#include "cocos2d.h"
#include <set>

class MyContactListener:public b2ContactListener
{
public:
	virtual void BeginContact(b2Contact* contact);
	/// Called when two fixtures cease to touch.
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
public:
	std::set<b2Body*> _contacts;
};

#endif// _MYCONTACTLISTENER_H_
