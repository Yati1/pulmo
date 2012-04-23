#include "LuaScriptManager.h"
#include "assert.hpp"

LuaScriptManager* Simpleton<LuaScriptManager>::s_singleton=0;

LuaScriptManager::LuaScriptManager(void)
{
	m_pLuaState = lua_open();
	luaL_openlibs(m_pLuaState);
}

LuaScriptManager::~LuaScriptManager(void)
{
	lua_close(m_pLuaState);
	//according to docs calling lua_close is all you have to do
	/*if(m_pLuaState)
		delete m_pLuaState;
	*/
}

void LuaScriptManager::doFile(const char* fileName)
{
	BOOST_ASSERT_MSG(fileName!=0,"lua file name is null");

	Ogre::String str = Ogre::String(fileName);
	str = m_strFileSystem + str;

	if(luaL_dofile(m_pLuaState,str.c_str()))
	{
		Ogre::LogManager::getSingletonPtr()->logMessage(lua_tostring(m_pLuaState, -1));
	}
}

luabind::object LuaScriptManager::getLuaGlobal(const char* globalName)
{
	luabind::object g = luabind::globals(m_pLuaState);
	return g[globalName];
}