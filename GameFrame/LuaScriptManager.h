#ifndef LUA_SCRIPT_MANAGER_H
#define LUA_SCRIPT_MANAGER_H

#include <lua.hpp>
#include <vector>
#include <string>

#include "luabind\luabind.hpp"
#include <Ogre.h>

#include "Simpleton.h"

//macro for setting a lua global var from c++
#define SET_LUA_GLOBAL(name,class) luabind::globals(LuaScriptManager::GetSingleton()->getLuaState())[name] = class;

//macro for the lua file path
#define LUA_SCRIPTS_PATH "../../media/lua scripts/"

class LuaScriptManager: public Simpleton<LuaScriptManager>
{
public:
	LuaScriptManager();
	~LuaScriptManager(void);

	void init(Ogre::String path)
	{
		m_strFileSystem = path;
	}

	lua_State* getLuaState(){return m_pLuaState;}

	/*
	*/
	template<typename T>
	void bindToLua(T &obj)
	{
		T._luaIntergrate();
	}

	void doFile(const char* fileName);

	luabind::object getLuaGlobal(const char* globalName);

private:
	lua_State *m_pLuaState;
	Ogre::String m_strFileSystem;

	friend class Simpleton< LuaScriptManager >;
};

#endif