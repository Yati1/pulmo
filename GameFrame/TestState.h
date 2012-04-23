#ifndef TEST_STATE_H
#define TEST_STATE_H

#include "GameState.h"

#include "InputSystem.h"

#include "SoundSystem.h"

#include "SoundNodeManager.h"

#include "DotSceneLoader.h"

#include "ThirdPersonCameraController.h"

#include "PhysicsSystem.h"

#include "Controller.h"

#include "PulmoActor.h"

#include "PulmoController.h"

#include "UserInterface.h"

#include "Level.h"

#include <Ogre.h>

#include <SdkCameraMan.h>

#include <OGRE/Terrain/OgreTerrain.h>
#include <OGRE/Terrain/OgreTerrainGroup.h>

#define GAME_STATE_BACK_TO_MENU "GAME_STATE_BACK_TO_MENU"
/*
	A class the test shit out in
*/
class TestState:public GameState
{
public:
	TestState(Ogre::RenderWindow*  pRenderWindow);
	virtual ~TestState(void);

	virtual void init();
	virtual void update(float elapsedTime);
	virtual void clean();

private:
	//unloads current level, loads next level & reinits the sub systems
	void changeLevel(const int& oldLevel,const int& newLevel);
	unsigned short int m_nCurrentLevel;
	std::vector<Level*> m_Levels;
	inline void initLevel();
	inline void clearLevels();

	inline void setUpPlayerController();
	inline void setUpPhysics();
	inline void destroyPhysics();
	inline void destroyPlayerController();

	void catchEvent(const EventSystem::EventPtr& e, EventSystem::EventManager& mgr);
	inline void handleKeyDown(OIS::KeyEvent evt);
	inline void handleKeyUp(OIS::KeyEvent evt);
	inline void handleMouseMoved(OIS::MouseEvent evt);
	inline void handleMousePressed(MouseInfo info);
	inline void handleMouseReleased(MouseInfo info);

	//void initBlendMaps(Ogre::Terrain* terrain);
	void setupHeightFields();

	Ogre::RenderWindow* m_pRenderWindow;
	Ogre::Viewport* m_pViewPort;
	Ogre::SceneManager* m_pSceneManager;

	Ogre::Camera* m_pPlayerCam;

	SoundNode* m_pSound;

	Ogre::SceneNode* m_pSoundNode;
	Ogre::SceneNode* m_pRedBox;

	NxOgre::Scene* m_pPhyxScene;
	Critter::RenderSystem* m_pRenderSys;

	Ogre::TerrainGroup* m_pTerrainGroup;
	Ogre::TerrainGlobalOptions* m_pTerrainGlobals;

	//from terrrain tut
	bool mTerrainsImported;

	void defineTerrain(long x,long y);
	void initBlendMaps(Ogre::Terrain* terrain);
	void configureTerrainDefaults(Ogre::Light* light);

	void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img);

	Controller* m_pCharController;
	PlayerController* m_pPlayerController;

	//boolean to pause the game
	bool m_bPause;

	Hikari::FlashValue onExitClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue onResumeClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);

//	Ogre::RaySceneQuery* m_pRaySceneQuery;
};

#endif