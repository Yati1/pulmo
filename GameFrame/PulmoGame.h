#ifndef PULMO_GAME_H
#define PULMO_GAME_H

#define GAME_STATE_BACK_TO_MENU "GAME_STATE_BACK_TO_MENU"

#include "GameState.h"
#include "SoundSystem.h"
#include "DotSceneLoader.h"
#include "InputSystem.h"
#include "ParticleEffectManager.h"
#include "PhysicsSystem.h"
#include "GAME_STATE_ID.h"

class PulmoGame : public GameState
{
public:

	PulmoGame(Ogre::RenderWindow*  pRenderWindow);
	~PulmoGame(void);

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