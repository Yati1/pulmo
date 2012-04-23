#ifndef CUSTOM_TRAY_MANAGER_H
#define CUSTOM_TRAY_MANAGER_H

#include <Ogre.h>
#include <SdkTrays.h>

#include <OIS\OISMouse.h>
/********************************************************
This class derives from SdktrayManager and allows us to customize the trays (ogre's UI system)
********************************************************/
class CustomTrayManager: public OgreBites::SdkTrayManager
{
public:
	CustomTrayManager(const Ogre::String& name, Ogre::RenderWindow* window, OIS::Mouse* mouse, SdkTrayListener* listener = 0);
	virtual ~CustomTrayManager(void);
};
#endif
