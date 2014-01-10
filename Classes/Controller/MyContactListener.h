#ifndef _MYCONTACTLISTENER_H_
#define _MYCONTACTLISTENER_H_
#include "Box2D/Box2D.h"
#include "cocos2d.h"
#include <set>

typedef struct _MyContact
{
	typedef struct _MyContact self;

	b2Fixture* _fixture;
	float32 _impulse;/*夹具受到的冲量**/
	b2Vec2 _linearVelocity;/*碰撞瞬间hero的线性速度**/

	bool operator < (const self& r) const
	{
		return (_fixture < r._fixture);
	}
}MyContact;

class MyContactListener:public b2ContactListener
{
public:
	friend class GameplayModel;
	friend class Hero;

	virtual void BeginContact(b2Contact* contact);
	/// Called when two fixtures cease to touch.
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	void process();
private:
	std::set<MyContact> _contacts;
};

#endif// _MYCONTACTLISTENER_H_
