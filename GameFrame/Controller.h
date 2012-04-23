#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "InputSystem.h"

#include "SoundSystem.h"

#include "PhysicsSystem.h"

//#include "AstroidsActorManager.h"
#include "ActorManager.h"

#include <ogre.h>

#include <memory>
class Controller
{
public:
	Controller(NxOgre::CharacterController* body,Ogre::Entity* entity);
	virtual ~Controller(void);

	virtual void update(float fElapsedTime);

	virtual void injectKeyUp(const OIS::KeyEvent& evt);
	virtual void injectKeyDown(const OIS::KeyEvent& evt);
	virtual void injectMouseMove(const OIS::MouseEvent& evt);
	virtual void injectMouseDown(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	virtual void injectMouseUp(const OIS::MouseEvent& evt, OIS::MouseButtonID id);

	void setCollisionBody(Critter::Body* body);
	NxOgre::CharacterController* getCollisionBody()const{return m_pCollisionBody;}

	Actor* getActor()const{return m_pActor;}
protected:
	float m_fSpeedConst;
	void translateZ(float trans);
	void translateX(float trans);
	void translateY(float trans);

	bool m_bGoingForward;
	bool m_bGoingBack;
	bool m_bGoingLeft;
	bool m_bGoingRight;
	bool m_bGoingUp;
	bool m_bGoingDown;
	bool m_bFastMove;

	Actor* m_pActor;

	NxOgre::CharacterController* m_pCollisionBody;
	Ogre::Entity* m_pEntity;
	Ogre::SceneNode* m_pNode;

	float m_fTopSpeed;
	Ogre::Vector3 m_Velocity;
};
#endif