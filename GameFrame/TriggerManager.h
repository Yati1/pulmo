#ifndef TRIGGER_MANAGER_H
#define TRIGGER_MANAGER_H

#include <map>

#include <luabind\luabind.hpp>
#include "PhysicsSystem.h"
#include "ActorManager.h"
#include "Simpleton.h"

class TriggerManager:public NxOgre::Callback,public Simpleton<TriggerManager>
{
public:

	enum TriggerShape
	{
		Cube=0,
		Sphere
	};

	TriggerManager(Ogre::SceneManager* scene,NxOgre::Scene* physScene,ActorManager* manager);

	TriggerManager():
		m_pPhysScene(0),
		m_pScene(0),
		m_pActorManager(0)
	{
	}

	~TriggerManager(void);

	void init(Ogre::SceneManager* scene,NxOgre::Scene* physScene,ActorManager* manager)
	{
		m_pPhysScene=physScene;
		m_pScene=scene;
		m_pActorManager=manager;
	}

	void clearAll();

	NxOgre::Volume* createTriggerVolume(const TriggerShape& shape,const float& size,const luabind::object& lua,const Ogre::Vector3& pos);

	void removeTrigger(NxOgre::Volume* toRemove);

	void onVolumeEvent(NxOgre::Volume* volume, NxOgre::Shape* volumeShape, NxOgre::RigidBody* rigidBody,
                            NxOgre::Shape* rigidBodyShape, unsigned int collisionEvent);

private:

	typedef std::map<NxOgre::Volume*,luabind::object> Volume_LuaTableMap;

	Volume_LuaTableMap m_Vol_Lua_map;

	Ogre::SceneManager* m_pScene;
	NxOgre::Scene* m_pPhysScene;
	ActorManager* m_pActorManager;

	friend class Simpleton< TriggerManager >;
};

#endif