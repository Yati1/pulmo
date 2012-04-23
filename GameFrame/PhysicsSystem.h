#ifndef PHYSICS_SYS_H
#define PHYSICS_SYS_H

//physics headers
#include "NxOgre.h"
#include "Critter.h"

#include "Simpleton.h"

#include "boost\assert.hpp"
/*******************************************************
Manages Physx world, scene and Critter::RenderSystems

Contains helper functions to convert between the NxVector to Ogre::vector
*******************************************************/
class PhysicsSystem:public Simpleton<PhysicsSystem>
{
public:
	PhysicsSystem(void);
	~PhysicsSystem(void);

	void update(float fElapsedTime);

	NxOgre::World* getWorld(){return m_pPhysXWorld;}

	/**
		Create a scene based on the pass parameters
		This funcion is a wrapper of physX's createScene function so the documentation of that applies here

		IMPORTANT: Call this function before createRenderSystem as createRenderSystem needs the NxOgre::Scene this creates
	**/

	NxOgre::Scene* createScene(const NxOgre::SceneDescription& desc= NxOgre::SceneDescription(), NxOgre::Enums::SceneType type= NxOgre::Enums::SceneType_Prinicipal);
	NxOgre::Scene* getScene()
	{
		BOOST_ASSERT(m_pCurrentScene);
		return m_pCurrentScene;
	}
	void destroyScene(NxOgre::Scene* scene);

	/**
		Create a Critter::RenderSystem for the passed Ogre Scene

		IMPORTANT: Call createScene before this
	**/
	Critter::RenderSystem* createRenderSystem(Ogre::SceneManager* ogreScene= Ogre::Root::getSingletonPtr()->getSceneManagerIterator().getNext());

	Critter::RenderSystem* getRenderSystem()
	{
		BOOST_ASSERT(m_pCurrentRenderSys);
		return m_pCurrentRenderSys;
	}

	void destroyRenderSystem(Critter::RenderSystem* sys);

	void createDefaultMaterial(NxOgre::Material* mat);

	void setCallbackFlagsAgainstGroup(Critter::Body* body,unsigned int groupID,unsigned int flags,NxOgre::Scene* scene,Critter::RenderSystem* renderSys);

	NxOgre::Vec3 convertOgreVec3(Ogre::Vector3 vec)
	{
		NxOgre::Vec3 v;
		v.x=vec.x;
		v.y=vec.y;
		v.z=vec.z;

		return v;
	}

	NxOgre::Quat convertOgreQuat(Ogre::Quaternion qu)
	{
		NxOgre::Quat q;
		q.w=qu.w;
		q.x=qu.x;
		q.y=qu.y;
		q.z=qu.z;

		return q;
	}

	Ogre::Quaternion convertNxQuat(NxOgre::Quat qu)
	{
		Ogre::Quaternion q;
		q.w=qu.w;
		q.x=qu.x;
		q.y=qu.y;
		q.z=qu.z;

		return q;
	}

	Ogre::Vector3 convertNxVec3(NxOgre::Vec3 v)
	{
		return Ogre::Vector3(v.x,v.y,v.z);
	}
	/**
		Toggles physics debug render
		This function fails silently if no current render system
	*/
	void toggleDebugRender();

	void setPaused(const bool& paused){m_bPaused = paused;}
	bool isPaused()const{return m_bPaused;}

private:
	bool m_bDebugRender;
	bool m_bPaused;

	NxOgre::World* m_pPhysXWorld;

	NxOgre::Scene* m_pCurrentScene;

	Critter::RenderSystem* m_pCurrentRenderSys;

	friend class Simpleton< PhysicsSystem >;
};
#endif