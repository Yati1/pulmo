#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <Ogre.h>

#include "ThirdPersonCameraController.h"
#include "Actor.h"
#include "Controller.h"
#include "SoundSystem.h"

#define CAM_HEIGHT 2.0f;	   // camera height from the body
#define TURN_SPEED 500.0f; // this is 500 unit/second

class PlayerController
{
public:
	PlayerController(Actor* player, Ogre::Camera* pCam);
	virtual ~PlayerController(void);

	virtual void update(float fElapsedTime);
	virtual void setupCamera();

	virtual void injectKeyUp(const OIS::KeyEvent& evt);
	virtual void injectMouseMove(const OIS::MouseEvent& evt);
	virtual void injectKeyDown(const OIS::KeyEvent& evt);

	bool RayCollide();
protected:
	// going to separate the update process then put them together in update function
	void updateActor(float fElapsedTime);
	void updateCamera(float fElapsedTime);
	void updateCameraGoal(Ogre::Real deltaYaw, Ogre::Real deltaPitch, Ogre::Real deltaZoom);

	Actor* m_pActor;

	Ogre::Camera* m_pCamera;
	//Camera Scene Nodes
	Ogre::SceneNode* m_pCameraNode;
	Ogre::SceneNode* m_pCameraPivot;
	Ogre::SceneNode* m_pCameraGoal;

	Ogre::RaySceneQuery* m_pRaySceneQuery;

	//Scene Node for Actor
	Ogre::SceneNode* m_pNode;

	Ogre::Vector3 m_KeyDirection; //intended direction
	Ogre::Vector3 m_GoalPos; //used to turn/rotate actor into camera's orientation/facing direction
	Ogre::Real m_PivotPitch;

	SoundListener* m_pListener;	//required so that FMOD 3d sound can work

	int nSpeed;
};

#endif