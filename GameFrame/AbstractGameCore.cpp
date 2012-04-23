#include "AbstractGameCore.h"

AbstractGameCore::AbstractGameCore(void)
{
	m_bRunning=true;
	m_bCompositorsEnabled=true;
	m_bDebugShadowsOn=false;
	m_bPaused=false;
}

AbstractGameCore::~AbstractGameCore(void)
{
	if(console)
	{
		console->shutdown();
		delete console;
	}

	if(LuaScriptManager::SingletonExists())
	{
		LuaScriptManager::DestroySingleton();
	}
	SoundSystem::DestroySingleton();

	UserInterface::DestroySingleton();

	delete m_pOgreRoot;
}

bool AbstractGameCore::run()
{
	#ifdef _DEBUG
		m_strPluginsCfg="plugins_d.cfg";
	#else
		m_strPluginsCfg="plugins.cfg";
	#endif

		m_pOgreRoot=new Ogre::Root(m_strPluginsCfg);
		m_pOgreRoot->addFrameListener(this);

		findDefaultResources();

		//show the config dialog and initialise systems
		/*
		In the first part of the if statement, we attempt to restore the config file.
		If that function returns false it means that the file does not exist so we should show the config dialog, which is the second portion of that if statement.
		If that also returns false it means the user cancelled out of config dialog (meaning they want to exit the program).
		*/
		if(!(m_pOgreRoot->showConfigDialog())){
			return false;
		}

		m_pRenderWindow=m_pOgreRoot->initialise(true,"^_^ JAKKA JAKKA ^_^");

		//init sound
		SoundSystem::GetSingleton()->setFileSystem("../../media/sound/");
		//init input
		InputSystem::GetSingleton()->initInputSystem(m_pRenderWindow);

		//add PhysicsSystem to lua globals
		SET_LUA_GLOBAL("PhysicsSys",PhysicsSystem::GetSingleton());

		// initialise all resource groups
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Essential");

		m_pTrayMgr=new CustomTrayManager("Tray",m_pRenderWindow,InputSystem::GetSingleton()->getMouseDevice());
		m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
		m_pTrayMgr->hideCursor();

		initScene();

		InputSystem::GetSingleton()->addListener(*this,EVENT_ID_INPUT_KEY_PRESSED);

		UserInterface::GetSingleton()->setTray(m_pTrayMgr, m_pRenderWindow);

		//init lua
		LuaScriptManager::GetSingleton()->init(Ogre::String(LUA_SCRIPTS_PATH));
		LuaScriptManager::GetSingleton()->doFile("Essential.lua");
		console = new LuaConsole();
		console->init(LuaScriptManager::GetSingleton()->getLuaState());

		m_pOgreRoot->startRendering();

		return true;
}

void AbstractGameCore::createDummyScene()
{
	m_pRenderWindow->removeAllViewports();
	Ogre::SceneManager* sm = m_pOgreRoot->createSceneManager(Ogre::ST_GENERIC, "DummyScene");
	Ogre::Camera* cam = sm->createCamera("DummyCamera");
	m_pRenderWindow->addViewport(cam);
}
void AbstractGameCore::destroyDummyScene()
{
	m_pRenderWindow->removeAllViewports();
	m_pOgreRoot->destroySceneManager(m_pOgreRoot->getSceneManager("DummyScene"));
}

void AbstractGameCore::findDefaultResources()
{
#ifdef _DEBUG
	m_strResourcesCfg="resources_d.cfg";
#else
	m_strResourcesCfg="resources.cfg";
#endif

	//setup resources
	//load resource paths from cfg file
	Ogre::ConfigFile cf;
	cf.load(m_strResourcesCfg);

	//go through the loaded file
	Ogre::ConfigFile::SectionIterator seci=cf.getSectionIterator();

	Ogre::String secName,typeName,archName;
	while(seci.hasMoreElements()){
		secName=seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap* settings=seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for(i=settings->begin();i !=settings->end();++i)
		{
			typeName=i->first;
			archName=i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName,
				typeName,
				secName
				);
		}
	}
}

