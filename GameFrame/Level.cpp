#include "Level.h"

#include <assert.h>

Level::Level(const Ogre::String& luaFileName,const Ogre::String& luaObjName)
{
	LuaScriptManager::GetSingleton()->doFile(luaFileName.c_str());
	m_obj = LuaScriptManager::GetSingleton()->getLuaGlobal(luaObjName.c_str());
	assert(m_obj.is_valid());	//check whether the object retrieved from Lua is valid

	//m_strSceneName = sceneName;
}
Level::~Level()
{
}

void Level::load(Ogre::SceneManager* sceneManager,	NxOgre::Scene* phyxScene, Critter::RenderSystem* renderSys)
{
	m_pSceneManager = sceneManager;
	m_pPhyxScene = phyxScene;
	m_pRenderSys = renderSys;

	assert(m_obj.is_valid());
	if(m_obj.is_valid())
	{
		m_strHieghtMap = luabind::object_cast<Ogre::String>(m_obj["HieghtMap"]);	//get Hieghtmap name from lua
		m_strSceneName = luabind::object_cast<Ogre::String>(m_obj["Scene"]);		//get scene name from lua
	}
	assert(!m_strHieghtMap.empty());
	assert(!m_strSceneName.empty());

	Ogre::DotSceneLoader loader = Ogre::DotSceneLoader();
	loader.parseDotScene(m_strSceneName,"Popular",m_pSceneManager,true,m_pPhyxScene,m_pRenderSys);

	//set up terrain
	m_pTerrainGlobals= OGRE_NEW Ogre::TerrainGlobalOptions();

	m_pTerrainGroup=OGRE_NEW Ogre::TerrainGroup(m_pSceneManager,Ogre::Terrain::ALIGN_X_Z, 513, 513);
	m_pTerrainGroup->setFilenameConvention("BasicTutorial3Terrain","dat");
	m_pTerrainGroup->setOrigin(Ogre::Vector3::ZERO);
	configureTerrainDefaults(m_pSceneManager->getLight("MAIN_LIGHT"));

	for (long x = 0; x <= 0; ++x)
        for (long y = 0; y <= 0; ++y)
            defineTerrain(x, y);

    // sync load since we want everything in place when we start
    m_pTerrainGroup->loadAllTerrains(true);

	//if the terrains were imported calculate blend maps
	if(mTerrainsImported)
	{
		Ogre::TerrainGroup::TerrainIterator ti=m_pTerrainGroup->getTerrainIterator();
		while(ti.hasMoreElements())
		{
			Ogre::Terrain* t=ti.getNext()->instance;
			initBlendMaps(t);
		}
	}

	//clean up
	m_pTerrainGroup->freeTemporaryResources();
	//from tut 3 end

	//set up PhysX terrain
	setupHeightFields();

	assert(m_obj.is_valid());
	if(m_obj.is_valid())
	{
		m_obj["Load"]();	//call the lua object's Loaded
	}
}

void Level::unLoad()
{
	//destroy every thing except camera
	m_pSceneManager->destroyAllAnimations();
	m_pSceneManager->destroyAllAnimationStates();
	m_pSceneManager->destroyAllBillboardChains();
	m_pSceneManager->destroyAllBillboardSets();
	m_pSceneManager->destroyAllEntities();
	m_pSceneManager->destroyAllInstancedGeometry();
	m_pSceneManager->destroyAllLights();
	m_pSceneManager->destroyAllManualObjects();
	m_pSceneManager->destroyAllMovableObjects();
	m_pSceneManager->destroyAllParticleSystems();
	m_pSceneManager->destroyAllRibbonTrails();
	m_pSceneManager->destroyAllStaticGeometry();

	//m_pSceneManager->clearScene();

	OGRE_DELETE m_pTerrainGroup;
	OGRE_DELETE m_pTerrainGlobals;

	TriggerManager::GetSingleton()->clearAll();

	assert(m_obj.is_valid());
	if(m_obj.is_valid())
	{
		m_obj["Unload"]();	//call the lua object's Unloaded
	}
}

