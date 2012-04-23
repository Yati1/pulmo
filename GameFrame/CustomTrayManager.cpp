#include "CustomTrayManager.h"


CustomTrayManager::CustomTrayManager(const Ogre::String& name, Ogre::RenderWindow* window, OIS::Mouse* mouse, SdkTrayListener* listener) : OgreBites::SdkTrayManager(name,window,mouse,listener)
{
	//override the default material with the bright white material
	mDialogShade->setMaterialName("Astroids/LoadingBack");
}


CustomTrayManager::~CustomTrayManager(void)
{
}
