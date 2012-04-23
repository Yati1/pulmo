#include "CameraController.h"

CameraController::CameraController(Ogre::Camera* cam):
	m_pCamera(0)
	, m_fTopSpeed(550)
	, m_Velocity(Ogre::Vector3::ZERO)
	, m_bGoingForward(false)
	, m_bGoingBack(false)
	, m_bGoingLeft(false)
	, m_bGoingRight(false)
	, m_bGoingUp(false)
	, m_bGoingDown(false)
	, m_bFastMove(false)
	,m_bListener(false)
	,m_pCollisionBody(0)

{
	m_pCamera=cam;
	FMOD_VECTOR forward,up,pos,vel;
	Ogre::Vector3 f=m_pCamera->getOrientation()*Ogre::Vector3::UNIT_X;
	Ogre::Vector3 u=m_pCamera->getUp();
	Ogre::Vector3 p=m_pCamera->getPosition();

	forward.x=f.x;
	forward.y=f.y;
	forward.z=f.z;

	up.x=u.x;
	up.y=u.y;
	up.z=u.z;

	pos.x=p.x;
	pos.y=p.y;
	pos.z=p.z;

	vel.x =0;
	vel.y =0;
	vel.z =0;

	m_currPos = p;

	m_listener=std::shared_ptr<SoundListener>(new SoundListener(pos,forward,up,vel));
}

CameraController::~CameraController(void)
{
}

Critter::Body* CameraController::getCollisionBody()const
{
	return m_pCollisionBody;
}

void CameraController::setCollisionBody(Critter::Body* body)
{
	m_pCollisionBody=body;
	//m_pCamera->getParentSceneNode()->attachObject(m_pCollisionBody);
	if(m_pCamera->isAttached())
		m_pCamera->detachFromParent();
	m_pCollisionBody->getSceneNode()->attachObject(m_pCamera);
	m_pCamera->getParentSceneNode()->setPosition(m_pCollisionBody->getGlobalPosition().x,m_pCollisionBody->getGlobalPosition().y,m_pCollisionBody->getGlobalPosition().z);
}

void CameraController::update(float fElapsedTime)
{
	if(!m_pCollisionBody)
	{
		// build our acceleration vector based on keyboard input composite
		Ogre::Vector3 accel = Ogre::Vector3::ZERO;
		if (m_bGoingForward) accel += m_pCamera->getDirection();
		if (m_bGoingBack) accel -= m_pCamera->getDirection();
		if (m_bGoingRight) accel += m_pCamera->getRight();
		if (m_bGoingLeft) accel -= m_pCamera->getRight();
		if (m_bGoingUp) accel += m_pCamera->getUp();
		if (m_bGoingDown) accel -= m_pCamera->getUp();

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

		if (m_Velocity != Ogre::Vector3::ZERO) m_pCamera->move(m_Velocity * fElapsedTime);	//for kematic actor just add this to current pos
	}
	else
	{
		/*
		if (m_bGoingForward) translateZ(-250.0f*fElapsedTime);
		if (m_bGoingBack) translateZ(250.0f*fElapsedTime);
		if (m_bGoingRight) translateX(250.0f*fElapsedTime);
		if (m_bGoingLeft) translateX(-250.0f*fElapsedTime);
		if (m_bGoingUp) translateY(250.0f*fElapsedTime);
		if (m_bGoingDown) translateY(-250.0f*fElapsedTime);
		*/
		// build our acceleration vector based on keyboard input composite
		Ogre::Vector3 accel = Ogre::Vector3::ZERO;
		if (m_bGoingForward) accel += m_pCamera->getDirection();
		if (m_bGoingBack) accel -= m_pCamera->getDirection();
		if (m_bGoingRight) accel += m_pCamera->getRight();
		if (m_bGoingLeft) accel -= m_pCamera->getRight();
		if (m_bGoingUp) accel += m_pCamera->getUp();
		if (m_bGoingDown) accel -= m_pCamera->getUp();

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
			m_pCollisionBody->addForce(PhysicsSystem::GetSingleton()->convertOgreVec3(m_Velocity * fElapsedTime),NxOgre::Enums::ForceMode_Acceleration);
			//m_pCollisionBody->setLinearVelocity(PhysicsSystem::GetSingleton()->convertOgreVec3(m_Velocity * fElapsedTime));

		m_pCamera->setPosition(m_pCamera->getParentSceneNode()->convertWorldToLocalPosition(Ogre::Vector3( m_pCollisionBody->getGlobalPosition().x,m_pCollisionBody->getGlobalPosition().y,m_pCollisionBody->getGlobalPosition().z)));
	}
	if(m_bListener)
	{
		FMOD_VECTOR forward,up,pos;
		Ogre::Vector3 f=m_pCamera->getDirection();
		Ogre::Vector3 u=m_pCamera->getUp();
		NxOgre::Vec3 p=m_pCollisionBody->getGlobalPosition();

		forward.x=f.x;
		forward.y=f.y;
		forward.z=f.z;

		up.x=u.x;
		up.y=u.y;
		up.z=u.z;

		pos.x=p.x;
		pos.y=p.y;
		pos.z=p.z;

		m_listener->setForward(forward);
		m_listener->setPos(pos);
		m_listener->setUp(up);
	}
}