void Level::defineTerrain(long x,long y)
{
	Ogre::String filename = m_pTerrainGroup->generateFilename(x, y);
    if (Ogre::ResourceGroupManager::getSingleton().resourceExists(m_pTerrainGroup->getResourceGroup(), filename))
    {
        m_pTerrainGroup->defineTerrain(x, y);
    }
    else
    {
        Ogre::Image img;
        getTerrainImage(x % 2 != 0, y % 2 != 0, m_strHieghtMap, img);
        m_pTerrainGroup->defineTerrain(x, y, &img);
        mTerrainsImported = true;
    }
}

void Level::initBlendMaps(Ogre::Terrain* terrain)
{
	Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);

	Ogre::Real minHeight0 = 5.5f;	//Ogre::Real minHeight0 = 3.5f;
    Ogre::Real fadeDist0 = 10.0f;	//Ogre::Real fadeDist0 = 2.0f;
    Ogre::Real minHeight1 = 6.5f;	//Ogre::Real minHeight1 = 3.5f;
	Ogre::Real fadeDist1 = 10.7f;	//Ogre::Real fadeDist1 = 0.7f;
    float* pBlend1 = blendMap1->getBlendPointer();
    for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
    {
        for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
        {
            Ogre::Real tx, ty;

            blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
            Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
            Ogre::Real val = (height - minHeight0) / fadeDist0;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);

            val = (height - minHeight1) / fadeDist1;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend1++ = val;
        }
    }
    blendMap0->dirty();
    blendMap1->dirty();
    //blendMap0->loadImage("blendmap1.png", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    blendMap0->update();
    blendMap1->update();
}

void Level::configureTerrainDefaults(Ogre::Light* light)
{
	// Configure global
    m_pTerrainGlobals->setMaxPixelError(8);
    // testing composite map
    m_pTerrainGlobals->setCompositeMapDistance(3000);

	// Important to set these so that the terrain knows what to use for derived (non-realtime) data
    m_pTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
    m_pTerrainGlobals->setCompositeMapAmbient(m_pSceneManager->getAmbientLight());
    m_pTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());

	// Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& defaultimp = m_pTerrainGroup->getDefaultImportSettings();
    /*defaultimp.terrainSize = 513;//the same as the terrain global
    defaultimp.worldSize = 513;//the same as the terrain global
    defaultimp.inputScale = 600;//how the height map is scaled up
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;*/

	defaultimp.terrainSize = 513;
	defaultimp.worldSize = 513; //My edit
	defaultimp.inputScale = 30.78f;
	defaultimp.minBatchSize = 33; //ori
	defaultimp.maxBatchSize = 65; //ori

	// textures
	// textures
	  defaultimp.layerList.resize(3);
	  //defaultimp.layerList[0].worldSize = 100;	//Ori
	  defaultimp.layerList[0].worldSize = 5;
	  defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
	  defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
	  //defaultimp.layerList[1].worldSize = 30;	//Ori'
	  defaultimp.layerList[1].worldSize = 1.5f;
	  defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
	  defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
	  //defaultimp.layerList[2].worldSize = 200;	//Ori
	  defaultimp.layerList[2].worldSize = 10.26f;
	  defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
	  defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
}

void Level::getTerrainImage(bool flipX, bool flipY,const Ogre::String& hieghtMap, Ogre::Image& img)
{
	img.load(hieghtMap, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    if (flipX)
        img.flipAroundY();
    if (flipY)
        img.flipAroundX();
}

void Level::setupHeightFields()
{
	Ogre::TerrainGroup::TerrainIterator ti = m_pTerrainGroup->getTerrainIterator();
	while(ti.hasMoreElements())
	{
		Ogre::Terrain* t = ti.getNext()->instance;
		m_pRenderSys->createTerrain(t);
	}
}