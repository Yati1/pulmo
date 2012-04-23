#ifndef LUACONSOLE_H
#define LUACONSOLE_H
 
//#include <Ogre.h>
#include <OgreRoot.h>
#include <OgreFrameListener.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OIS.h>
#include <list>
#include <string>
#include "EditString.h"
#include "LuaInterpreter.h"
 
class LuaConsole: public Ogre::Singleton<LuaConsole>, Ogre::FrameListener, Ogre::LogListener
{
public:
    LuaConsole();
    virtual ~LuaConsole();
 
    void    init(lua_State *L );
    void    shutdown();
    void    setVisible(bool fVisible);
    bool    isVisible(){ return visible; }
    void    print( std::string text );
    bool    injectKeyPress( const OIS::KeyEvent &evt );
 
    // Frame listener
    bool    frameStarted(const Ogre::FrameEvent &evt);
    bool    frameEnded(const Ogre::FrameEvent &evt);
 
    // Log Listener
    void    messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName );
 
protected:
    bool                    visible;
    bool                    textChanged;
    float                   height;
    int                     startLine;
    bool                    cursorBlink;
    float                   cursorBlinkTime;
    bool                    fInitialised;
 
    Ogre::Overlay           *overlay;
    Ogre::OverlayContainer  *panel;
    Ogre::OverlayElement    *textbox;
 
    EditString              editLine;
    LuaInterpreter          *interpreter;
 
    std::list<std::string>  lines;
    std::list<std::string>  history;
 
    std::list<std::string>::iterator    historyLine;
 
    void    addToHistory( const std::string& cmd );
};
 
#endif // LUACONSOLE_H