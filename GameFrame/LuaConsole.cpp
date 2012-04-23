#include "LuaConsole.h"

#define CONSOLE_LINE_LENGTH 85
#define CONSOLE_LINE_COUNT 15
#define CONSOLE_MAX_LINES 32000
#define CONSOLE_MAX_HISTORY 128
#define CONSOLE_TAB_STOP 8

using namespace Ogre;
using namespace std;

template<> LuaConsole *Singleton<LuaConsole>::ms_Singleton=0;

LuaConsole::LuaConsole() : fInitialised(false)
{
}

LuaConsole::~LuaConsole()
{
    if( fInitialised )
        shutdown();
}

void LuaConsole::init(lua_State *L)
{
    if( fInitialised )
        shutdown();

    OverlayManager &overlayManager = OverlayManager::getSingleton();

    Root *root = Root::getSingletonPtr();

    //scene=root->getSceneManagerIterator().getNext();
    root->addFrameListener(this);

    height = 1;
    startLine = 0;
    cursorBlinkTime = 0;
    cursorBlink = false;
    visible = false;

    interpreter = new LuaInterpreter( L );
    print( interpreter->getOutput() );
    interpreter->clearOutput();

    textbox = overlayManager.createOverlayElement("TextArea","ConsoleText");
    textbox->setMetricsMode(GMM_RELATIVE);
    textbox->setPosition(0,0);
    textbox->setParameter("font_name","Console");
    textbox->setParameter("colour_top","1 1 1");
    textbox->setParameter("colour_bottom","1 1 1");
    textbox->setParameter("char_height","0.03");

    panel = static_cast<OverlayContainer*>(overlayManager.createOverlayElement("Panel", "ConsolePanel"));
    panel->setMetricsMode(Ogre::GMM_RELATIVE);
    panel->setPosition(0, 0);
    panel->setDimensions(1, 0);
    panel->setMaterialName("console/background");

    panel->addChild(textbox);

    overlay = overlayManager.create("Console");
    overlay->add2D(panel);
    overlay->show();
    LogManager::getSingleton().getDefaultLog()->addListener(this);

    fInitialised = true;
}

void LuaConsole::shutdown()

{
    if( fInitialised )
    {
        delete interpreter;

        OverlayManager::getSingleton().destroyOverlayElement( textbox );
        OverlayManager::getSingleton().destroyOverlayElement( panel );
        OverlayManager::getSingleton().destroy( overlay );

        Root::getSingleton().removeFrameListener( this );
        LogManager::getSingleton().getDefaultLog()->removeListener(this);
    }
    fInitialised = false;
}

void LuaConsole::setVisible(bool fVisible)
{
    visible = fVisible;
}

void LuaConsole::messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName )
{
    print( message );
}

bool LuaConsole::frameStarted(const Ogre::FrameEvent &evt)
{
    if(visible)
    {
        cursorBlinkTime += evt.timeSinceLastFrame;

        if( cursorBlinkTime > 0.5f )
        {
            cursorBlinkTime -= 0.5f;
            cursorBlink = ! cursorBlink;
            textChanged = true;
        }
    }

    if(visible && height < 1)
    {
        height += evt.timeSinceLastFrame * 10;
        textbox->show();

        if(height >= 1)
        {
            height = 1;
        }
    }
    else if( !visible && height > 0)
    {
        height -= evt.timeSinceLastFrame * 10;
        if(height <= 0)
        {
            height = 0;
            textbox->hide();
        }
    }

    textbox->setPosition(0, (height - 1) * 0.5);
    panel->setDimensions( 1, height * 0.5 );

    if(textChanged)
    {
        String text;
        std::list<string>::iterator i,start,end;

        //make sure is in range
        //NOTE: the code elsewhere relies on startLine's signedness.
        //I.e. the ability to go below zero and not wrap around to a high number.
        if(startLine < 0 )
            startLine = 0;
        if((unsigned)startLine > lines.size())
            startLine = lines.size();

        start=lines.begin();

        for(int c = 0; c < startLine; c++)
            start++;

        end = start;

        for(int c = 0; c < CONSOLE_LINE_COUNT; c++)
        {
            if(end == lines.end())
                break;
            end++;
        }

        for(i = start; i != end; i++)
            text += (*i) + "\n";

        //add the edit line with cursor
        string editLineText( editLine.getText() + " " );
        if( cursorBlink )
            editLineText[editLine.getPosition()] = '_';

        text += interpreter->getPrompt() + editLineText;

        textbox->setCaption(text);

        textChanged = false;
    }

	if(!interpreter->getOutput().empty())
	{
		print(interpreter->getOutput());
		interpreter->clearOutput();
	}

    return true;
}

bool LuaConsole::frameEnded(const Ogre::FrameEvent &evt)
{
    return true;
}

void LuaConsole::print( std::string text )
{
    string line;
    string::iterator pos;
    int column;

    pos = text.begin();
    column = 1;

    while( pos != text.end() )
    {
        if( *pos == '\n' || column > CONSOLE_LINE_LENGTH )
        {
            lines.push_back( line );
            line.clear();
            if( *pos != '\n' )
              --pos;  // We want to keep this character for the next line.

            column = 0;
        }
        else if (*pos =='\t')
        {
           // Push at least 1 space
           line.push_back (' ');
           column++;

           // fill until next multiple of CONSOLE_TAB_STOP
           while ((column % CONSOLE_TAB_STOP )!=0)
           {
              line.push_back (' ');
              column++;
           }
        }
        else
        {
            line.push_back( *pos );
            column++;
        }

        pos++;
    }
    if( line.length() )
    {
        if( lines.size() > CONSOLE_MAX_LINES-1 )
            lines.pop_front();

        lines.push_back( line );
    }

    // Make sure last text printed is in view.
    if( lines.size() > CONSOLE_LINE_COUNT )
        startLine = lines.size() - CONSOLE_LINE_COUNT;

    textChanged = true;

    return;
}

void LuaConsole::addToHistory( const string& cmd )
{
    history.remove( cmd );
    history.push_back( cmd );
    if( history.size() > CONSOLE_MAX_HISTORY )
        history.pop_front();
    historyLine = history.end();
}

bool LuaConsole::injectKeyPress( const OIS::KeyEvent &evt )
{
    switch( evt.key )
    {
        case OIS::KC_RETURN:
            print( interpreter->getPrompt() + editLine.getText() );
            interpreter->insertLine( editLine.getText() );
            addToHistory( editLine.getText() );
            print( interpreter->getOutput() );
            interpreter->clearOutput();
            editLine.clear();
            break;

        case OIS::KC_PGUP:
            startLine -= CONSOLE_LINE_COUNT;
            textChanged = true;
            break;

        case OIS::KC_PGDOWN:
            startLine += CONSOLE_LINE_COUNT;
            textChanged = true;
            break;

        case OIS::KC_UP:
            if( !history.empty() )
            {
              if( historyLine == history.begin() )
                  historyLine = history.end();
              historyLine--;
              editLine.setText( *historyLine );
              textChanged = true;
            }
            break;

        case OIS::KC_DOWN:
            if( !history.empty() )
            {
              if( historyLine != history.end() )
                  historyLine++;
              if( historyLine == history.end() )
                  historyLine = history.begin();
              editLine.setText( *historyLine );
              textChanged = true;
            }
            break;

        default:
            textChanged = editLine.injectKeyPress( evt );
            break;
    }

    return true;
}