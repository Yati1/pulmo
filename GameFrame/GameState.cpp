#include "GameState.h"

GameState::GameState(void)
{
	m_currentState=GAME_STATE_RUNNING;
}

GameState::~GameState(void)
{
}

char* GameState::getState()const
{
	return m_currentState;
}