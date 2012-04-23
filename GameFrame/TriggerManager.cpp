#include "TriggerManager.h"
#include <assert.h>

TriggerManager* Simpleton<TriggerManager>::s_singleton=0;

TriggerManager::TriggerManager(Ogre::SceneManager* scene,NxOgre::Scene* physScene,ActorManager* manager):
	m_pPhysScene(physScene),
	m_pScene(scene),
	m_pActorManager(manager)
{
}
TriggerManager::~TriggerManager(void)
{
	m_Vol_Lua_map.clear();
}

void TriggerManager::clearAll()
{
	m_Vol_Lua_map.clear();
}

NxOgre::Volume* TriggerManager::createTriggerVolume(const TriggerShape& shape,const float& size,const luabind::object& lua,const Ogre::Vector3& pos)
{
	//NxOgre::ShapeDescription sp;
	NxOgre::Volume* vol;

	if(!lua.is_valid())
	{
		Ogre::LogManager::getSingleton().logMessage("TriggerManager: Lua object invalid!");
		return 0;
	}

	switch(shape)
	{
	case Sphere:
		//sp = NxOgre::SphereDescription(size);
		vol = m_pPhysScene->createVolume(NxOgre::SphereDescription(size),PhysicsSystem::GetSingleton()->convertOgreVec3(pos),this);
		break;
	case Cube:
		//sp = NxOgre::BoxDescription(size);
		vol = m_pPhysScene->createVolume(NxOgre::BoxDescription(size),PhysicsSystem::GetSingleton()->convertOgreVec3(pos),this);
		break;
	}
	if(lua.is_valid())
	{
		m_Vol_Lua_map[vol]=lua;
	}else
	{
		Ogre::LogManager::getSingleton().logMessage("TriggerManager: Lua object invalid!");
		return 0;
	}

	return vol;
}

void TriggerManager::removeTrigger(NxOgre::Volume* toRemove)
{
	assert(toRemove!=0);	//if toRemove is null

	m_Vol_Lua_map.erase(toRemove);
	//m_pPhysScene->destroyVolume(toRemove);
}

void TriggerManager::onVolumeEvent(NxOgre::Volume* volume, NxOgre::Shape* volumeShape, NxOgre::RigidBody* rigidBody,
                        NxOgre::Shape* rigidBodyShape, unsigned int collisionEvent)
{
	luabind::object& obj = m_Vol_Lua_map[volume];
	if(collisionEvent == NxOgre::Enums::VolumeCollisionType_OnEnter)		//When a RigidBody enters the volume
	{
		if(obj.is_valid())
		{
			Actor* act =  m_pActorManager->getActor(static_cast<Critter::Body*>(rigidBody));
			assert(act);//check is act is null
			obj["Trigger_OnEnter"](act);	//so the lua side would have a table for each type of Actor and the table would contain functions
		}
	}
	if(collisionEvent == NxOgre::Enums::VolumeCollisionType_OnExit)			//When a RigidBody exits the volume.
	{
	}
	if(collisionEvent == NxOgre::Enums::VolumeCollisionType_OnPresence)		//When a RigidBody is inside the volume (called per timestep).
	{
	}
	if(collisionEvent == NxOgre::Enums::VolumeCollisionType_EnterOrExit)	//When a RigidBody enters or exits
	{
	}
	if(collisionEvent == NxOgre::Enums::VolumeCollisionType_All)			//When a RigidBody enters, exits or inside a Volume
	{
	}
}