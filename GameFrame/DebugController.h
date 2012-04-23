#ifndef DEBUG_CONTROLLER_H
#define DEBUG_CONTROLLER_H

#include "Simpleton.h"

#include <OgreCompositorManager.h>
#include <OgreCompositorChain.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>

#include <SdkTrays.h>

class DebugController : public Simpleton<DebugController>
{
public:
	DebugController(void);
	~DebugController(void);

	void initDebugController(Ogre::RenderWindow* pRenderWindow,Ogre::SceneManager* pSceneManager);

private:

	bool m_bCompositor;			//is the compositors enabled
	bool m_bDebugShadows;		//is debug shadows on


	Ogre::RenderWindow* m_pRenderWindow;
	Ogre::SceneManager* m_pSceneMgr;
	OgreBites::SdkTrayManager* mTrayMgr;

	friend class Simpleton< DebugController >;


};

#endif

