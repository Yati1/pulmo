#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "InputSystem.h"

#include "SoundSystem.h"

#include "PhysicsSystem.h"

#include <ogre.h>

#include <memory>

class CameraController
{
public:
	CameraController(Ogre::Camera* cam);
	virtual ~CameraController(void);

	Ogre::Vector3 getPosition()const{return m_pCamera->getPosition();}
	Ogre::Quaternion getOrientation()const{return m_pCamera->getOrientation();}
	Ogre::Camera* getCamera()const{return m_pCamera;}

	bool isListening()const{return m_bListener;}
	void setListen(bool listen){m_bListener=listen;}

	SoundListener* getSoundListener()const{return m_listener.get();}

	virtual void update(float fElapsedTime);

	void injectKeyUp(const OIS::KeyEvent& evt);
	void injectKeyDown(const OIS::KeyEvent& evt);
	void injectMouseMove(const OIS::MouseEvent& evt);
	void injectMouseDown(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	void injectMouseUp(const OIS::MouseEvent& evt, OIS::MouseButtonID id);

	void setCollisionBody(Critter::Body* body);
	Critter::Body* getCollisionBody()const;

protected:

	void translateZ(float trans);
	void translateX(float trans);
	void translateY(float trans);

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

	bool m_bListener;
	std::shared_ptr<SoundListener> m_listener;

	Critter::Body* m_pCollisionBody;
	//Critter::KinematicBody* m_pCollisionBody;

	Ogre::Vector3 m_currPos;
};

#endif