#ifndef PULMO_CONTROLLER_H
#define PULMO_CONTROLLER_H

#include "PlayerController.h"
#include "PulmoActor.h"

class PulmoController : public PlayerController
{
public:
	PulmoController(PulmoActor* player, Ogre::Camera* pCam);
	~PulmoController();

	void injectKeyUp(const OIS::KeyEvent& evt);
private:

	PulmoActor* m_pPulmo;
};

#endif