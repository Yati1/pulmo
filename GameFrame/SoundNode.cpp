#include "SoundNode.h"
#include "assert.hpp"

SoundNode::SoundNode():m_pSound(0)
	,m_pChannel(0)
	,m_v3Vel(Ogre::Vector3::ZERO)
	,m_v3LastPos(Ogre::Vector3::ZERO)
	,m_bLooping(false)
{
	setListener(&m_listener);

	m_FMOD_pos.x=0;
	m_FMOD_pos.y=0;
	m_FMOD_pos.z=0;

	m_FMOD_vel.x=0;
	m_FMOD_vel.y=0;
	m_FMOD_vel.z=0;
}
SoundNode::SoundNode(const Ogre::String& name,float size,bool looping ):Ogre::MovableObject(name)
	,m_pSound(0)
	,m_pChannel(0)
	,m_bLooping(looping)
	,m_fSize(size)
{
	setListener(&m_listener);

	m_FMOD_pos.x=0;
	m_FMOD_pos.y=0;
	m_FMOD_pos.z=0;

	m_FMOD_vel.x=0;
	m_FMOD_vel.y=0;
	m_FMOD_vel.z=0;
}

SoundNode::SoundNode(const Ogre::String& name,const Ogre::String& soundFile,float size,bool looping):Ogre::MovableObject(name)
	,m_strSoundFile(soundFile)
	,m_pChannel(0)
	,m_bLooping(looping)
	,m_fSize(size)
{
	m_pSound=SoundSystem::GetSingleton()->getSound(m_strSoundFile.c_str());
	setListener(&m_listener);

	m_FMOD_pos.x=0;
	m_FMOD_pos.y=0;
	m_FMOD_pos.z=0;

	m_FMOD_vel.x=0;
	m_FMOD_vel.y=0;
	m_FMOD_vel.z=0;
}

SoundNode::~SoundNode(void)
{
}

void SoundNode::play(bool bLoop,bool bPaused)
{
	FMOD_VECTOR fpos,fvel;

	Ogre::Vector3 pos=Ogre::MovableObject::getParentSceneNode()->getPosition();

	fpos.x=pos.x;
	fpos.y=pos.y;
	fpos.z=pos.z;
	//HACK: should we hold a velocity
	fvel.x=0.0f;
	fvel.y=0.0f;
	fvel.z=0.0f;

	m_pChannel = SoundSystem::GetSingleton()->play3DSound(m_strSoundFile.c_str(),fpos,fvel,m_fSize,bLoop,bPaused);
}

const Ogre::AxisAlignedBox& SoundNode::getBoundingBox(void)const
{
	return Ogre::AxisAlignedBox();
}

Ogre::Real SoundNode::getBoundingRadius(void)const
{
	return Ogre::Real(1.0f);
}

void SoundNode::SoundNodeListener::objectMoved(SoundNode* object)
{
	FMOD_VECTOR pos,vel;
	Ogre::Vector3 vpos;
	pos.x=vpos.x;
	pos.y=vpos.y;
	pos.z=vpos.z;

	vel.x=0.0f;
	vel.y=0.0f;
	vel.z=0.0f;

	object->getChannel()->set3DAttributes(&pos,&vel);
}

void SoundNode::_update(float elapsedTime)
{
	Ogre::Vector3 pos=getParentSceneNode()->getPosition();
	m_v3Vel.x = (pos.x - m_v3LastPos.x) * (1000 / elapsedTime);
    m_v3Vel.y = (pos.y - m_v3LastPos.y) * (1000 / elapsedTime);
    m_v3Vel.z = (pos.z - m_v3LastPos.z) * (1000 / elapsedTime);

	m_FMOD_vel.x=m_v3Vel.x;
	m_FMOD_vel.y=m_v3Vel.y;
	m_FMOD_vel.z=m_v3Vel.z;

	m_FMOD_pos.x=pos.x;
	m_FMOD_pos.y=pos.y;
	m_FMOD_pos.z=pos.z;

	m_v3LastPos=pos;
}

void SoundNode::stop()
{
	m_pChannel->stop();
}