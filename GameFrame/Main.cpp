#include "GameApplication.h"
#include "EngineLuaBinding.h"

#if OGRE_PLATFORM== OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#ifndef __cplusplus
extern "C"{
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
	int main(int argc,char *argv[])
#endif
	{
		//create the main object
		GameApplication app;
		try{
			BindEngineToLua();

			SET_LUA_GLOBAL("GameApp",&app);
			SET_LUA_GLOBAL("ParticleManager",ParticleEffectManager::GetSingleton());
			app.run();
		}catch(Ogre::Exception& e){
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr<<"An exception occured: "<<
				e.getFullDescription().c_str() << std::endl;
#endif
		}

		return 0;
	}
#ifndef __cplusplus
}
#endif