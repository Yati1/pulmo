#include "PlayerController.h"

PlayerController::PlayerController(Actor* player, Ogre::Camera* pCamera)
{
	m_pActor = player;
	m_pCamera = pCamera;
	m_pNode = m_pActor->getBody()->getSceneNode();
	//m_pActor->getBody()->setSceneNode(m_pNode);
	//m_CurrPos = PhysicsSystem::GetSingleton()->convertNxVec3(m_pNode->getPosition());
	m_KeyDirection = Ogre::Vector3::ZERO;

	m_pRaySceneQuery = pCamera->getSceneManager()->createRayQuery(Ogre::Ray());

	nSpeed = 10;
//	if(m_pCamera->isAttached())
//		m_pCamera->detachFromParent();
	//(Ogre::Vector3(0,75,0)
	//camera node will be the position of the camera

//	m_pCameraNode->setPosition(Ogre::Vector3(m_pNode->getPosition().x, m_pNode->getPosition().y, m_pNode->getPosition().z-5));
}

PlayerController::~PlayerController(void)
{
	m_pCamera->getSceneManager()->destroyQuery(m_pRaySceneQuery);
}

void PlayerController::injectKeyUp(const OIS::KeyEvent& evt)
{
	// using another condition to make sure actor can still move when player press 2 buttons but lift only 1 ubtton
	if (evt.key == OIS::KC_W && m_KeyDirection.z == -1) m_KeyDirection.z = 0;
	else if (evt.key == OIS::KC_A && m_KeyDirection.x == -1) m_KeyDirection.x = 0;
	else if (evt.key == OIS::KC_S && m_KeyDirection.z == 1) m_KeyDirection.z = 0;
	else if (evt.key == OIS::KC_D && m_KeyDirection.x == 1) m_KeyDirection.x = 0;
}

void PlayerController::injectKeyDown(const OIS::KeyEvent& evt)
{
	if (evt.key == OIS::KC_W) m_KeyDirection.z = -1;		//go forward
	else if (evt.key == OIS::KC_A) m_KeyDirection.x = -1;	//go left
	else if (evt.key == OIS::KC_S) m_KeyDirection.z = 1;	//go backward
	else if (evt.key == OIS::KC_D) m_KeyDirection.x = 1;	//go right
}

void PlayerController::injectMouseMove(const OIS::MouseEvent& evt)
{
	updateCameraGoal(-0.05f * evt.state.X.rel, -0.05f * evt.state.Y.rel, -0.0005f * evt.state.Z.rel);
}

void PlayerController::update(float fElapsedTime)
{
	updateActor(fElapsedTime);
	updateCamera(fElapsedTime);

	//update the sound listener
	SoundSystem::GetSingleton()->updateSoundListener(	-m_pCameraNode->getLocalAxes().GetColumn(2),
																	m_pCameraNode->getLocalAxes().GetColumn(1),
																	m_pCameraNode->getPosition(),
																	PhysicsSystem::GetSingleton()->convertNxVec3(m_pActor->getBody()->getLinearVelocity()));

//	m_pRay->setOrigin(m_pCameraNode->getPosition());
//	m_pRay->setDirection(m_pNode->getPosition());
}

void PlayerController::updateActor(float fElapsedTime)
{
	m_GoalPos =  Ogre::Vector3::ZERO;

	if (m_KeyDirection != Ogre::Vector3::ZERO)
	{
		// calculate actually goal direction in world based on player's key directions
		m_GoalPos += m_KeyDirection.z * m_pCameraNode->getOrientation().zAxis();
		m_GoalPos += m_KeyDirection.x * m_pCameraNode->getOrientation().xAxis();
		m_GoalPos.y = 0;
	}

	if (m_GoalPos != Ogre::Vector3::ZERO)
	{
		m_GoalPos.normalise();

		Ogre::Quaternion toGoal = m_pNode->getOrientation().zAxis().getRotationTo(m_GoalPos);

		// calculate how much the character has to turn to face goal direction
		Ogre::Real yawToGoal = toGoal.getYaw().valueDegrees();
		// this is how much the character CAN turn this frame
		Ogre::Real yawAtSpeed = yawToGoal / Ogre::Math::Abs(yawToGoal) * fElapsedTime * TURN_SPEED; //500.0f = Turn speed per second

		// turn as much as we can, but not more than we need to
		if (yawToGoal < 0) yawToGoal = std::min<Ogre::Real>(0, std::max<Ogre::Real>(yawToGoal, yawAtSpeed)); //
		else if (yawToGoal > 0) yawToGoal = std::max<Ogre::Real>(0, std::min<Ogre::Real>(yawToGoal, yawAtSpeed)); //

		//rotate the body around y-axis(left-right)
		m_pNode->yaw(Ogre::Degree(yawToGoal));

		// move in current body direction (not the goal direction)
		//m_pNode->translate(0, 0, fElapsedTime * m_GoalPos.z * 500, Ogre::Node::TS_WORLD);

		//Make the body yaw as well, otherwise NxOgre'll will reset to the initial orientation(it ignores the scene node orientation)
		m_pActor->getBody()->setGlobalOrientationQuat(m_pActor->getBody()->getSceneNode()->_getDerivedOrientation());
	}

	if (m_KeyDirection != Ogre::Vector3::ZERO && m_GoalPos != Ogre::Vector3::ZERO)
	{
		m_pActor->getBody()->addForce(PhysicsSystem::GetSingleton()->convertOgreVec3(m_GoalPos * 1000 * fElapsedTime),NxOgre::Enums::ForceMode_SmoothImpulse);
	}

//	m_pNode->setPosition(m_pNode->getParentSceneNode()->convertWorldToLocalPosition(Ogre::Vector3( m_pActor->getBody()->getGlobalPosition().x,m_pActor->getBody()->getGlobalPosition().y,m_pActor->getBody()->getGlobalPosition().z)));
}

