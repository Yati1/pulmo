#ifndef GAME_APPLICATION_H
#define GAME_APPLICATION_H

#define INTERNAL_GAME_STATE_MENU 0

#include <vector>
#include <map>
#include <OgreMeshManager.h>

#include "InputSystem.h"

#include "AbstractGameCore.h"

#include "GameState.h"
//these are the game states
#include "GAME_STATE_ID.h"

#include "PulmoMainMenu.h"
#include "PulmoGame.h"
#include "TestState.h"

class GameApplication :
	public AbstractGameCore
{
public:
	GameApplication(void);
	~GameApplication(void);

	void update(float fElapsedTime);
	void initScene();

	void startState(short unsigned int name);

	void consoleStartState(short unsigned int name);
private:
	inline void loadAllResources();
	void initStates();

	Ogre::Camera* m_pPlayerCam;

	GameState* m_pCurrentState;

	bool m_bStateChangeQueued;	//whether there is a state change queue from using the console

	int m_nQueuedStateID;	//the queued state's ID

	//std::vector<GameState*> m_vStatesList;

	std::map<short unsigned int,GameState*> m_GameStateMap;	//map of game states. With state name as the key
};

#endif