#include "MyContactListener.h"
#include "Defined.h"

void MyContactListener::BeginContact( b2Contact* contact )
{
// 	int fixtureTypeA = (int)(contact->GetFixtureA()->GetUserData());
// 	int fixtureTypeB = (int)(contact->GetFixtureB()->GetUserData());
// 
// 	if (fixtureTypeA == kFixtrue_Hero)
// 	{
// 		_heroContacts.insert(contact->GetFixtureB());
// 	}
// 	if (fixtureTypeB == kFixtrue_Hero)
// 	{
// 		_heroContacts.insert(contact->GetFixtureA());
// 	}
}

void MyContactListener::EndContact( b2Contact* contact )
{
// 	int fixtureTypeA = (int)(contact->GetFixtureA()->GetUserData());
// 	int fixtureTypeB = (int)(contact->GetFixtureB()->GetUserData());
// 
// 	if (fixtureTypeA == kFixtrue_Hero)
// 	{
// 		_heroContacts.erase(contact->GetFixtureB());
// 	}
// 	if (fixtureTypeB == kFixtrue_Hero)
// 	{
// 		_heroContacts.erase(contact->GetFixtureA());
// 	}
}

void MyContactListener::PreSolve( b2Contact* contact, const b2Manifold* oldManifold )
{

}

void MyContactListener::PostSolve( b2Contact* contact, const b2ContactImpulse* impulse )
{
	int fixtureTypeA = (int)(contact->GetFixtureA()->GetUserData());
	int fixtureTypeB = (int)(contact->GetFixtureB()->GetUserData());
	if ((fixtureTypeA == kFixtrue_Stone) || 
		(fixtureTypeB == kFixtrue_Stone) )
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
			if (fixtureTypeA == kFixtrue_Stone)
				_contacts.insert(contact->GetFixtureA()->GetBody());
			if (fixtureTypeB == kFixtrue_Stone)
				_contacts.insert(contact->GetFixtureB()->GetBody());
		}
	}
}
