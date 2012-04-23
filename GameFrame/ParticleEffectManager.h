#ifndef PARTICLE_EFFECT_MANAGER_H
#define PARTICLE_EFFECT_MANAGER_H

#define PARTICLE_GEN_ID "PARTICLE_GEN_"

#include <Ogre.h>

#include <list>

#include "Simpleton.h"
/*************************************************************************************
The particleEffectManager can spawn 3 types of particle systems.
	Unmanaged particle system - The ParticleEffectManager does not automatically detect whether the system is active and destroy.
	Managed partile system - The ParticleEffectManager checks whether the number of particles in the system is 0 if so destroys
	Following Particle system - A work around for Critteer destroying the node of the Critter::Body and any attached object along with it
*************************************************************************************/
class ParticleEffectManager:public Simpleton<ParticleEffectManager>,public Ogre::MovableObject::Listener
{
public:
	ParticleEffectManager(void);
	~ParticleEffectManager(void);
	/*
		To create and destroy particle sysstem we need access to the  current Ogre::SceneManager.
		Call this function before you do anything else with the ParticleEffectManager
	*/
	void setSceneManager(Ogre::SceneManager* pManager){m_pSceneManager = pManager;}
	/*
		Get the current Ogre::SceneManager
	*/
	Ogre::SceneManager* getSceneManager(){return m_pSceneManager;}
	/*
		Returns the number of managed particles
	*/
	int getNumManagedParticles()const;
	/*
		Returns the number of particles total
	*/
	int getNumUnManagedParticles()const;
	/*
		Returns the number of managed particles
	*/
	int getTotalParticles()const;
	/*
		Spawn a effect with the particle system name. The system must be defined in a particle script file

		bool managed -	if true the system will be automatically deleted when its particle count reaches 0
						if false the system ill have to be destroyed manually. The system will also be destroyed
						when the scene node its attached to is destroyed
	*/
	Ogre::ParticleSystem* spawnEffect(Ogre::String particleName,Ogre::SceneNode* node,bool managed=true);

	/*
		Spawn a effect then attach it to a scene node at that location and orientation

		bool managed -	if true the system will be automatically deleted when its particle count reaches 0
						if false the system ill have to be destroyed manually. The system will also be destroyed
						when the scene node its attached to is destroyed
	*/
	Ogre::ParticleSystem* spawnEffect(Ogre::String particleName,Ogre::Vector3 pos,Ogre::Quaternion ori,bool managed=true);

	/*
		Spawn a effect has the same location/orientation as the passed object's node but is not a child of it.
		Basically a workaround for Critter deleteing the scene node a Critter::Body is attached to and every child node of that node
	*/
	Ogre::ParticleSystem* spawnFollowEffect(Ogre::String particleName,Ogre::MovableObject* object);

	void removeDeadParticleSystems();
	/*
		Destroy all the particle systems
	*/
	void clearAll();

private:

	unsigned int ID;

	Ogre::String generateID();

	typedef std::list<Ogre::ParticleSystem*> ParticleList;
	//a map, with Ogre::MovableObjects as keys to a list of particle systems attached to its parent node
	typedef std::map<Ogre::MovableObject*,ParticleList> ParticleNodeMap;
	typedef std::map<Ogre::MovableObject*,Ogre::SceneNode*> ObjectNodeMap;

	ParticleList m_Particles;					//all the particles in here

	ParticleList m_managedParticles;			//this is checked for dead particle systems by removeDeadParticleSystems()
	ParticleList m_unManagedParticles;			//particles in this must be manually removed
	ParticleNodeMap m_followParticles;			//particles for the Critter workaround

	ObjectNodeMap m_ObjectNodeMap;				//to retrieve to which scene node the object was attached to before getting detached

	Ogre::SceneManager* m_pSceneManager;

	//callbacks for Ogre::MovableObject::Listener
	void objectDetached(Ogre::MovableObject* object);

	friend class Simpleton< ParticleEffectManager >;
};
#endif