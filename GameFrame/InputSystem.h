#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include <OgreRenderWindow.h>
#include <OgreString.h>
#include <OgreLogManager.h>

#include <OgreWindowEventUtilities.h>

//input headers for OIS
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

//event system
#include "EventListener.h"
#include "Event.h"
#include "EventManager.h"
#include "DataEvent.h"

#include "Simpleton.h"

//event define
#define EVENT_ID_INPUT_KEY_PRESSED "EVENT_ID_INPUT_KEY_PRESSED"
#define EVENT_ID_INPUT_KEY_RELEASED "EVENT_ID_INPUT_KEY_RELEASED"
#define EVENT_ID_INPUT_MOUSE_MOVED "EVENT_ID_INPUT_MOUSE_MOVED"
#define EVENT_ID_INPUT_MOUSE_KEY_PRESSED "EVENT_ID_INPUT_MOUSE_KEY_PRESSED"
#define EVENT_ID_INPUT_MOUSE_KEY_RELEASED "EVENT_ID_INPUT_MOUSE_KEY_RELEASED"

class MouseInfo
{
public:
	MouseInfo(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
	{
		m_arg=std::shared_ptr<OIS::MouseEvent>(new OIS::MouseEvent(arg));
		m_id=id;
	}

	~MouseInfo()
	{
		if(m_arg)
		{
			;//using shared pointer for m_arg notices mem leaks
		}
		//m_arg=0;
	}

	std::shared_ptr<OIS::MouseEvent> m_arg;
	OIS::MouseButtonID m_id;
};

struct MouseButton
{
	OIS::MouseButtonID id;

	int rel;
	int abs;
	bool bAbsOnly;
};

class InputSystem: public Simpleton<InputSystem>,public Ogre::WindowEventListener,public OIS::KeyListener,public OIS::MouseListener
{
public:

	InputSystem(void);
	~InputSystem(void);

	void initInputSystem(Ogre::RenderWindow* pWindow);
	void updateDevices();

	void removeListener(EventSystem::EventListener& eventListener){ m_EventManager.removeListener(eventListener); }
	/**
		Adds your class to the listener registry
		Assume that you are listening for all the Input events
	*/
	void addListener(EventSystem::EventListener& eventListener);
	/**
		Adds your class to the listener registry
		Only listens for the specific event
	*/
	void addListener(EventSystem::EventListener& eventListener, EventSystem::Event::TypeId typeToCatch);

	OIS::Mouse* getMouseDevice(){return m_pMouse;}
	OIS::Keyboard* getKeyboardDevice(){return m_pKeyboard;}
	OIS::InputManager* getInputManager(){return m_pInputManager;}

	void processEventQueue(){m_EventManager.processEventQueue();}

protected:
	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);

	//ois members
	OIS::InputManager* m_pInputManager;
	OIS::Mouse* m_pMouse;
	OIS::Keyboard* m_pKeyboard;
	//OIS keyboard callbacks
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	//OIS mouse callbacks
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	EventSystem::EventManager m_EventManager;

	friend class Simpleton< InputSystem >;
};

#endif