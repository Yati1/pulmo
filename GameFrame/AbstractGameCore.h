#ifndef ABSTRACT_GAME_CORE
#define ABSTRACT_GAME_CORE

#include <OgreException.h>
#include <OgreRoot.h>//has root
#include <OgreConfigFile.h>//to use the cfg file parser
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>
#include <OgreCompositorManager.h>
#include <OgreCompositorChain.h>

//input headers for OIS
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

//ogre bites
#include "CustomTrayManager.h"

#include "EventListener.h"
#include "InputSystem.h"
#include "SoundSystem.h"
#include "PhysicsSystem.h"
#include "SoundNodeManager.h"
#include "LuaScriptManager.h"
#include "UserInterface.h"

#include "LuaConsole.h"

class AbstractGameCore:public Ogre::FrameListener, EventSystem::EventListener
{
public:
	AbstractGameCore(void);
	virtual ~AbstractGameCore(void);
	/*
	Initializes Ogre then starts rendering
	*/
	bool run();

	virtual void pause();

	virtual void stop();

protected:

	virtual void update(float fElapsedTime)=0;

	virtual void initScene()=0;
	/*
		Create a dummy scene to facilitate the loading screen
	*/
	virtual void createDummyScene();
	/*
		Destroy th dummyScene
	*/
	virtual void destroyDummyScene();

	Ogre::Root* m_pOgreRoot;
	Ogre::RenderWindow* m_pRenderWindow;
	Ogre::SceneManager* m_pSceneMgr;
	CustomTrayManager* m_pTrayMgr;

	bool m_bPaused;
	bool m_bRunning;

private:

	/*
	Calls the update function
	Returns m_bRunning
	*/
	bool frameRenderingQueued(const Ogre::FrameEvent &evt);

	inline void findDefaultResources();

	void catchEvent(const EventSystem::EventPtr& e, EventSystem::EventManager& mgr);
	inline void handleDebugInput(OIS::KeyEvent evt);
	bool m_bCompositorsEnabled;		//whether the post-process effeccts are on
	bool m_bDebugShadowsOn;			//whether debug shadows are on

	//cfg file paths
	//lists plugins
	Ogre::String m_strPluginsCfg;
	//lists all resources, This includes the textures, models, scripts, and so on.
	Ogre::String m_strResourcesCfg;

	LuaConsole* console;
};
#endif