void AbstractGameCore::stop()
{
	m_bRunning=false;
}

bool AbstractGameCore::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
	//exit ogre
	if(m_pRenderWindow->isClosed())
		return false;
	//update input devices
	InputSystem::GetSingleton()->updateDevices();

	//update the tray
	 m_pTrayMgr->frameRenderingQueued(evt);

	//call the update function
	if(!m_bPaused)
	{
		PhysicsSystem::GetSingleton()->update((float)evt.timeSinceLastFrame);	//update Physics
		update((float)evt.timeSinceLastFrame);
	}

	//update the sound system
	SoundSystem::GetSingleton()->update(evt.timeSinceLastFrame);
	//update sound node
	SoundNodeManager::GetSingleton()->_update(evt.timeSinceLastFrame);

	//update the tray
	 m_pTrayMgr->frameRenderingQueued(evt);

	return m_bRunning;
}

void AbstractGameCore::pause()
{
	m_bPaused=!m_bPaused;
	if(m_bPaused)
		Ogre::ControllerManager::getSingleton().setTimeFactor(0);
	else
		Ogre::ControllerManager::getSingleton().setTimeFactor(1);
}

void AbstractGameCore::handleDebugInput(OIS::KeyEvent evt)
{
	if(LuaConsole::getSingleton().isVisible())
		LuaConsole::getSingleton().injectKeyPress(evt);
	/*OIS::Keyboard* pKeyboard=InputSystem::GetSingleton()->getKeyboardDevice();
	//F1 to turn off any post-process effects
	if(pKeyboard->isKeyDown(OIS::KC_F1))
	{
		m_bCompositorsEnabled=!m_bCompositorsEnabled;

		Ogre::CompositorChain* chain = Ogre::CompositorManager::getSingleton().getCompositorChain(m_pRenderWindow->getViewport(0));
		int n = chain->getNumCompositors();
		//loop through and disable all
		for(int i=0;i<n;i++)
		{
			if(m_bCompositorsEnabled)
				chain->setCompositorEnabled(i,false);
			else
				chain->setCompositorEnabled(i,true);
		}
	}*/

	switch(evt.key)
	{
	//press "`" to turn on console
	case OIS::KC_GRAVE:				//this is the "`" or "~" key
	case OIS::KC_KANJI:				//in some machines (like mine >_<) it thinks its KANJI
		LuaConsole::getSingleton().setVisible( ! LuaConsole::getSingleton().isVisible() );
		pause();
		SoundSystem::GetSingleton()->togglePauseAll();
		break;
	//F1 to toggle post processing
	case OIS::KC_F1:
		{
			m_bCompositorsEnabled=!m_bCompositorsEnabled;

			Ogre::CompositorChain* chain = Ogre::CompositorManager::getSingleton().getCompositorChain(m_pRenderWindow->getViewport(0));
			int n = chain->getNumCompositors();
			//loop through and disable all
			for(int i=0;i<n;i++)
			{
				if(m_bCompositorsEnabled)
					chain->setCompositorEnabled(i,false);
				else
					chain->setCompositorEnabled(i,true);
			}
		}
		break;
		//F2 to toggle debug shadows
		case OIS::KC_F2:
		m_pSceneMgr->setShowDebugShadows(m_bDebugShadowsOn);
		m_bDebugShadowsOn=!m_bDebugShadowsOn;
		break;
	}
}

void AbstractGameCore::catchEvent(const EventSystem::EventPtr& e, EventSystem::EventManager& mgr)
{
	const EventSystem::Event::TypeId strType = e->type();
	const EventSystem::DataEvent<OIS::KeyEvent>* pDataEvent = static_cast<const EventSystem::DataEvent<OIS::KeyEvent>*>(e.get());

	handleDebugInput(pDataEvent->data());
}