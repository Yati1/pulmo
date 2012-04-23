#ifndef PULMO_ACTOR_H
#define PULMO_ACTOR_H

#include "Actor.h"

class PulmoActor : public Actor
{
public:
	enum Color
	{
		red,
		yellow,
		blue,
		purple,
		green,
		orange,
		none
	};

	PulmoActor(Ogre::Entity* entity,Critter::Body* body,ActorManager* manager);
	~PulmoActor();

	void update(float fElapsedTime);

	void onContact(const NxOgre::ContactPair& pair);

	Color getPrimaryColor()const{return m_primaryColor;}
	Color getSecondaryColor()const{return m_secondaryColor;}

	void setPrimaryColor(Color color);
	void mixColor(Color color);
private:

	const float BLEND_RATE;

	void startBlendingBetween(const Ogre::ColourValue& color1,const Ogre::ColourValue& color2);
	//Out = C0 + (C1-C0) * t
	inline float interpolateFloat(const float& c0,const float& c1,const float& t)const;
	Color m_primaryColor;
	Color m_secondaryColor;

	Ogre::ColourValue m_currColor;
	Ogre::ColourValue m_toColor;
	Ogre::ColourValue m_fromColor;
	float m_fBlendT;
	bool m_bIsBlending;
	Ogre::MaterialPtr m_pMaterial;
};

#endif