//_getDerivedPosition() = get position from parent node

void PlayerController::updateCameraGoal(Ogre::Real deltaYaw, Ogre::Real deltaPitch, Ogre::Real deltaZoom)
{
	m_pCameraPivot->yaw(Ogre::Degree(deltaYaw), Ogre::Node::TS_WORLD);

		// bound the pitch
		if (!(m_PivotPitch + deltaPitch > 25 && deltaPitch > 0) &&
			!(m_PivotPitch + deltaPitch < -60 && deltaPitch < 0))
		{
			m_pCameraPivot->pitch(Ogre::Degree(deltaPitch), Ogre::Node::TS_LOCAL);
			m_PivotPitch += deltaPitch;
		}

		Ogre::Real dist = m_pCameraGoal->_getDerivedPosition().distance(m_pCameraPivot->_getDerivedPosition());

		if(!RayCollide())
		{
			Ogre::Real distChange = deltaZoom * dist;
			// bound the zoom
			if (!(dist + distChange < 8 && distChange < 0) &&
				!(dist + distChange > 25 && distChange > 0))

				m_pCameraGoal->translate(0, 0, distChange, Ogre::Node::TS_LOCAL);
		}
		else
		{
			Ogre::Real distChange = -0.7f * dist;
			// bound the zoom, we dont want the camera to move away from the mesh or deep into it

			if (!(dist + distChange < 3 && distChange < 0) &&
				!(dist + distChange > 25 && distChange > 0))

				m_pCameraGoal->translate(0, 0, distChange, Ogre::Node::TS_LOCAL);
		}
}

void PlayerController::updateCamera(float fElapsedTime)
{
	// place the camera pivot roughly at the character's shoulder
	m_pCameraPivot->setPosition(m_pNode->getPosition() + Ogre::Vector3::UNIT_Y * 2);

	// move the camera smoothly to the goal
	Ogre::Vector3 goalOffset = m_pCameraGoal->_getDerivedPosition() - m_pCameraNode->getPosition();
	m_pCameraNode->translate(goalOffset * fElapsedTime * 9.0f);

	m_pCameraNode->lookAt(m_pCameraPivot->_getDerivedPosition(), Ogre::Node::TS_WORLD);
}

void PlayerController::setupCamera()
{
	m_pCameraNode = m_pCamera->getSceneManager()->getRootSceneNode()->createChildSceneNode(m_pNode->getPosition());
	//create a center point for the camera to rotate around and to look at
	m_pCameraPivot = m_pCamera->getSceneManager()->getRootSceneNode()->createChildSceneNode(m_pNode->getPosition());
	//place the initial camera in front of the body
	m_pCameraGoal = m_pCameraPivot->createChildSceneNode(Ogre::Vector3(0, 0, 20));

	m_pCameraNode->setPosition(m_pCameraNode->convertLocalToWorldPosition( m_pCameraPivot->getPosition()) + m_pCameraNode->convertLocalToWorldPosition( m_pCameraGoal->getPosition()));

	//make all the nodes yaw axis fixed so the camera only yaws around the pre-determined y axis which is located in the middle of the body (prevents flipping the camera upside down)
	m_pCameraPivot->setFixedYawAxis(true);
	m_pCameraGoal->setFixedYawAxis(true);
	m_pCameraNode->setFixedYawAxis(true);

	m_pCameraNode->attachObject(m_pCamera);
	m_PivotPitch = 0;

		//create sound Listener based on camera position
	m_pListener = SoundSystem::GetSingleton()->createSoundListener(	m_pCamera->getOrientation()*Ogre::Vector3::UNIT_X,
																	m_pCamera->getUp(),
																	m_pCamera->getPosition(),
																	Ogre::Vector3::ZERO);
}

bool PlayerController::RayCollide()
{
	//put the ray a bit behind the camera, so it can respond faster to a collision
	/*+ Ogre::Vector3::UNIT_Z * -25.0f*/

	/* ---commented this out for now, until ray collision is fixed
	Ogre::Ray cameraRay(m_pCameraNode->getPosition() , m_pCameraNode->convertLocalToWorldPosition(m_pActor->getBody()->getSceneNode()->getPosition()));
    m_pRaySceneQuery->setRay(cameraRay);

	Ogre::RaySceneQueryResult &result = m_pRaySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator itr = result.begin();

	if (itr != result.end() && itr->movable)
    {
		//only returns true if the raycast hit other objects, not Pulmo!
		if (itr->movable->getParentSceneNode() != m_pNode)
		{
			return true;
		}
	}*/

	return false;
}