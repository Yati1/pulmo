#include "PhysicsSystem.h"

PhysicsSystem* Simpleton<PhysicsSystem>::s_singleton =0;

PhysicsSystem::PhysicsSystem(void)
{
		m_pPhysXWorld= NxOgre::World::createWorld();
		NxOgre::ResourceSystem::getSingleton()->openProtocol(new Critter::OgreResourceProtocol());
		m_pPhysXWorld->getRemoteDebugger()->connect("localhost");
		m_pCurrentScene=0;
		m_bDebugRender=false;
		m_bPaused = false;
}

PhysicsSystem::~PhysicsSystem(void)
{
	NxOgre::World::destroyWorld();
}

void PhysicsSystem::update(float fElapsedTime)
{
	if(!m_bPaused)
	{
	//update physics
	//TODO: if it slows down time-step this
		m_pPhysXWorld->advance(fElapsedTime);
	}
}

void PhysicsSystem::createDefaultMaterial(NxOgre::Material* mat)
{
	mat->setRestitution(0.1f);
	mat->setDynamicFriction(0.9);
	mat->setStaticFriction(0.5);
}

void PhysicsSystem::setCallbackFlagsAgainstGroup(Critter::Body* body,unsigned int groupID,unsigned int flags,NxOgre::Scene* scene,Critter::RenderSystem* renderSys)
{
	Critter::RenderSystem::BodyIterator iter = renderSys->getBodies();

	for(iter;iter !=iter.end();iter++)
	{
		if(iter->getGroup()==groupID)
		{
			scene->setActorFlags(body,*iter,flags);
		}
	}
}

NxOgre::Scene* PhysicsSystem::createScene(const NxOgre::SceneDescription& desc, NxOgre::Enums::SceneType type)
{
	m_pCurrentScene = m_pPhysXWorld->createScene(desc,type);
	return m_pCurrentScene;
}

void PhysicsSystem::destroyScene(NxOgre::Scene* scene)
{
	//check whether the scene being deleted is the current scene
	//this check is not needed if we have multiple scenes
	BOOST_ASSERT(scene==m_pCurrentScene);

	m_pPhysXWorld->destroyScene(scene);

	if(!m_pCurrentScene)
		m_pCurrentScene=0;
}

Critter::RenderSystem* PhysicsSystem::createRenderSystem(Ogre::SceneManager* ogreScene)
{
	//this function needs to be called after createScene
	BOOST_ASSERT(m_pCurrentScene);
	m_pCurrentRenderSys = new Critter::RenderSystem(m_pCurrentScene,ogreScene);
	return m_pCurrentRenderSys;
}

void PhysicsSystem::destroyRenderSystem(Critter::RenderSystem* sys)
{
	//check whether the System being deleted is the current system
	//this check is not needed if we have multiple systems
	BOOST_ASSERT(sys == m_pCurrentRenderSys);
	delete sys;

	if(!m_pCurrentRenderSys)
		m_pCurrentRenderSys=0;
}

void PhysicsSystem::toggleDebugRender()
{
	if(m_pCurrentRenderSys)
	{
		m_bDebugRender=!m_bDebugRender;
		if(m_bDebugRender)
			m_pCurrentRenderSys->setVisualisationMode(NxOgre::Enums::VisualDebugger_Collision_AABBs);
		else
			m_pCurrentRenderSys->setVisualisationMode(NxOgre::Enums::VisualDebugger_ShowNone);
	}
}