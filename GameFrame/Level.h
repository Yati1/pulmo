#ifndef LEVEL_H
#define LEVEL_H

#include <Ogre.h>
#include <OGRE/Terrain/OgreTerrain.h>
#include <OGRE/Terrain/OgreTerrainGroup.h>

#include <luabind\luabind.hpp>
#include "LuaScriptManager.h"
#include "DotSceneLoader.h"

class Level
{
public:
	Level(const Ogre::String& luaFileName,const Ogre::String& luaObjName);
	~Level();

	void load(Ogre::SceneManager* sceneManager,	NxOgre::Scene* phyxScene, Critter::RenderSystem* renderSys);
	void unLoad();
private:
	//the lua object that is a table with 2 functions
	//	* Load - which sets up the level by loading terrain, parsing dot scene
	//	* Unload - which unloads the level
	//it also has two string variables
	//	* HieghtMap - stores the name of the hieght map file eg: HightMap.jpg
	//	* Scene - stores thw name of the DotScene file eg: TestScene.scene
	luabind::object m_obj;

	Ogre::String m_strSceneName;
	Ogre::String m_strHieghtMap;
	//pointers to the objects in the game state
	Ogre::SceneManager* m_pSceneManager;
	NxOgre::Scene* m_pPhyxScene;
	Critter::RenderSystem* m_pRenderSys;

	//terrain details
	Ogre::TerrainGroup* m_pTerrainGroup;
	Ogre::TerrainGlobalOptions* m_pTerrainGlobals;
	bool mTerrainsImported;
	//terrain creation functions
	void defineTerrain(long x,long y);
	void initBlendMaps(Ogre::Terrain* terrain);
	void configureTerrainDefaults(Ogre::Light* light);
	void getTerrainImage(bool flipX, bool flipY,const Ogre::String& hieghtMap, Ogre::Image& img);
	void setupHeightFields();
};

#endif