#include "GameApplication.h"

GameApplication::GameApplication(void)
{
	//added the vars to lua global space
	SET_LUA_GLOBAL("MENU_STATE",MENU_STATE);
	SET_LUA_GLOBAL("PULMO_GAME_STATE",PULMO_GAME_STATE);
	SET_LUA_GLOBAL("TEST_STATE",TEST_STATE);

	m_bStateChangeQueued=false;
	m_nQueuedStateID=-1;
}

GameApplication::~GameApplication(void)
{
	m_pCurrentState=0;
	/*std::vector<GameState*>::iterator iter;

	for(iter=m_vStatesList.begin();iter!=m_vStatesList.end();iter++)
	{
		delete *iter;
	}

	m_vStatesList.clear();
	*/

	std::map<short unsigned int,GameState*>::iterator iter;

	for(iter=m_GameStateMap.begin();iter!=m_GameStateMap.end();iter++)
	{
		delete iter->second;
	}

	m_GameStateMap.clear();
}

void GameApplication::update(float fElapsedTime)
{
	if(m_bRunning)
	{
		m_pCurrentState->update(fElapsedTime);

		if(m_bStateChangeQueued)
		{
			startState(m_nQueuedStateID);
			m_bStateChangeQueued=false;
		}

		if(m_pCurrentState->getState()!=GAME_STATE_RUNNING)
		{
			if(m_pCurrentState->getState()==GAME_STATE_EXIT)
			{
				m_pCurrentState->clean();
				GameApplication::stop();
			}else
			if(m_pCurrentState->getState()==GAME_STATE_PLAY)
			{
				startState(PULMO_GAME_STATE);
			}else
			if(m_pCurrentState->getState()==GAME_STATE_BACK_TO_MENU)
			{
				startState(MENU_STATE);

				/*m_pCurrentState->clean();
				m_pCurrentState=m_vStatesList[0];
				m_pCurrentState->init();*/
			}
		}
	}
}

void GameApplication::initScene()
{
	createDummyScene();
	loadAllResources();
	destroyDummyScene();

	// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	//init scene manager
	m_pSceneMgr=m_pOgreRoot->createSceneManager(Ogre::ST_GENERIC,"DefaultSceneManager");
	//m_pSceneMgr->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);
	m_pSceneMgr->setShowDebugShadows(false);
	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(.4,.4,.4));

	initStates();
}

void GameApplication::loadAllResources()
{
	m_pTrayMgr->showLoadingBar(1, 0);
	m_pTrayMgr->hideBackdrop();
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	m_pTrayMgr->hideLoadingBar();
}

void GameApplication::initStates()
{
	m_GameStateMap[MENU_STATE] = new PulmoMainMenu(m_pRenderWindow);
	m_GameStateMap[PULMO_GAME_STATE] = new PulmoGame(m_pRenderWindow);
	m_GameStateMap[TEST_STATE] = new TestState(m_pRenderWindow);

	m_pCurrentState = m_GameStateMap[MENU_STATE];
	m_pRenderWindow->removeAllViewports();
	m_pCurrentState->init();

	/*m_vStatesList.push_back(new MenuState(m_pRenderWindow));
	m_vStatesList.push_back(new AstroidsGameState(m_pRenderWindow));
	m_vStatesList.push_back(new AstroidsGameState(m_pRenderWindow));
	//m_pMenuState=new MenuState(m_pRenderWindow);

	m_pCurrentState=m_vStatesList[2];
	m_pRenderWindow->removeAllViewports();
	m_pCurrentState->init();
	*/
}
void GameApplication::startState(short unsigned int name)
{
	GameState* state=0;
	if(!state)
	{
		std::map<short unsigned int,GameState*>::iterator iter;
		iter=m_GameStateMap.find(name);
		if(iter!=m_GameStateMap.end())
		{
			state = iter->second;
		}
	}

	if(state)
	{
		m_pCurrentState->clean();
		m_pCurrentState=state;
		m_pCurrentState->init();
	}
}

void GameApplication::consoleStartState(short unsigned int name)
{
	std::map<short unsigned int,GameState*>::iterator iter;
	iter=m_GameStateMap.find(name);
	if(iter!=m_GameStateMap.end())
	{
		m_bStateChangeQueued=true;
		m_nQueuedStateID=name;
	}
}