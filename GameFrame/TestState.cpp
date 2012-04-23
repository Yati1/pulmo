#include "TestState.h"

TestState::TestState(Ogre::RenderWindow*  pRenderWindow)
{
	m_nCurrentLevel = 0;
	m_pSceneManager=Ogre::Root::getSingleton().getSceneManager("DefaultSceneManager");
	m_pRenderWindow=pRenderWindow;

	m_pSound =0;
}

TestState::~TestState(void)
{
}

void TestState::init()
{
	m_bPause = false;

	PhysicsSystem::GetSingleton()->setPaused(false);

	m_pSceneManager->setAmbientLight(Ogre::ColourValue(1.0f, 0.5f, 0.5f));
	m_pSceneManager->setDisplaySceneNodes(false);

	//sign up for everything
	InputSystem::GetSingleton()->addListener(*this);
	m_currentState=GAME_STATE_RUNNING;

	setUpPhysics();

	Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
    Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(7);

	initLevel();	//initialize levels
	/*
	m_pPlayerCam=m_pSceneManager->createCamera("PlayerCam");
	m_pViewPort=m_pRenderWindow->addViewport(m_pPlayerCam);
	m_pViewPort->setBackgroundColour(Ogre::ColourValue(0.0,0.0,0.0));

	// Alter the camera aspect ratio to match the viewport
	m_pPlayerCam->setAspectRatio(
	Ogre::Real(m_pViewPort->getActualWidth()) / Ogre::Real(m_pViewPort->getActualHeight()));

	m_pViewPort->setCamera(m_pPlayerCam);
	*/

	//std::auto_ptr<Ogre::DotSceneLoader> sceneLoader(new Ogre::DotSceneLoader());
	//sceneLoader->parseDotScene("TestScene.scene", "Popular", m_pSceneManager,true,m_pPhyxScene,m_pRenderSys);

	//make terrain
#pragma region The old terrain creation stuff is commented out
	/////FROM TUT 3////////////
	/*Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
    Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(7);
	//set up lights
	Ogre::Vector3 lightdir(0.55,-0.3,-0.75);
	lightdir.normalise();

	m_pSceneManager->setAmbientLight(Ogre::ColourValue(0.2,0.2,0.2));

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

	NxOgre::SphereDescription sphereDesc;
    sphereDesc.mRadius = 1.0f;
    sphereDesc.mMass = 1.0f;
    sphereDesc.mDensity = 1.0f;

	//Ogre::SceneNode* n = m_pSceneManager->getRootSceneNode()->createChildSceneNode("PlayerCamNode");
	//n->attachObject(m_pPlayerCam);

	*/
	/*NxOgre::RigidBodyDescription bodyDesc;
	bodyDesc.mMass=1.0f;
	bodyDesc.mDensity=0.0f;
	bodyDesc.mLinearDamping=10.0f;
	bodyDesc.mAngularDamping=10.0f;
	bodyDesc.mDynamicRigidbodyFlags |= NxOgre::DynamicRigidbodyFlags::FreezeRotation;
	bodyDesc.mDynamicRigidbodyFlags |= NxOgre::DynamicRigidbodyFlags::DisableGravity;*/

	//m_pRenderSys->setVisualisationMode(NxOgre::Enums::VisualDebugger_ShowAll);
#pragma endregion
	//------------Nik's test section------------------
	//Ogre::SceneNode* myBody = m_pSceneManager->getRootSceneNode()->createChildSceneNode(); //(0,75,0)

	/*Critter::BodyDescription bodyDesc;
    bodyDesc.mMass = 40.0f;
    bodyDesc.mDensity = 0.0f;
	bodyDesc.mLinearDamping=0.2f;
	bodyDesc.mAngularDamping= 0.0f;
    bodyDesc.mDynamicRigidbodyFlags|= NxOgre::DynamicRigidbodyFlags::FreezeRotation;
	bodyDesc.mNode = m_pSceneManager->getSceneNode("Player_StartNode");

	NxOgre::CapsuleDescription capsDesc;
	capsDesc.mHeight=6.0f;
	capsDesc.mRadius=2.f;
	capsDesc.mDensity=0.f;
	capsDesc.mMass=40.0f;

	Ogre::Entity* ent = m_pSceneManager->createEntity("pulmo.mesh");

	m_pPlayerCam->setNearClipDistance(0.1);
    m_pPlayerCam->setFarClipDistance(100);

	Critter::Body* bod = m_pRenderSys->createBody(capsDesc,PhysicsSystem::GetSingleton()->convertOgreVec3(bodyDesc.mNode->getPosition()),ent,bodyDesc);

	//------------Nik's test section------------------
	ent->getParentSceneNode()->scale(0.7,0.7,0.7);
	PulmoActor* player = new PulmoActor(ent, bod, ActorManager::GetSingleton());
	ActorManager::GetSingleton()->addActor(player);
	m_pPlayerController = new PulmoController(player, m_pPlayerCam);
	m_pPlayerController->setupCamera();
	*/

	m_pSceneManager->setAmbientLight(Ogre::ColourValue(0.2,0.2,0.2));

	Ogre::ColourValue fadeColour(0.1, 0.4, 0.5);
	//m_pSceneManager->setFog(Ogre::FOG_EXP, fadeColour, -0.005);
	m_pSceneManager->setFog(Ogre::FOG_LINEAR, fadeColour, 1.0f, 0.1, 100);

	changeLevel(m_nCurrentLevel,1);
	m_pRenderWindow->getViewport(0)->setBackgroundColour(fadeColour);
	PhysicsSystem::GetSingleton()->update(0.1f);

	//-----------UI initialization--------------
	m_bPause = false;
	UserInterface::GetSingleton()->loadFlash("PauseMenu.swf", m_pViewPort, "Pause_Menu", 550, 400, Hikari::Center ,1.0f);
	UserInterface::GetSingleton()->loadFlash("BlurryScreen.swf", m_pViewPort, "Blurry", m_pRenderWindow->getWidth(), m_pRenderWindow->getHeight(), Hikari::Center, 0.8f);
	UserInterface::GetSingleton()->bindFlash("Pause_Menu", "resumeClick", Hikari::FlashDelegate(this, &TestState::onResumeClick));
	UserInterface::GetSingleton()->bindFlash("Pause_Menu", "exitClick", Hikari::FlashDelegate(this, &TestState::onExitClick));
}
void TestState::update(float elapsedTime)
{
//	m_pCharController->update(elapsedTime); -------un-comment this after test-------
//	m_pCamMan->update(elapsedTime); -------un-comment this after test-------

	//SoundSystem::GetSingleton()->setListener(m_pCamMan->getSoundListener());

	if (m_bPause == false)
	{
		//-------Nik's test section-------
		ActorManager::GetSingleton()->update(elapsedTime);
		m_pPlayerController->update(elapsedTime);
	}
	else
	{
		UserInterface::GetSingleton()->displayFlash("Blurry");
		UserInterface::GetSingleton()->displayFlash("Pause_Menu");
		UserInterface::GetSingleton()->update();
	}
}

