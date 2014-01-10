#include "MyContactListener.h"
#include "Defined.h"
#include "GameplayModel.h"
#include "Hero.h"

void MyContactListener::BeginContact( b2Contact* contact )
{

}

void MyContactListener::EndContact( b2Contact* contact )
{

}

void MyContactListener::PreSolve( b2Contact* contact, const b2Manifold* oldManifold )
{
	/*金币等不能参与碰撞求解**/
	int fixtureTypeA = (int)(contact->GetFixtureA()->GetUserData());
	int fixtureTypeB = (int)(contact->GetFixtureB()->GetUserData());
	if ((fixtureTypeA == kFixtrue_Gold) || 
		(fixtureTypeB == kFixtrue_Gold) )
	{
		contact->SetEnabled(false);

		MyContact myContact;
		
		if (fixtureTypeA == kFixtrue_Gold)
		{
			myContact._fixture =  contact->GetFixtureA();
		}
		else
		{
			myContact._fixture =  contact->GetFixtureB();
		}
		_contacts.insert(myContact);
	}
}

void MyContactListener::PostSolve( b2Contact* contact, const b2ContactImpulse* impulse )
{
	int fixtureTypeA = (int)(contact->GetFixtureA()->GetUserData());
	int fixtureTypeB = (int)(contact->GetFixtureB()->GetUserData());
	if ((fixtureTypeA == kFixtrue_Hero) || 
 		(fixtureTypeB == kFixtrue_Hero) )
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
			// linearVelocity
			b2Vec2 vel = GameplayModel::sharedModel()->getHero()->getBody()->GetLinearVelocity();

			MyContact myContact;
			myContact._impulse = maxImpulse;
			myContact._linearVelocity = vel;
			// Flag the enemy(ies) for breaking.
			if (fixtureTypeA == kFixtrue_Hero)
			{
				myContact._fixture =  contact->GetFixtureB();
			}
			else
			{
				myContact._fixture =  contact->GetFixtureA();
			}
			_contacts.insert(myContact);
		}
	}
}
