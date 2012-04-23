#include "ThirdPersonCameraController.h"


ThirdPersonCameraController::ThirdPersonCameraController(Ogre::Camera* cam,Ogre::SceneNode* followThis):CameraController(cam),
	m_pFollowingNode(followThis),
	m_pCamNode(0)
{
	
	Ogre::Vector3 camPos=followThis->convertWorldToLocalPosition(cam->getPosition());
	m_pCamNode=followThis->createChildSceneNode(Ogre::Vector3(0.0f,camPos.y,camPos.z));
	cam->setPosition(m_pCamNode->getPosition());
	//cam->lookAt(m_pFollowingNode->getPosition());
	if(cam->isAttached())
		cam->detachFromParent();
	m_pCamNode->attachObject(cam);
	//m_pCamNode->setInheritOrientation(false);
	cam->setAutoTracking(true,m_pFollowingNode,Ogre::Vector3(0.0f,2.0f,5.0f));

}


ThirdPersonCameraController::~ThirdPersonCameraController(void)
{
}

void ThirdPersonCameraController::injectMouseMove(const OIS::MouseEvent& evt)
{
	if(evt.state.Z.rel!=0.0f)
	{
		if(evt.state.Z.rel<0)
		{
			m_bGoingForward=false;
			m_bGoingBack=true;
		}
		else
		{
			m_bGoingBack=false;
			m_bGoingForward=true;
		}
	}
}

void ThirdPersonCameraController::update(float elapsedTime)
{
	CameraController::update(elapsedTime);

	m_bGoingBack=false;
	m_bGoingForward=false;
}