void TestState::setUpPlayerController()
{
	m_pPlayerCam=m_pSceneManager->createCamera("PlayerCam");
	m_pViewPort=m_pRenderWindow->addViewport(m_pPlayerCam);
	m_pViewPort->setBackgroundColour(Ogre::ColourValue(0.0,0.0,0.0));

	// Alter the camera aspect ratio to match the viewport
	m_pPlayerCam->setAspectRatio(
	Ogre::Real(m_pViewPort->getActualWidth()) / Ogre::Real(m_pViewPort->getActualHeight()));

	m_pViewPort->setCamera(m_pPlayerCam);

	Critter::BodyDescription bodyDesc;
    bodyDesc.mMass = 40.0f;
    bodyDesc.mDensity = 0.0f;
	bodyDesc.mLinearDamping=0.2f;
	bodyDesc.mAngularDamping= 0.0f;
    bodyDesc.mDynamicRigidbodyFlags|= NxOgre::DynamicRigidbodyFlags::FreezeRotation;
	bodyDesc.mNode = m_pSceneManager->getSceneNode("Player_StartNode");

	NxOgre::CapsuleDescription capsDesc;
	capsDesc.mHeight=6.0f;
	capsDesc.mRadius=2.f;
	capsDesc.mDensity=0.f;
	capsDesc.mMass=40.0f;

	Ogre::Entity* ent = m_pSceneManager->createEntity("pulmo.mesh");

	m_pPlayerCam->setNearClipDistance(0.1);
    m_pPlayerCam->setFarClipDistance(100);

	Critter::Body* bod = m_pRenderSys->createBody(capsDesc,PhysicsSystem::GetSingleton()->convertOgreVec3(bodyDesc.mNode->getPosition()),ent,bodyDesc);

	ent->getParentSceneNode()->scale(0.7,0.7,0.7);
	PulmoActor* player = new PulmoActor(ent, bod, ActorManager::GetSingleton());
	ActorManager::GetSingleton()->addActor(player);
	m_pPlayerController = new PulmoController(player, m_pPlayerCam);
	m_pPlayerController->setupCamera();
}
void TestState::setUpPhysics()
{
	NxOgre::SceneDescription scene_description;
	scene_description.mGravity = NxOgre::Constants::MEAN_EARTH_GRAVITY;
	scene_description.mUseHardware = false;
	scene_description.mTimeStepMethod = NxOgre::Enums::TimeStepMethod_Variable;
	//scene_description.mMaxSubSteps = 4;
	//scene_description.mMaxTimeStep = NxOgre::_1_30 / scene_description.mMaxSubSteps;
	//scene_description.mTimeStepMethod = NxOgre::Enums::TimeStepMethod_Inherit;
	//set up physx scene
	//m_pPhyxScene=PhysicsSystem::GetSingleton()->getWorld()->createScene(scene_description);
	m_pPhyxScene=PhysicsSystem::GetSingleton()->createScene(scene_description);

	PhysicsSystem::GetSingleton()->createDefaultMaterial(m_pPhyxScene->getMaterial(0));

	m_pRenderSys=PhysicsSystem::GetSingleton()->createRenderSystem(m_pSceneManager);

	TriggerManager::GetSingleton()->init(m_pSceneManager,m_pPhyxScene,ActorManager::GetSingleton());
}
void TestState::destroyPhysics()
{
	PhysicsSystem::GetSingleton()->destroyRenderSystem(m_pRenderSys);
	m_pRenderSys=0;

	PhysicsSystem::GetSingleton()->destroyScene(m_pPhyxScene);
	m_pPhyxScene=0;

	delete m_pRenderSys;
	m_pRenderSys=0;

	PhysicsSystem::GetSingleton()->getWorld()->destroyScene(m_pPhyxScene);
	m_pPhyxScene=0;

	TriggerManager::GetSingleton()->clearAll();
}
void TestState::destroyPlayerController()
{
	if(m_pPlayerController)
	{
		delete m_pPlayerController;
		m_pPlayerController = 0;
	}
}

