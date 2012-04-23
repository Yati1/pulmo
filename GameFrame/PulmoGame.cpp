#include "PulmoGame.h"

PulmoGame::PulmoGame(Ogre::RenderWindow*  pRenderWindow)
{
	assert(pRenderWindow!=0);
	m_pSceneManager=Ogre::Root::getSingleton().getSceneManager("DefaultSceneManager");
	m_pRenderWindow=pRenderWindow;
}
PulmoGame::~PulmoGame(void)
{
	m_pSceneManager=0;
	m_pViewPort=0;
	m_pRenderWindow=0;
}

void PulmoGame::init()
{
	InputSystem::GetSingleton()->addListener(*this);

	m_pPlayerCam=m_pSceneManager->createCamera("PlayerCam");
	m_pPlayerCam->setPosition(Ogre::Vector3(0,0,0));
	// Look back along -Z
	m_pPlayerCam->lookAt(Ogre::Vector3(0,0,-300));
	m_pPlayerCam->setNearClipDistance(5);

	m_pViewPort=m_pRenderWindow->addViewport(m_pPlayerCam);
	m_pViewPort->setBackgroundColour(Ogre::ColourValue(1.0,1.0,1.0));

	m_currentState = GAME_STATE_RUNNING;
}
void PulmoGame::update(float elapsedTime)
{
}

void PulmoGame::clean()
{
	InputSystem::GetSingleton()->removeListener(*this);
	if(m_pPlayerCam)
	{
		m_pRenderWindow->removeViewport(0);

		m_pSceneManager->destroyAllCameras();
		m_pPlayerCam=0;
	}

	m_pSceneManager->clearScene();

	SoundSystem::GetSingleton()->unloadAllSounds();
}
void PulmoGame::catchEvent(const EventSystem::EventPtr& e, EventSystem::EventManager& mgr)
{
	const EventSystem::Event::TypeId strType = e->type();

	if(strType==EVENT_ID_INPUT_KEY_PRESSED)
	{
		const EventSystem::DataEvent<OIS::KeyEvent>* pDataEvent = static_cast<const EventSystem::DataEvent<OIS::KeyEvent>*>(e.get());
		handleKeyDown(pDataEvent->data());
	}else
	if(strType==EVENT_ID_INPUT_KEY_RELEASED)
	{
		const EventSystem::DataEvent<OIS::KeyEvent>* pDataEvent = static_cast<const EventSystem::DataEvent<OIS::KeyEvent>*>(e.get());
		handleKeyUp(pDataEvent->data());
	}else
	if(strType==EVENT_ID_INPUT_MOUSE_MOVED)
	{
		const EventSystem::DataEvent<OIS::MouseEvent>* pDataEvent = static_cast<const EventSystem::DataEvent<OIS::MouseEvent>*>(e.get());
		handleMouseMoved(pDataEvent->data());
	}else
	if(strType==EVENT_ID_INPUT_MOUSE_KEY_PRESSED)
	{
		const EventSystem::DataEvent<MouseInfo>* pDataEvent = static_cast<const EventSystem::DataEvent<MouseInfo>*>(e.get());
		handleMousePressed(pDataEvent->data());
	}else
	if(strType==EVENT_ID_INPUT_MOUSE_KEY_RELEASED)
	{
		const EventSystem::DataEvent<MouseInfo>* pDataEvent = static_cast<const EventSystem::DataEvent<MouseInfo>*>(e.get());
		handleMouseReleased(pDataEvent->data());
	}
}
void PulmoGame::handleKeyDown(OIS::KeyEvent evt)
{
}
void PulmoGame::handleKeyUp(OIS::KeyEvent evt)
{
	if(evt.key==OIS::KC_ESCAPE)
	{
		m_currentState = GAME_STATE_BACK_TO_MENU;
	}
}
void PulmoGame::handleMouseMoved(OIS::MouseEvent evt)
{
}
void PulmoGame::handleMousePressed(MouseInfo info)
{
}
void PulmoGame::handleMouseReleased(MouseInfo info)
{
}