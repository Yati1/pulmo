#ifndef ACTOR_MANAGER
#define ACTOR_MANAGER

#include <Ogre.h>
#include <vector>
#include <queue>
#include <algorithm>
#include "PhysicsSystem.h"
#include "Actor.h"
#include "Simpleton.h"
/**
	Manages the creation, deletion and storage actors
**/
class ActorManager : public Simpleton<ActorManager>
{
public:
	ActorManager():
		m_pSceneManager(0),
		m_pPhyxScene(0),
		m_pPhyXRenderSys(0),
		m_bActorsDataLocked(false)
	{
		m_Actors.clear();
		m_tempNewActors.clear();
	}
	ActorManager(Ogre::SceneManager* pSceneMan,NxOgre::Scene* pScene,Critter::RenderSystem* pRenderSys);
	virtual ~ActorManager(void);

	void init(Ogre::SceneManager* pSceneMan,NxOgre::Scene* pScene,Critter::RenderSystem* pRenderSys);

	Actor* createActor(Ogre::Entity* ent,NxOgre::ShapeDescription shapeD,Critter::BodyDescription bodyD,Ogre::Vector3 pos=Ogre::Vector3::ZERO,Ogre::Quaternion quat=Ogre::Quaternion::IDENTITY);

	Actor* createActor(Ogre::Entity* ent,Critter::Body* bod);

	void addActor(Actor* toAdd);

	Actor* getActor(Actor* toGet)const;
	Actor* getActor(Critter::Body* toGet)const;
	/**
		Removes the actor

		Note: Do not use this function from a Actor::OnContact callback function because the
			PhysX docs says so.
			Instead use ActorManager::queueToDelete if you want to remove a Actor inside the Actor::OnContact function
	**/
	bool removeActor(Actor* toDel);
	/**
		Put the actor in a queue that gets emptied when the ActorManager::proccessDeleteQueue function is called in ActorManager::update.
	**/
	bool queueToDelete(Actor* toDel);
	/**
		Alt function that searches for the Actor with the passed body
	**/
	bool queueToDelete(Critter::Body* toDel);

	void update(float fElapsedTime);

	void clearAll();

	Actor* getPlayerActor()const{return m_pPlayerActor;};
	void setPlayerActor(Actor* player);

protected:
	typedef std::vector<Actor*> ActorsList;
	bool m_bActorsDataLocked;
	ActorsList m_Actors;
	ActorsList m_tempNewActors;		//since std::vectors iterators get corrupted if the vectors size changed during iteration we add
									//new Actors to this then after m_Actors has finished iterating, append this vector to m_Actors
	typedef std::queue<Actor*> ActorIterQueue;
	ActorIterQueue m_toDelete;

	inline Actor* getActorFromBody(Critter::Body* bod);

	void proccessDeleteQueue();

	NxOgre::Scene* m_pPhyxScene;
	Critter::RenderSystem* m_pPhyXRenderSys;

	Ogre::SceneManager* m_pSceneManager;

	Actor* m_pPlayerActor;

	friend class Simpleton< ActorManager >;
};
#endif