void TestState::initLevel()
{
	m_Levels.push_back(new Level("Level1.lua","Level1"));
}

void TestState::clearLevels()
{
	if(!m_Levels.empty())
	{
		std::vector<Level*>::iterator iter;
		for(iter = m_Levels.begin(); iter != m_Levels.end(); iter++)
			delete *iter;
	}
	m_Levels.clear();
}

void TestState::changeLevel(const int& oldLevel,const int& newLevel)
{
	if(oldLevel != 0)
	{
		m_Levels[m_nCurrentLevel-1]->unLoad();
		m_pSceneManager->clearScene();
		SoundNodeManager::GetSingleton()->emptyManager();
		SoundSystem::GetSingleton()->unloadAllSounds();

		destroyPlayerController();
		destroyPhysics();
	}

	if(newLevel <= m_Levels.size())
	{
		setUpPhysics();
		m_nCurrentLevel = newLevel;
		m_Levels[m_nCurrentLevel-1]->load(m_pSceneManager,m_pPhyxScene,m_pRenderSys);
		setUpPlayerController();
		SoundNodeManager::GetSingleton()->playAllSounds();
	}
}

void TestState::clean()
{
	if(m_nCurrentLevel != 0)
	{
		m_Levels[m_nCurrentLevel-1]->unLoad();
	}

	m_nCurrentLevel = 0;

	UserInterface::GetSingleton()->hideFlash("Pause_Menu");
	UserInterface::GetSingleton()->hideFlash("Blurry");
	UserInterface::GetSingleton()->destroyAllFlash();

	UserInterface::GetSingleton()->update();

	InputSystem::GetSingleton()->removeListener(*this);

	ActorManager::GetSingleton()->clearAll();

//	m_pSceneManager->destroyQuery(m_pRaySceneQuery);
/*	if(m_pCharController)
	{
		delete m_pCharController;
		m_pCharController =0;
	}*/  //-----------uncomment after test-----------

	if(m_pSound)
	{
		delete m_pSound;
		m_pSound=0;
	}

/*	if(m_pCamMan)
	{
		delete m_pCamMan;
	}
	m_pCamMan=0;
*///==============uncomment after test==================
	if(m_pPlayerCam)
	{
		m_pRenderWindow->removeViewport(0);

		m_pSceneManager->destroyAllCameras();
		m_pPlayerCam=0;
	}

	if(m_pPlayerController)
	{
		delete m_pPlayerController;
		m_pPlayerController = 0;
	}
/*	if(m_pTerrainGroup)
		OGRE_DELETE m_pTerrainGroup;
	if(m_pTerrainGlobals)
		OGRE_DELETE m_pTerrainGlobals;
*/
	PhysicsSystem::GetSingleton()->destroyRenderSystem(m_pRenderSys);
	m_pRenderSys=0;

	PhysicsSystem::GetSingleton()->destroyScene(m_pPhyxScene);
	m_pPhyxScene=0;

	delete m_pRenderSys;
	m_pRenderSys=0;

	PhysicsSystem::GetSingleton()->getWorld()->destroyScene(m_pPhyxScene);
	m_pPhyxScene=0;

	m_pSceneManager->clearScene();

	SoundNodeManager::GetSingleton()->emptyManager();

	SoundSystem::GetSingleton()->unloadAllSounds();
}

