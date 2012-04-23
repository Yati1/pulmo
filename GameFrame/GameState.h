#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "EventListener.h"
#include "InputSystem.h"

#define GAME_STATE_RUNNING "GAME_STATE_RUNNING" //the game-state is running
/************************
GameState

Abstract class representing a game-state

Game-states are very important to my software architecture. The game runs as a state machine. Game-State is one particular state.
Each concrete game-state has to implement:
	init - to initialize the game-state, called before the state starts to run
	update - to update the state
	render - to render the game state
	clean - to end the game-state cleanly, called when the state has to stop running

In past games there has been a StateManager that manages the collection of game-state that run the game but due to time constraints
that functionality is the responsibility of the class that is the concrete class of AbstractGameCore

m_currentState stores the current state of the game-state. The state of a game-state is a #define beginning with GAME_STATE_<add state name>
The nice thing about this is that while all concrete classes of GameState will have visibility of GAME_STATE_RUNNING they need not have visibility
of other GAME_STATEs. The class that manages the states will have visibility of all the various GAME_STATEs.

GAME_STATEs are primarily used to notify the manager of the current status of a state and when the state ends which state to run next.

While the m_currentState is GAME_STATE_RUNNING the state will continue running

************************/
class GameState:public EventSystem::EventListener
{
public:
	GameState(void);
	virtual ~GameState(void);

	virtual void init()=0;

	virtual void update(float elapsedTime)=0;
	virtual void clean()=0;

	char* getState()const;

protected:
	virtual void catchEvent(const EventSystem::EventPtr& e, EventSystem::EventManager& mgr)=0;
	char* m_currentState;
};
#endif