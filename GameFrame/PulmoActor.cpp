#include "PulmoActor.h"

PulmoActor::PulmoActor(Ogre::Entity* entity,Critter::Body* body,ActorManager* manager) :
	Actor(entity,body,manager),
	m_bIsBlending(false),
	m_fBlendT(0.0f),
	m_toColor(Ogre::ColourValue::Black),
	m_fromColor(Ogre::ColourValue::Black),
	m_currColor(Ogre::ColourValue::Black),
	BLEND_RATE(1.0f)
{
	setPrimaryColor(red);
	m_secondaryColor = none;	//the secondary color is empty at start

	m_pEntity->setMaterialName("GhostlyBody");
}
PulmoActor::~PulmoActor()
{
}

void PulmoActor::update(float fElapsedTime)
{
	//we are currently blending colored
	if(m_bIsBlending)
	{
		float r = interpolateFloat(m_fromColor.r,m_toColor.r,m_fBlendT);
		float g = interpolateFloat(m_fromColor.g,m_toColor.g,m_fBlendT);
		float b = interpolateFloat(m_fromColor.b,m_toColor.b,m_fBlendT);

		m_currColor = Ogre::ColourValue(r,g,b,0.0f);
		m_currColor.saturate();

		Ogre::MaterialPtr ghostlyMat = Ogre::MaterialManager::getSingleton().getByName("GhostlyBody");
		ghostlyMat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("ColorRGBA",m_currColor);
		ghostlyMat->getTechnique(0)->getPass(1)->getFragmentProgramParameters()->setNamedConstant("ColorRGBA",m_currColor);

		if(m_currColor == m_toColor)
		{
			m_bIsBlending = false;
			m_fBlendT = 0.0f;
		}

		if(m_fBlendT < 1.0f)
			m_fBlendT = m_fBlendT + BLEND_RATE * fElapsedTime;
		else
		{
			m_bIsBlending = false;
			m_fBlendT = 0.0f;
		}
	}
}

float PulmoActor::interpolateFloat(const float& c0,const float& c1,const float& t)const
{
	return c0 + (c1 - c0) * t;
}

void PulmoActor::onContact(const NxOgre::ContactPair& pair)
{
}

void PulmoActor::setPrimaryColor(Color color)
{
	Color old = m_primaryColor;
	m_primaryColor = color;

	Ogre::ColourValue cl;
	Ogre::ColourValue clOld;

	switch(m_primaryColor)
	{
	case red:
		cl = Ogre::ColourValue(1.0f ,0.0f, 0.0, 0.0f);
		break;
	case blue:
		cl = Ogre::ColourValue(0.0f ,0.0f, 1.0, 0.0f);
		break;
	case yellow:
		cl = Ogre::ColourValue(1.0f ,1.0f, 0.0, 0.0f);
		break;
	}

	if(old != m_primaryColor)
	{
		switch(old)
		{
		case red:
			clOld = Ogre::ColourValue(1.0f ,0.0f, 0.0, 0.0f);
			break;
		case blue:
			clOld = Ogre::ColourValue(0.0f ,0.0f, 1.0, 0.0f);
			break;
		case yellow:
			clOld = Ogre::ColourValue(1.0f ,1.0f, 0.0, 0.0f);
			break;
		}
		startBlendingBetween(clOld,cl);
	}
}

void PulmoActor::mixColor(Color color)
{
	if(color == m_primaryColor)				//if
	{
		m_secondaryColor = m_primaryColor;
		return;
	}

	switch(m_primaryColor)
	{
	case red:
		if(color == yellow)
			m_secondaryColor = orange;
		else
			m_secondaryColor = purple;
		break;
	case yellow:
		if(color == red)
			m_secondaryColor = orange;
		else
			m_secondaryColor = green;
		break;
	case blue:
			if(color == red)
				m_secondaryColor = orange;
			else
				m_secondaryColor = green;
		break;
	default:
		assert(false);
	}
}

void PulmoActor::startBlendingBetween(const Ogre::ColourValue& color1,const Ogre::ColourValue& color2)
{
	m_fromColor = color1;
	m_toColor = color2;
	m_currColor = color1;
	m_fBlendT = 0.0f;

	m_bIsBlending = true;
}