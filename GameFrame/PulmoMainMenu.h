#ifndef PULMO_MAIN_MENU
#define PULMO_MAIN_MENU

#define GAME_STATE_PLAY "GAME_STATE_PLAY"	//the play game is selected by player
#define GAME_STATE_EXIT "GAME_STATE_EXIT"

#include "GameState.h"
#include "SoundSystem.h"
#include "DotSceneLoader.h"
#include "InputSystem.h"
#include "GAME_STATE_ID.h"

#include <assert.h>

class PulmoMainMenu : public GameState
{
public:
	PulmoMainMenu(Ogre::RenderWindow*  pRenderWindow);
	~PulmoMainMenu(void);

	void init();
	void update(float elapsedTime);
	void clean();

private:
	void catchEvent(const EventSystem::EventPtr& e, EventSystem::EventManager& mgr);
	inline void handleKeyDown(OIS::KeyEvent evt);
	inline void handleKeyUp(OIS::KeyEvent evt);
	inline void handleMouseMoved(OIS::MouseEvent evt);
	inline void handleMousePressed(MouseInfo info);
	inline void handleMouseReleased(MouseInfo info);

	Ogre::Camera* m_pPlayerCam;
	Ogre::RenderWindow* m_pRenderWindow;
	Ogre::Viewport* m_pViewPort;
	Ogre::SceneManager* m_pSceneManager;
};

#endif