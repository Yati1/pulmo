#include "Controller.h"

Controller::Controller(NxOgre::CharacterController* body,Ogre::Entity* entity):
	m_bGoingBack(false),
	m_bGoingDown(false),
	m_bGoingForward(false),
	m_bGoingLeft(false),
	m_bGoingRight(false),
	m_bGoingUp(false),
	m_bFastMove(false),
	m_fSpeedConst(1000.0f),
	m_fTopSpeed(500),
	m_Velocity(Ogre::Vector3::ZERO)
{
	m_pCollisionBody=body;
	m_pEntity=entity;
	m_pNode=m_pEntity->getParentSceneNode();
	m_pCollisionBody->setPosition(PhysicsSystem::GetSingleton()->convertOgreVec3(m_pNode->getPosition()));
	//m_pActor = ActorManager::GetSingleton()->createActor(entity,body);
}

Controller::~Controller(void)
{
}

void Controller::translateZ(float trans)
{
	Ogre::Vector3 transV = m_pNode->getOrientation() * Ogre::Vector3(0, 0, trans);
	m_pCollisionBody->move(PhysicsSystem::GetSingleton()->convertOgreVec3(transV));
}
void Controller::translateX(float trans)
{
	Ogre::Vector3 transV = m_pNode->getOrientation() * Ogre::Vector3(trans, 0, 0);
	m_pCollisionBody->move(PhysicsSystem::GetSingleton()->convertOgreVec3(transV));
	//m_pCollisionBody->move(NxOgre::Vec3(100,0,0));
}
void Controller::translateY(float trans)
{
	Ogre::Vector3 transV = m_pNode->getOrientation() * Ogre::Vector3(0, trans,0);
	m_pCollisionBody->move(PhysicsSystem::GetSingleton()->convertOgreVec3(transV));
}

void Controller::injectKeyUp(const OIS::KeyEvent& evt)
{
	if (evt.key == OIS::KC_W || evt.key == OIS::KC_UP) m_bGoingForward = false;
	else if (evt.key == OIS::KC_S || evt.key == OIS::KC_DOWN) m_bGoingBack = false;
	else if (evt.key == OIS::KC_A || evt.key == OIS::KC_LEFT) m_bGoingLeft = false;
	else if (evt.key == OIS::KC_D || evt.key == OIS::KC_RIGHT) m_bGoingRight = false;
	else if (evt.key == OIS::KC_PGUP) m_bGoingUp = false;
	else if (evt.key == OIS::KC_PGDOWN) m_bGoingDown = false;
	else if (evt.key == OIS::KC_LSHIFT) m_bFastMove = false;
}

void Controller::injectKeyDown(const OIS::KeyEvent& evt)
{
	if (evt.key == OIS::KC_W || evt.key == OIS::KC_UP) m_bGoingForward = true;
	else if (evt.key == OIS::KC_S || evt.key == OIS::KC_DOWN) m_bGoingBack = true;
	else if (evt.key == OIS::KC_A || evt.key == OIS::KC_LEFT) m_bGoingLeft = true;
	else if (evt.key == OIS::KC_D || evt.key == OIS::KC_RIGHT) m_bGoingRight = true;
	else if (evt.key == OIS::KC_PGUP) m_bGoingUp = true;
	else if (evt.key == OIS::KC_PGDOWN) m_bGoingDown = true;
	else if (evt.key == OIS::KC_LSHIFT) m_bFastMove = true;
}

void Controller::injectMouseMove(const OIS::MouseEvent& evt)
{
	//m_pCollisionBody->setAngularVelocity(0.0f,-evt.state.X.rel * 0.05f,0.0f);
	//m_pNode->yaw(Ogre::Degree(-evt.state.X.rel * 0.15f));
	//m_pCollisionBody->setGlobalOrientationQuat(PhysicsSystem::GetSingleton()->convertOgreQuat(m_pNode->getOrientation()));
	//m_pEntity->getParentSceneNode()->yaw(Ogre::Degree(-evt.state.X.rel * 0.15f));
	m_pNode->pitch(Ogre::Degree(-evt.state.Y.rel * 0.15f));
	//m_pCamera->pitch(Ogre::Degree(-evt.state.Y.rel * 0.15f));
}

void Controller::injectMouseDown(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	//do nothing
}

void Controller::injectMouseUp(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	//do nothing
}

void Controller::update(float fElapsedTime)
{
	/*if (m_bGoingForward) translateZ(m_fSpeedConst*fElapsedTime);
	if (m_bGoingBack) translateZ(-m_fSpeedConst*fElapsedTime);
	if (m_bGoingRight) translateX(-m_fSpeedConst*fElapsedTime);
	if (m_bGoingLeft) translateX(m_fSpeedConst*fElapsedTime);
	if (m_bGoingUp) translateY(m_fSpeedConst*fElapsedTime);
	if (m_bGoingDown) translateY(-m_fSpeedConst*fElapsedTime);
	//NxOgre::Vec3 v = m_pCollisionBody->getPosition();
	m_pNode->setPosition(PhysicsSystem::GetSingleton()->convertNxVec3(m_pCollisionBody->getPosition()));*/

	// build our acceleration vector based on keyboard input composite
	Ogre::Vector3 accel = Ogre::Vector3::ZERO;
	if (m_bGoingForward) accel += m_pNode->getOrientation() * Ogre::Vector3(0, 0, 1);
	if (m_bGoingBack) accel -= m_pNode->getOrientation() * Ogre::Vector3(0, 0, 1);
	if (m_bGoingRight) accel += m_pNode->getOrientation() * Ogre::Vector3(1, 0, 0);
	if (m_bGoingLeft) accel -= m_pNode->getOrientation() * Ogre::Vector3(1, 0, 0);
	if (m_bGoingUp) accel += m_pNode->getOrientation() * Ogre::Vector3(0, 1, 0);
	if (m_bGoingDown) accel -= m_pNode->getOrientation() * Ogre::Vector3(0, 1, 0);

	// if accelerating, try to reach top speed in a certain time
	Ogre::Real topSpeed = m_bFastMove ? m_fTopSpeed * 20 : m_fTopSpeed;
	if (accel.squaredLength() != 0)
	{
		accel.normalise();
		m_Velocity += accel * topSpeed * fElapsedTime * 10;
	}
	// if not accelerating, try to stop in a certain time
	else m_Velocity -= m_Velocity * fElapsedTime * 10;

	Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

	// keep camera velocity below top speed and above epsilon
	if (m_Velocity.squaredLength() > topSpeed * topSpeed)
	{
		m_Velocity.normalise();
		m_Velocity *= topSpeed;
	}
	else if (m_Velocity.squaredLength() < tooSmall * tooSmall)
		m_Velocity = Ogre::Vector3::ZERO;

	if (m_Velocity != Ogre::Vector3::ZERO)
	{
		Ogre::Vector3 v = (m_Velocity * fElapsedTime);	//for kematic actor just add this to current pos
		m_pCollisionBody->move(PhysicsSystem::GetSingleton()->convertOgreVec3(v));
	}

	m_pNode->setPosition(PhysicsSystem::GetSingleton()->convertNxVec3(m_pCollisionBody->getPosition()));
}