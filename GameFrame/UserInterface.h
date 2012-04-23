#ifndef UI_H
#define UI_H

#include <Ogre.h>

#include "Hikari.h"
#include "InputSystem.h"
#include "Simpleton.h"
#include "CustomTrayManager.h"
#include <string>

class UserInterface : public Simpleton<UserInterface>
{
public:
	UserInterface();
	~UserInterface(void);

	void loadFlash(Ogre::String strFileName, Ogre::Viewport* viewport, Ogre::String strOverlayName, int nWidth, int nHeight, Hikari::Position pos, float fOpacity);
	void unloadFlash(const Ogre::String& strOverlayName);

	void displayFlash(Ogre::String strOverlayName);
	void hideFlash(Ogre::String strOverlayName);

	void bindFlash(const Ogre::String& strOverlayName, const Ogre::String& strCallbackName, const Hikari::FlashDelegate& callback);
	void callFlashFunction(const Ogre::String& strOverlayName, const Ogre::String& strCallbackName, Hikari::Arguments args);

	void setTray(CustomTrayManager* pTray, Ogre::RenderWindow* rw);

	Hikari::FlashControl* getFlash(const Ogre::String& strOverlayName) { return m_pHikariMgr->getFlashControl(strOverlayName); };

	void update();

	void destroyAllFlash();

	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(MouseInfo info);
	bool mouseReleased(MouseInfo info);

	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased( const OIS::KeyEvent &arg );

private:
	Hikari::HikariManager* m_pHikariMgr;

//	int m_nWidth;
//	int m_nHeight;

	//Make two functions to communicate with flash
//	Hikari::FlashValue onExitClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);
//	Hikari::FlashValue onPlayClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);

	friend class Simpleton<UserInterface>;

	CustomTrayManager* m_pTrayMgr;
};

#endif