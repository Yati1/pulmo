#ifndef FPS_CAM_ACTOR_H
#define FPS_CAM_ACTOR_H

#include <ogre.h>
#include "InputSystem.h"
#include "PhysicsSystem.h"
#include "Actor.h"
#include "SoundSystem.h"

class FPSCamActor : public Actor
{
public:
	FPSCamActor(Ogre::Entity* entity,Critter::Body* body,ActorManager* manager,Ogre::Camera* cam);
	Ogre::Vector3 getPosition()const{return m_pCamera->getPosition();}
	Ogre::Quaternion getOrientation()const{return m_pCamera->getOrientation();}
	Ogre::Camera* getCamera()const{return m_pCamera;}

	void injectKeyUp(const OIS::KeyEvent& evt);
	void injectKeyDown(const OIS::KeyEvent& evt);
	void injectMouseMove(const OIS::MouseEvent& evt);
	void injectMouseDown(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	void injectMouseUp(const OIS::MouseEvent& evt, OIS::MouseButtonID id);

	void update(float fElapsedTime);

	SoundListener* getSoundListener()const{return m_listener.get();}

private:

	Ogre::Camera* m_pCamera;
	Ogre::Real m_fTopSpeed;
	Ogre::Vector3 m_Velocity;
	bool m_bGoingForward;
	bool m_bGoingBack;
	bool m_bGoingLeft;
	bool m_bGoingRight;
	bool m_bGoingUp;
	bool m_bGoingDown;
	bool m_bFastMove;
	Ogre::Vector3 m_currPos;

	std::shared_ptr<SoundListener> m_listener;	//required so that FMOD 3d sound can work
};

#endif