#ifndef THIRD_PERSON_CAMERA_H
#define THIRD_PERSON_CAMERA_H

#include "CameraController.h"

class ThirdPersonCameraController:public CameraController
{
public:
	ThirdPersonCameraController(Ogre::Camera* cam,Ogre::SceneNode* followThis);
	~ThirdPersonCameraController(void);
	
	Ogre::SceneNode* getFollowing(){return m_pFollowingNode;}

	void injectMouseMove(const OIS::MouseEvent& evt);

	void update(float elapsedTime);

private:
	Ogre::SceneNode* m_pFollowingNode;
	Ogre::SceneNode* m_pCamNode;

};

#endif

