#include "InputSystem.h"
//init simpleton instance
InputSystem* Simpleton<InputSystem>::s_singleton=0;

InputSystem::InputSystem(void)
{
}

InputSystem::~InputSystem(void)
{
	m_EventManager.clear();
}

void InputSystem::initInputSystem(Ogre::RenderWindow* pWindow)
{
		//set up OIS
		Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
		OIS::ParamList pl;
		size_t windowHnd=0;
		std::ostringstream windowHndStr;

		pWindow->getCustomAttribute("WINDOW",&windowHnd);
		windowHndStr<<windowHnd;
		pl.insert(std::make_pair(std::string("WINDOW"),windowHndStr.str()));

		m_pInputManager=OIS::InputManager::createInputSystem(pl);
		m_pKeyboard=static_cast<OIS::Keyboard*>(m_pInputManager->createInputObject(OIS::Type::OISKeyboard,true));
		m_pMouse=static_cast<OIS::Mouse*>(m_pInputManager->createInputObject(OIS::Type::OISMouse,true));

		Ogre::WindowEventUtilities::addWindowEventListener(pWindow,this);

		m_pKeyboard->setEventCallback(this);
		m_pMouse->setEventCallback(this);

		//set initial mouse clipping area
		//windowResized(mWindow);
}

void InputSystem::updateDevices()
{
	m_pKeyboard->capture();
	m_pMouse->capture();

	//tell event manager to process the queue of events
	m_EventManager.processEventQueue();
}
//TODO: Need to finish this for mouse input
void InputSystem::windowResized(Ogre::RenderWindow* rw){}
void InputSystem::windowClosed(Ogre::RenderWindow* rw)
{
		if(m_pInputManager)
		{
			m_pInputManager->destroyInputObject(m_pMouse);
			m_pInputManager->destroyInputObject(m_pKeyboard);

			OIS::InputManager::destroyInputSystem(m_pInputManager);
			m_pInputManager=0;
		}
}

bool InputSystem::keyPressed( const OIS::KeyEvent &arg )
{
	// Create and send events
	EventSystem::EventPtr ptr = EventSystem::makeDataEvent(EVENT_ID_INPUT_KEY_PRESSED, arg);
	m_EventManager.broadcastEvent(ptr,false);
	return true;
}

bool InputSystem::keyReleased( const OIS::KeyEvent &arg )
{
	// Create and send events
	EventSystem::EventPtr ptr = EventSystem::makeDataEvent(EVENT_ID_INPUT_KEY_RELEASED, arg);
	m_EventManager.broadcastEvent(ptr,false);
	return true;
}

bool InputSystem::mouseMoved( const OIS::MouseEvent &arg )
{
	EventSystem::EventPtr ptr = EventSystem::makeDataEvent(EVENT_ID_INPUT_MOUSE_MOVED, arg);
	m_EventManager.broadcastEvent(ptr,false);
	return true;
}

bool InputSystem::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	MouseInfo data(arg,id);
	EventSystem::EventPtr ptr = EventSystem::makeDataEvent(EVENT_ID_INPUT_MOUSE_KEY_PRESSED, data);
	m_EventManager.broadcastEvent(ptr,false);
	return true;
}

bool InputSystem::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	MouseInfo data(arg,id);
	EventSystem::EventPtr ptr = EventSystem::makeDataEvent(EVENT_ID_INPUT_MOUSE_KEY_RELEASED, data);
	m_EventManager.broadcastEvent(ptr,false);
	return true;
}

void InputSystem::addListener(EventSystem::EventListener& eventListener)
{
	m_EventManager.addListener(eventListener,EVENT_ID_INPUT_KEY_PRESSED);
	m_EventManager.addListener(eventListener,EVENT_ID_INPUT_KEY_RELEASED);
	m_EventManager.addListener(eventListener,EVENT_ID_INPUT_MOUSE_KEY_PRESSED);
	m_EventManager.addListener(eventListener,EVENT_ID_INPUT_MOUSE_KEY_RELEASED);
	m_EventManager.addListener(eventListener,EVENT_ID_INPUT_MOUSE_MOVED);
}
void InputSystem::addListener(EventSystem::EventListener& eventListener, EventSystem::Event::TypeId typeToCatch)
{
	m_EventManager.addListener(eventListener,typeToCatch);
}