void TestState::catchEvent(const EventSystem::EventPtr& e, EventSystem::EventManager& mgr)
{
	const EventSystem::Event::TypeId strType = e->type();

	if(strType==EVENT_ID_INPUT_KEY_PRESSED)
	{
		const EventSystem::DataEvent<OIS::KeyEvent>* pDataEvent = static_cast<const EventSystem::DataEvent<OIS::KeyEvent>*>(e.get());
		handleKeyDown(pDataEvent->data());
	}else
	if(strType==EVENT_ID_INPUT_KEY_RELEASED)
	{
		const EventSystem::DataEvent<OIS::KeyEvent>* pDataEvent = static_cast<const EventSystem::DataEvent<OIS::KeyEvent>*>(e.get());
		handleKeyUp(pDataEvent->data());
	}else
	if(strType==EVENT_ID_INPUT_MOUSE_MOVED)
	{
		const EventSystem::DataEvent<OIS::MouseEvent>* pDataEvent = static_cast<const EventSystem::DataEvent<OIS::MouseEvent>*>(e.get());
		handleMouseMoved(pDataEvent->data());
	}else
	if(strType==EVENT_ID_INPUT_MOUSE_KEY_PRESSED)
	{
		const EventSystem::DataEvent<MouseInfo>* pDataEvent = static_cast<const EventSystem::DataEvent<MouseInfo>*>(e.get());
		handleMousePressed(pDataEvent->data());
	}else
	if(strType==EVENT_ID_INPUT_MOUSE_KEY_RELEASED)
	{
		const EventSystem::DataEvent<MouseInfo>* pDataEvent = static_cast<const EventSystem::DataEvent<MouseInfo>*>(e.get());
		handleMouseReleased(pDataEvent->data());
	}
}
void TestState::handleKeyDown(OIS::KeyEvent evt)
{
//	m_pCamMan->injectKeyDown(evt);
//	m_pCharController->injectKeyDown(evt);

	//----------Nik----------
	if(m_bPause == false)
		m_pPlayerController->injectKeyDown(evt);
}
void TestState::handleKeyUp(OIS::KeyEvent evt)
{
	if(evt.key==OIS::KC_ESCAPE)
	{
	//	m_currentState=GAME_STATE_BACK_TO_MENU;
		if (m_bPause == false)
		{
			m_bPause = true;
			PhysicsSystem::GetSingleton()->setPaused(true);
		}
		else
		{
			m_bPause = false;
			UserInterface::GetSingleton()->hideFlash("Pause_Menu");
			UserInterface::GetSingleton()->hideFlash("Blurry");
			PhysicsSystem::GetSingleton()->setPaused(false);
		}
	}

//	m_pCamMan->injectKeyUp(evt);
//	m_pCharController->injectKeyUp(evt);

	//----------Nik----------
	if(m_bPause == false)
		m_pPlayerController->injectKeyUp(evt);
}
void TestState::handleMouseMoved(OIS::MouseEvent evt)
{
//	m_pCamMan->injectMouseMove(evt);
//	m_pCharController->injectMouseMove(evt);

	//----------Nik----------
	if(m_bPause == false)
	{
		m_pPlayerController->injectMouseMove(evt);
	}
	else
	{
		UserInterface::GetSingleton()->mouseMoved(evt);
	}
}
void TestState::handleMousePressed(MouseInfo info)
{
//	m_pCamMan->injectMouseDown(*info.m_arg,info.m_id); ---------uncomment after test--------
	if (m_bPause == true)
		UserInterface::GetSingleton()->mousePressed(info);
}
void TestState::handleMouseReleased(MouseInfo info)
{
//	m_pCamMan->injectMouseUp(*info.m_arg,info.m_id); ---------uncomment after test--------
	if (m_bPause == true)
		UserInterface::GetSingleton()->mouseReleased(info);
}

