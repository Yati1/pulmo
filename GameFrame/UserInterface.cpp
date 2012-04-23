#include "UserInterface.h"
#include <assert.h>

UserInterface* Simpleton<UserInterface>::s_singleton=0;

UserInterface::UserInterface()
{
//	m_strFilePath = "../media";
//	m_bGUIShow = true;
	m_pHikariMgr = new Hikari::HikariManager("../../media/flash");
}

UserInterface::~UserInterface(void)
{
	if(m_pHikariMgr)
		delete m_pHikariMgr;
}

void UserInterface::destroyAllFlash()
{
	m_pHikariMgr->destroyAllControls();
}

/*void UserInterface::init(CustomTrayManager* pTrayMgr, Ogre::RenderWindow* rw)
{
	m_pTrayMgr = pTrayMgr;


  -----Need for reference---------
	m_nWidth = rw->getWidth();
	m_nHeight = rw->getHeight();

	const OIS::MouseState &ms = InputSystem::GetSingleton()->getMouseDevice()->getMouseState();

    ms.width = m_nWidth;
    ms.height = m_nHeight;
}*/

//The method below loadFlash automatically and initialize it. UI are hidden upon initialization

void UserInterface::loadFlash(Ogre::String strFileName, Ogre::Viewport* viewport, Ogre::String strOverlayName, int nWidth, int nHeight, Hikari::Position pos, float fOpacity)
{
    m_pHikariMgr->createFlashOverlay(strOverlayName, viewport, nWidth, nHeight, pos);

	m_pHikariMgr->getFlashControl(strOverlayName)->hide();
	m_pHikariMgr->getFlashControl(strOverlayName)->load(strFileName);
	m_pHikariMgr->getFlashControl(strOverlayName)->setTransparent(true, true);
	m_pHikariMgr->getFlashControl(strOverlayName)->setOpacity(fOpacity);


//	m_pUserInterface->bind("exitClick", Hikari::FlashDelegate(this, &UserInterface::onExitClick));
//	m_pUserInterface->bind("startClick", Hikari::FlashDelegate(this, &UserInterface::onPlayClick));

//	m_pTrayMgr->showCursor();
}

void UserInterface::displayFlash(Ogre::String strOverlayName)
{
	assert(strOverlayName.length() > 0);

	m_pTrayMgr->showCursor();

	m_pHikariMgr->getFlashControl(strOverlayName)->show();
	m_pHikariMgr->getFlashControl(strOverlayName)->play();
	m_pHikariMgr->getFlashControl(strOverlayName)->focus(); //this function will make the flash on top
}

void UserInterface::hideFlash(Ogre::String strOverlayName)
{
	assert(strOverlayName.length() > 0);

	m_pHikariMgr->getFlashControl(strOverlayName)->hide();
	m_pHikariMgr->getFlashControl(strOverlayName)->stop();

	m_pHikariMgr->defocusAll();

	m_pTrayMgr->hideCursor();
}

void UserInterface::bindFlash(const Ogre::String& strOverlayName, const Ogre::String& strCallbackName, const Hikari::FlashDelegate& callback)
{
	assert(strOverlayName.length() > 0);
	assert(strCallbackName.length() > 0);

	m_pHikariMgr->getFlashControl(strOverlayName)->bind(strCallbackName, callback);
}

void UserInterface::callFlashFunction(const Ogre::String& strOverlayName, const Ogre::String& strCallbackName, Hikari::Arguments args)
{
    assert(strOverlayName.length() > 0);
	assert(strCallbackName.length() > 0);

	m_pHikariMgr->getFlashControl(strOverlayName)->callFunction(strCallbackName, args);
}

void UserInterface::unloadFlash(const Ogre::String& strOverlayName)
{
	//mark specified overlay to be destroyed
}

/* -------Need this for reference---------------
Hikari::FlashValue UserInterface::onExitClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
//	m_bGUIShow = false;
	m_pUserInterface->hide();
	m_pUserInterface->rewind();
	m_pUserInterface->stop();
	m_pTrayMgr->hideCursor();
//	BaseApplication::setStatus(true);

	return FLASH_VOID;
}

Hikari::FlashValue UserInterface::onPlayClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
//	m_bGUIShow = false;
	m_pUserInterface->hide();
	m_pUserInterface->rewind();
	m_pUserInterface->stop();
	m_pTrayMgr->hideCursor();

	return FLASH_VOID;
}*/

void UserInterface::update()
{
	m_pHikariMgr->update();
}

bool UserInterface::mouseMoved(const OIS::MouseEvent &arg)
{
//	if(m_bGUIShow)
		m_pTrayMgr->injectMouseMove(arg);
	return m_pHikariMgr->injectMouseMove(arg.state.X.abs, arg.state.Y.abs) || m_pHikariMgr->injectMouseWheel(arg.state.Z.rel);
}

bool UserInterface::mousePressed(MouseInfo info)
{
//	if (m_bGUIShow)
		m_pTrayMgr->injectMouseDown(*info.m_arg,info.m_id);
	return m_pHikariMgr->injectMouseDown(info.m_id);
}

bool UserInterface::mouseReleased(MouseInfo info)
{
//	if (m_bGUIShow)
		m_pTrayMgr->injectMouseUp(*info.m_arg,info.m_id);
	return m_pHikariMgr->injectMouseUp(info.m_id);
}

bool UserInterface::keyPressed( const OIS::KeyEvent &arg )
{
	if(m_pHikariMgr->isAnyFocused())
		return true;

	return false;
}

bool UserInterface::keyReleased( const OIS::KeyEvent &arg )
{
	/*
	switch(arg.key)
	{
	case OIS::KC_ESCAPE:
	{
		if(m_bGUIShow)
		{
			m_pTrayMgr->hideCursor();
			m_bGUIShow=false;
			m_pUserInterface->hide();
			m_pUserInterface->rewind();
			m_pUserInterface->stop();
			return false;
		}
		else
		{
			m_pTrayMgr->showCursor();
			m_bGUIShow = true;
			m_pUserInterface->show();
			m_pUserInterface->focus();
			m_pUserInterface->play();
		}
		break;
	}
	}
	*/
	return true;
}

void UserInterface::setTray(CustomTrayManager* pTray, Ogre::RenderWindow* rw)
{
	m_pTrayMgr = pTray;


	
	const OIS::MouseState &ms = InputSystem::GetSingleton()->getMouseDevice()->getMouseState();

	ms.width = rw->getWidth();
    ms.height = rw->getHeight();
}