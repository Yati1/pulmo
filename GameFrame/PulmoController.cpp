#include "PulmoController.h"

PulmoController::PulmoController(PulmoActor* player, Ogre::Camera* pCam): PlayerController(player,pCam)
{
	m_pPulmo = (PulmoActor*)m_pActor;
}
PulmoController::~PulmoController()
{
}

void PulmoController::injectKeyUp(const OIS::KeyEvent& evt)
{
	PlayerController::injectKeyUp(evt);	//call super class's function

	if (evt.key == OIS::KC_1) m_pPulmo->setPrimaryColor(PulmoActor::red);
	else
	if (evt.key == OIS::KC_2) m_pPulmo->setPrimaryColor(PulmoActor::yellow);
	else
	if (evt.key == OIS::KC_3) m_pPulmo->setPrimaryColor(PulmoActor::blue);
}