void TestState::initBlendMaps(Ogre::Terrain* terrain)
{
	Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
    /*Ogre::Real minHeight0 = 70;
    Ogre::Real fadeDist0 = 40;
    Ogre::Real minHeight1 = 70;
    Ogre::Real fadeDist1 = 15;*/

	//My vars
	/*Ogre::Real minHeight0 = 1000.5f;
    Ogre::Real fadeDist0 = 2.0f;
    Ogre::Real minHeight1 = 600.5f;
    Ogre::Real fadeDist1 = 0.7f;*/

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
            //*pBlend0++ = val;

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

    // set up a colour map
    /*
    if (!terrain->getGlobalColourMapEnabled())
    {
        terrain->setGlobalColourMapEnabled(true);
        Image colourMap;
        colourMap.load("testcolourmap.jpg", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        terrain->getGlobalColourMap()->loadImage(colourMap);
    }
    */
}

void TestState::setupHeightFields()
{
	Ogre::TerrainGroup::TerrainIterator ti = m_pTerrainGroup->getTerrainIterator();
	while(ti.hasMoreElements())
	{
		Ogre::Terrain* t = ti.getNext()->instance;
		m_pRenderSys->createTerrain(t);
	}
}

void TestState::defineTerrain(long x,long y)
{
	Ogre::String filename = m_pTerrainGroup->generateFilename(x, y);
    if (Ogre::ResourceGroupManager::getSingleton().resourceExists(m_pTerrainGroup->getResourceGroup(), filename))
    {
        m_pTerrainGroup->defineTerrain(x, y);
    }
    else
    {
        Ogre::Image img;
        getTerrainImage(x % 2 != 0, y % 2 != 0, img);
        m_pTerrainGroup->defineTerrain(x, y, &img);
        mTerrainsImported = true;
    }
}
/*void TestState::initBlendMaps(Ogre::Terrain* terrain)
{
	Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
    Ogre::Real minHeight0 = 70;
    Ogre::Real fadeDist0 = 40;
    Ogre::Real minHeight1 = 70;
    Ogre::Real fadeDist1 = 15;
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
    blendMap0->update();
    blendMap1->update();
}*/
void TestState::configureTerrainDefaults(Ogre::Light* light)
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
	/*
	The default material generator takes two textures per layer:

			diffuse_specular - diffuse texture with a specular map in the alpha channel
			normal_height - normal map with a height map in the alpha channel
	*/
    /*defaultimp.layerList.resize(3);//set 3 texture layers
    defaultimp.layerList[0].worldSize = 100;//how big the texture is going to be, a lower number gives higher resolution
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    defaultimp.layerList[1].worldSize = 30;
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
    defaultimp.layerList[2].worldSize = 200;
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");*/
}

void TestState::getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
	img.load("HeightMap.jpg", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    if (flipX)
        img.flipAroundY();
    if (flipY)
        img.flipAroundX();
}

Hikari::FlashValue TestState::onExitClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
//	clean(); change the exit here
	m_currentState = GAME_STATE_BACK_TO_MENU;
	UserInterface::GetSingleton()->hideFlash("Pause_Menu");
	UserInterface::GetSingleton()->hideFlash("Blurry");
	return FLASH_VOID;
}

Hikari::FlashValue TestState::onResumeClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	m_bPause = false;
	PhysicsSystem::GetSingleton()->setPaused(false);
	UserInterface::GetSingleton()->hideFlash("Pause_Menu");
	UserInterface::GetSingleton()->hideFlash("Blurry");
	return FLASH_VOID;
}