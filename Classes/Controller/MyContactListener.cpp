#include "MyContactListener.h"

void MyContactListener::BeginContact( b2Contact* contact )
{

}

void MyContactListener::EndContact( b2Contact* contact )
{

}

void MyContactListener::PreSolve( b2Contact* contact, const b2Manifold* oldManifold )
{

}

void MyContactListener::PostSolve( b2Contact* contact, const b2ContactImpulse* impulse )
{
	bool isAStone = (contact->GetFixtureA()->GetUserData() != NULL);
	bool isBStone = (contact->GetFixtureB()->GetUserData() != NULL);
	if (isAStone || isBStone)
	{
		// Should the body break?
		int32 count = contact->GetManifold()->pointCount;

		float32 maxImpulse = 0.0f;
		for (int32 i = 0; i < count; ++i)
		{
			maxImpulse = b2Max(maxImpulse, impulse->normalImpulses[i]);
		}

		if (maxImpulse > 1.0f)
		{
			// Flag the enemy(ies) for breaking.
			if (isAStone)
				_contacts.insert(contact->GetFixtureA()->GetBody());
			if (isBStone)
				_contacts.insert(contact->GetFixtureB()->GetBody());
		}
	}
}
