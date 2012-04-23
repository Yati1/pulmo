#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H

#include <Ogre.h>
#include <OgreLogManager.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <map>
#include <string>

#include "Simpleton.h"

//TODO: Implement multi-listener compatibility
/*
	A little helper class to represent a sound listener positioned in 3d space
*/
class SoundListener
{
public:
	SoundListener(FMOD_VECTOR pos,FMOD_VECTOR forward,FMOD_VECTOR up,FMOD_VECTOR vel)
	{
		m_vPos=pos;
		m_vForward=forward;
		m_vUp=up;
		m_vVel = vel;

		m_vLastPos=pos;
	}
	~SoundListener(){};

	void update(float fElapsedTime)
	{
		m_vVel.x = (m_vPos.x - m_vLastPos.x) * (1000 / fElapsedTime);
        m_vVel.y = (m_vPos.y - m_vLastPos.y) * (1000 / fElapsedTime);
        m_vVel.z = (m_vPos.z - m_vLastPos.z) * (1000 / fElapsedTime);

		m_vLastPos=m_vPos;
	}

	FMOD_VECTOR getPos()const{return m_vPos;}
	FMOD_VECTOR getVel()const{return m_vVel;}
	FMOD_VECTOR getForward()const{return m_vForward;}
	FMOD_VECTOR getUp()const{return m_vUp;}

	void setPos(FMOD_VECTOR pos){m_vPos=pos;}
	void setVel(FMOD_VECTOR vel){m_vVel=vel;}
	void setForward(FMOD_VECTOR forward){m_vForward=forward;}
	void setUp(FMOD_VECTOR up){m_vUp=up;}

private:
	FMOD_VECTOR m_vPos;
	FMOD_VECTOR m_vVel;
	FMOD_VECTOR m_vForward;
	FMOD_VECTOR m_vUp;

	FMOD_VECTOR m_vLastPos;
};

class SoundSystem: public Simpleton<SoundSystem>
{
public:
	SoundSystem(void);
	~SoundSystem(void);

	void setFileSystem(Ogre::String str){m_strFileSystem=str;}
	Ogre::String getFileSystem(){return m_strFileSystem;}

	/*
	Unloads all the sounds stored in the sound system
	*/
	void unloadAllSounds();
	/*
	Sets the volume of the sound system

	volume must be 0.0 - 1.0
	*/
	void setVolume(float volume);
	/*
	Loads a sound MUST DO THIS FRIST
	If a large file like a music then set bStreamed to frue to stream the audio
	*/
	void loadSound(const char* filePath,bool bStreamed=false);

	void playSound(const char* filePath,bool bLooping=false,bool bPaused=false);

	void load3dSound(const char* filePath,bool bStreamed=false);

	FMOD::Channel* play3DSound(const char* filePath,FMOD_VECTOR pos,FMOD_VECTOR velocity,float size,bool bLooping=false,bool bPaused=false);

	FMOD::Sound* getSound(const char* filePath,bool bAppendFileSys=true);

	/*
	Update FMOD sound system
	*/
	void update(float fElapsedTime);

	/*
	If a error occurs outputs an log entry to the ogre log
	*/
	static void ERRCHECK(FMOD_RESULT result)
	{
		if (result != FMOD_OK)
		{
			//HACK: Not sure if this does exactly what I want it to do.
			//It should print the contents of the buffer to Ogre's log
			static char buffer[256];
			sprintf_s(buffer, "FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
			Ogre::LogManager::getSingletonPtr()->logMessage(buffer);
		}
	}
	/**
		Creates and sets the sound listener from the passed vectors
	**/
	SoundListener* createSoundListener(Ogre::Vector3 forward,Ogre::Vector3 up,Ogre::Vector3 position,Ogre::Vector3 velocity);
	void updateSoundListener(Ogre::Vector3 forward,Ogre::Vector3 up,Ogre::Vector3 position,Ogre::Vector3 velocity);

	void setListener(FMOD_VECTOR pos,FMOD_VECTOR forward,FMOD_VECTOR up,FMOD_VECTOR vel);
	void setListener(SoundListener* listener);

	/*
	Stop the sound
	*/
	void stopSound(const char* filePath,bool bAppendFileSys=false);

	void togglePauseAll();
private:
	FMOD::System*	m_pSystem;
	std::map<std::string, FMOD::Sound*> m_FileNameSoundMap;	//storing the filename as the key and the sound object as data
	FMOD::Channel*	m_pChannel;
	FMOD_RESULT     m_result;
	unsigned int    m_unVersion;

	Ogre::String m_strFileSystem;

	friend class Simpleton< SoundSystem >;

	//bool m_bActiveListener;					//whether a listener exists
	//TODO: Make multi-listener
	SoundListener* m_pListener;				//the listener
};

#endif