void CameraController::translateZ(float trans)
{
	Ogre::Vector3 transV = m_pCamera->getOrientation() * Ogre::Vector3(0, 0, trans);
	if(m_pCollisionBody)
	{
		m_pCollisionBody->addForce(transV,NxOgre::Enums::ForceMode_SmoothImpulse);
	}
	else
	{
	}
}
void CameraController::translateX(float trans)
{
	Ogre::Vector3 transV = m_pCamera->getOrientation() * Ogre::Vector3(trans, 0, 0);
	if(m_pCollisionBody)
	{
		//m_currPos+=transV;
		//m_pCollisionBody->moveGlobalPosition(m_currPos);
		m_pCollisionBody->addForce(transV,NxOgre::Enums::ForceMode_SmoothImpulse);
	}
	else
	{
	}
}
void CameraController::translateY(float trans)
{
	Ogre::Vector3 transV = m_pCamera->getOrientation() * Ogre::Vector3(0, trans,0);
	if(m_pCollisionBody)
	{
		//m_currPos+=transV;
		//m_pCollisionBody->moveGlobalPosition(m_currPos);
		m_pCollisionBody->addForce(transV,NxOgre::Enums::ForceMode_SmoothImpulse);
	}
	else
	{
	}
}

void CameraController::injectKeyUp(const OIS::KeyEvent& evt)
{
	if (evt.key == OIS::KC_W || evt.key == OIS::KC_UP) m_bGoingForward = false;
	else if (evt.key == OIS::KC_S || evt.key == OIS::KC_DOWN) m_bGoingBack = false;
	else if (evt.key == OIS::KC_A || evt.key == OIS::KC_LEFT) m_bGoingLeft = false;
	else if (evt.key == OIS::KC_D || evt.key == OIS::KC_RIGHT) m_bGoingRight = false;
	else if (evt.key == OIS::KC_PGUP) m_bGoingUp = false;
	else if (evt.key == OIS::KC_PGDOWN) m_bGoingDown = false;
	else if (evt.key == OIS::KC_LSHIFT) m_bFastMove = false;
}

void CameraController::injectKeyDown(const OIS::KeyEvent& evt)
{
	if (evt.key == OIS::KC_W || evt.key == OIS::KC_UP) m_bGoingForward = true;
	else if (evt.key == OIS::KC_S || evt.key == OIS::KC_DOWN) m_bGoingBack = true;
	else if (evt.key == OIS::KC_A || evt.key == OIS::KC_LEFT) m_bGoingLeft = true;
	else if (evt.key == OIS::KC_D || evt.key == OIS::KC_RIGHT) m_bGoingRight = true;
	else if (evt.key == OIS::KC_PGUP) m_bGoingUp = true;
	else if (evt.key == OIS::KC_PGDOWN) m_bGoingDown = true;
	else if (evt.key == OIS::KC_LSHIFT) m_bFastMove = true;
}

void CameraController::injectMouseMove(const OIS::MouseEvent& evt)
{
	m_pCamera->yaw(Ogre::Degree(-evt.state.X.rel * 0.15f));
	m_pCamera->pitch(Ogre::Degree(-evt.state.Y.rel * 0.15f));
}

void CameraController::injectMouseDown(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	//do nothing
}

void CameraController::injectMouseUp(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	//do nothing
}