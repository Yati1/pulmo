#include "FPSCamActor.h"

FPSCamActor::FPSCamActor(Ogre::Entity* entity,Critter::Body* body,ActorManager* manager,Ogre::Camera* cam):
	Actor(entity,body,manager),
	m_fTopSpeed(550)
	, m_Velocity(Ogre::Vector3::ZERO)
	, m_bGoingForward(false)
	, m_bGoingBack(false)
	, m_bGoingLeft(false)
	, m_bGoingRight(false)
	, m_bGoingUp(false)
	, m_bGoingDown(false)
	, m_bFastMove(false)
	,m_pCamera(cam)
{
	if(m_pCamera->isAttached())
		m_pCamera->detachFromParent();
	m_pBody->getSceneNode()->attachObject(m_pCamera);
	m_pCamera->getParentSceneNode()->setPosition(m_pBody->getGlobalPosition().x,m_pBody->getGlobalPosition().y,m_pBody->getGlobalPosition().z);

	m_currPos = m_pCamera->getPosition();

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

void FPSCamActor::injectKeyUp(const OIS::KeyEvent& evt)
{
	if (evt.key == OIS::KC_W || evt.key == OIS::KC_UP) m_bGoingForward = false;
	else if (evt.key == OIS::KC_S || evt.key == OIS::KC_DOWN) m_bGoingBack = false;
	else if (evt.key == OIS::KC_A || evt.key == OIS::KC_LEFT) m_bGoingLeft = false;
	else if (evt.key == OIS::KC_D || evt.key == OIS::KC_RIGHT) m_bGoingRight = false;
	else if (evt.key == OIS::KC_PGUP) m_bGoingUp = false;
	else if (evt.key == OIS::KC_PGDOWN) m_bGoingDown = false;
	else if (evt.key == OIS::KC_LSHIFT) m_bFastMove = false;
}
void FPSCamActor::injectKeyDown(const OIS::KeyEvent& evt)
{
	if (evt.key == OIS::KC_W || evt.key == OIS::KC_UP) m_bGoingForward = true;
	else if (evt.key == OIS::KC_S || evt.key == OIS::KC_DOWN) m_bGoingBack = true;
	else if (evt.key == OIS::KC_A || evt.key == OIS::KC_LEFT) m_bGoingLeft = true;
	else if (evt.key == OIS::KC_D || evt.key == OIS::KC_RIGHT) m_bGoingRight = true;
	else if (evt.key == OIS::KC_PGUP) m_bGoingUp = true;
	else if (evt.key == OIS::KC_PGDOWN) m_bGoingDown = true;
	else if (evt.key == OIS::KC_LSHIFT) m_bFastMove = true;
}
void FPSCamActor::injectMouseMove(const OIS::MouseEvent& evt)
{
	m_pCamera->yaw(Ogre::Degree(-evt.state.X.rel * 0.15f));
	m_pCamera->pitch(Ogre::Degree(-evt.state.Y.rel * 0.15f));
}
void FPSCamActor::injectMouseDown(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	//do nothing
}
void FPSCamActor::injectMouseUp(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	//do nothing
}

void FPSCamActor::update(float fElapsedTime)
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

	if (m_Velocity != Ogre::Vector3::ZERO)
		m_pBody->addForce(PhysicsSystem::GetSingleton()->convertOgreVec3(m_Velocity * fElapsedTime),NxOgre::Enums::ForceMode_Acceleration);
		//m_pCollisionBody->setLinearVelocity(PhysicsSystem::GetSingleton()->convertOgreVec3(m_Velocity * fElapsedTime));

	m_pCamera->setPosition(m_pCamera->getParentSceneNode()->convertWorldToLocalPosition(Ogre::Vector3( m_pBody->getGlobalPosition().x,m_pBody->getGlobalPosition().y,m_pBody->getGlobalPosition().z)));

	//update the sound listener
	FMOD_VECTOR forward,up,pos,vel;
	Ogre::Vector3 f=m_pCamera->getDirection();
	Ogre::Vector3 u=m_pCamera->getUp();
	NxOgre::Vec3 p=m_pBody->getGlobalPosition();

	forward.x=f.x;
	forward.y=f.y;
	forward.z=f.z;

	up.x=u.x;
	up.y=u.y;
	up.z=u.z;

	pos.x=p.x;
	pos.y=p.y;
	pos.z=p.z;

	vel.x = m_pBody->getLinearVelocity().x;
	vel.y = m_pBody->getLinearVelocity().y;
	vel.z = m_pBody->getLinearVelocity().z;
	m_listener->setForward(forward);
	m_listener->setPos(pos);
	m_listener->setUp(up);
	m_listener->setVel(vel);
}