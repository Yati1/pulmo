#include "SoundSystem.h"

#include "assert.hpp"

SoundSystem* Simpleton<SoundSystem>::s_singleton=0;

SoundSystem::SoundSystem(void)
{
	m_pListener=0;
	//pint to log
	Ogre::LogManager::getSingletonPtr()->logMessage("**************************************");
	Ogre::LogManager::getSingletonPtr()->logMessage("*** INITIALIZING FMOD SOUND SYSTEM ***");
	Ogre::LogManager::getSingletonPtr()->logMessage("**************************************");
	/*
		Create a System object and initialize.
	*/
	m_result = FMOD::System_Create(&m_pSystem);
	ERRCHECK(m_result);

	m_result = m_pSystem->getVersion(&m_unVersion);
	ERRCHECK(m_result);

	if (m_unVersion < FMOD_VERSION)
	{
		static char buffer[256];
		sprintf_s(buffer, "Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", m_unVersion, FMOD_VERSION);
		Ogre::LogManager::getSingletonPtr()->logMessage(buffer);
	}
	//Ogre uses right handed coord system
	m_result = m_pSystem->init(32,FMOD_INIT_3D_RIGHTHANDED, 0);
	ERRCHECK(m_result);

	m_result = m_pSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
	ERRCHECK(m_result);

	m_result = m_pSystem->set3DSettings(0.3f,1.0f,1.0f);
	ERRCHECK(m_result);

	FMOD_REVERB_PROPERTIES prop1 = FMOD_PRESET_UNDERWATER;
	m_pSystem->setReverbAmbientProperties(&prop1);

	Ogre::LogManager::getSingletonPtr()->logMessage("****************************************");
	Ogre::LogManager::getSingletonPtr()->logMessage("*** INITIALIZED FMOD SOUND SYSTEM OK ***");
	Ogre::LogManager::getSingletonPtr()->logMessage("****************************************");
}

SoundSystem::~SoundSystem(void)
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** DESTROYING FMOD SOUND SYSTEM ***");

	//unload the sounds from the map
	if(!m_FileNameSoundMap.empty())
	{
		std::map<std::string, FMOD::Sound*>::iterator iter;

		for(iter=m_FileNameSoundMap.begin();iter!=m_FileNameSoundMap.end();iter++)
		{
			iter->second->release();
			iter->second=0;
		}
	}

	m_FileNameSoundMap.clear();

	/*
		Shut down
	*/

	m_result = m_pSystem->close();
	ERRCHECK(m_result);
	m_result = m_pSystem->release();
	ERRCHECK(m_result);
}

void SoundSystem::unloadAllSounds()
{
	//unload the sounds from the map
	if(!m_FileNameSoundMap.empty())
	{
		std::map<std::string, FMOD::Sound*>::iterator iter;

		for(iter=m_FileNameSoundMap.begin();iter!=m_FileNameSoundMap.end();iter++)
		{
			iter->second->release();
			iter->second=0;
		}
	}

	m_FileNameSoundMap.clear();
}

void SoundSystem::load3dSound(const char* filePath,bool bStreamed)
{
	Ogre::String path;
	if(!m_strFileSystem.empty())
	{
		path=m_strFileSystem+filePath;

		filePath=path.c_str();
	}

	if(m_FileNameSoundMap[filePath])
		return;

	FMOD::Sound      *sound;

	//if not streamed then load normally or load the streamed version
	if(!bStreamed)
	{
		m_result = m_pSystem->createSound(filePath, FMOD_HARDWARE | FMOD_3D, 0, &sound);
		ERRCHECK(m_result);
	}else
	{
		m_result = m_pSystem->createStream(filePath, FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_3D, 0, &sound);
		ERRCHECK(m_result);
	}

	//if the loading was successful put into map
	if(m_result == FMOD_OK)
	{
		m_FileNameSoundMap[filePath]=sound;
	}
}

void SoundSystem::loadSound(const char* filePath,bool bStreamed)
{
	Ogre::String path;
	if(!m_strFileSystem.empty())
	{
		path=m_strFileSystem+filePath;

		filePath=path.c_str();
	}
	if(m_FileNameSoundMap[filePath])
		return;
	FMOD::Sound      *sound;

	//if not streamed then load normally or load the streamed version
	if(!bStreamed)
	{
		m_result = m_pSystem->createSound(filePath, FMOD_HARDWARE, 0, &sound);
		ERRCHECK(m_result);
	}else
	{
		m_result = m_pSystem->createStream(filePath, FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);
		ERRCHECK(m_result);
	}

	//if the loading was successful put into map
	if(m_result == FMOD_OK)
	{
		m_FileNameSoundMap[filePath]=sound;
	}
}

void SoundSystem::playSound(const char* filePath,bool bLooping,bool bPaused)
{
	FMOD::Sound*	sound;
	Ogre::String path;
	if(!m_strFileSystem.empty())
	{
		path=m_strFileSystem+filePath;

		filePath=path.c_str();
	}

	sound = getSound(filePath,false);
	//HACK: should check if sound is null
	BOOST_ASSERT_MSG(sound!=0,"sound file not found! Check if the sound's file-path is correct.");

	if(sound)
	{
		//HACK: There are other looping settings other than FMOD_LOOP_NORMAL & FMOD_LOOP_OFF
		if(bLooping)
			m_result = sound->setMode(FMOD_LOOP_NORMAL);
		else
			m_result = sound->setMode(FMOD_LOOP_OFF);

		ERRCHECK(m_result);
	}
	if(bLooping)
		m_result = m_pSystem->playSound(FMOD_CHANNEL_FREE, sound, bPaused, &m_pChannel);
	else
		m_result = m_pSystem->playSound(FMOD_CHANNEL_FREE, sound, bPaused, 0);
	ERRCHECK(m_result);
}

FMOD::Channel* SoundSystem::play3DSound(const char* filePath,FMOD_VECTOR pos,FMOD_VECTOR velocity,float size,bool bLooping,bool bPaused)
{
	FMOD::Channel* channel=0;
	FMOD::Sound*	sound;
	Ogre::String path;
	if(!m_strFileSystem.empty())
	{
		path=m_strFileSystem+filePath;

		filePath=path.c_str();
	}

	sound = getSound(filePath,false);
	//HACK: should check if sound is null
	BOOST_ASSERT_MSG(sound!=0,"sound file not found! Check if the sound's file-path is correct.");

	if(sound)
	{
		if(bLooping)
			m_result = sound->setMode(FMOD_LOOP_NORMAL);
		else
			m_result = sound->setMode(FMOD_LOOP_OFF);

		ERRCHECK(m_result);

		m_result = m_pSystem->playSound(FMOD_CHANNEL_FREE, sound, bPaused, &channel);
		//result = system->playSound(FMOD_CHANNEL_FREE, sound2, true, &channel1);
		ERRCHECK(m_result);
		m_result = channel->set3DAttributes(&pos, &velocity);
		ERRCHECK(m_result);
		m_result = channel->set3DMinMaxDistance(size,1000.f);
		ERRCHECK(m_result);
	}
	return channel;
}

FMOD::Sound* SoundSystem::getSound(const char* filePath,bool bAppendFileSys)
{
	Ogre::String path;
	if(bAppendFileSys)
	{
		if(!m_strFileSystem.empty())
		{
			path=m_strFileSystem+filePath;

			filePath=path.c_str();
		}
	}

	std::string strFileName=filePath;
	std::map<std::string, FMOD::Sound*>::iterator iter;

	iter = m_FileNameSoundMap.find(strFileName);

	if(iter != m_FileNameSoundMap.end())
	{
		return iter->second;
	}

	return 0;
}

void SoundSystem::stopSound(const char* filePath,bool bAppendFileSys)
{
	Ogre::String path;
	if(bAppendFileSys)
	{
		if(!m_strFileSystem.empty())
		{
			path=m_strFileSystem+filePath;

			filePath=path.c_str();
		}
	}

	FMOD::Sound* sound=getSound(filePath);
	if(sound)
	{
		FMOD::SoundGroup* sGroup;
		sound->getSoundGroup(&sGroup);
		sGroup->stop();
	}
}

void SoundSystem::update(float fElapsedTime)
{
	//update the listner
	if(m_pListener)
	{
		//setListener(m_pListener->getPos(),m_pListener->getForward(),m_pListener->getUp(),m_pListener->getVel());
		//m_pListener->update(fElapsedTime);

		m_result = m_pSystem->set3DListenerAttributes(0, &m_pListener->getPos(), &m_pListener->getVel(), &m_pListener->getForward(), &m_pListener->getUp());
        ERRCHECK(m_result);
	}

	m_pSystem->update();

	//HACK: some checks for debugging
	{
		unsigned int ms = 0;
		unsigned int lenms = 0;
		bool         playing = 0;
		bool         paused = 0;
		int          channelsplaying = 0;

		if (m_pChannel)
		{
			FMOD::Sound *currentsound = 0;

			m_result = m_pChannel->isPlaying(&playing);
			if ((m_result != FMOD_OK) && (m_result != FMOD_ERR_INVALID_HANDLE) && (m_result != FMOD_ERR_CHANNEL_STOLEN))
			{
				ERRCHECK(m_result);
			}

			m_result = m_pChannel->getPaused(&paused);
			if ((m_result != FMOD_OK) && (m_result != FMOD_ERR_INVALID_HANDLE) && (m_result != FMOD_ERR_CHANNEL_STOLEN))
			{
				ERRCHECK(m_result);
			}

			m_result = m_pChannel->getPosition(&ms, FMOD_TIMEUNIT_MS);
			if ((m_result != FMOD_OK) && (m_result != FMOD_ERR_INVALID_HANDLE) && (m_result != FMOD_ERR_CHANNEL_STOLEN))
			{
				ERRCHECK(m_result);
			}

			m_pChannel->getCurrentSound(&currentsound);
			if (currentsound)
			{
				m_result = currentsound->getLength(&lenms, FMOD_TIMEUNIT_MS);
				if ((m_result != FMOD_OK) && (m_result != FMOD_ERR_INVALID_HANDLE) && (m_result != FMOD_ERR_CHANNEL_STOLEN))
				{
					ERRCHECK(m_result);
				}
			}
		}
	}
}

SoundListener* SoundSystem::createSoundListener(Ogre::Vector3 forward,Ogre::Vector3 up,Ogre::Vector3 position,Ogre::Vector3 velocity)
{
	FMOD_VECTOR f,u,p,v;

	f.x=forward.x;
	f.y=forward.y;
	f.z=forward.z;

	u.x=up.x;
	u.y=up.y;
	u.z=up.z;

	p.x=position.x;
	p.y=position.y;
	p.z=position.z;

	v.x =velocity.x;
	v.y =velocity.y;
	v.z =velocity.z;

	m_pListener = new SoundListener(p,f,u,v);

	return m_pListener;
}

void SoundSystem::updateSoundListener(Ogre::Vector3 forward,Ogre::Vector3 up,Ogre::Vector3 position,Ogre::Vector3 velocity)
{
	FMOD_VECTOR f,u,p,v;

	f.x=forward.x;
	f.y=forward.y;
	f.z=forward.z;

	u.x=up.x;
	u.y=up.y;
	u.z=up.z;

	p.x=position.x;
	p.y=position.y;
	p.z=position.z;

	v.x =velocity.x;
	v.y =velocity.y;
	v.z =velocity.z;

	m_pListener->setForward(f);
	m_pListener->setPos(p);
	m_pListener->setUp(u);
	m_pListener->setVel(v);
}

void SoundSystem::setListener(FMOD_VECTOR pos,FMOD_VECTOR forward,FMOD_VECTOR up,FMOD_VECTOR vel)
{
	m_pListener=new SoundListener(pos,forward,up,vel);
}
void SoundSystem::setListener(SoundListener* listener)
{
	m_pListener=listener;
}

void SoundSystem::setVolume(float volume)
{
	FMOD::ChannelGroup* masterChannel;
	m_result=m_pSystem->getMasterChannelGroup(&masterChannel);
	ERRCHECK(m_result);

	m_result=masterChannel->setVolume(volume);
	ERRCHECK(m_result);
}

void SoundSystem::togglePauseAll()
{
	FMOD::ChannelGroup* masterChannel;
	m_result=m_pSystem->getMasterChannelGroup(&masterChannel);
	ERRCHECK(m_result);

	bool paused;
	masterChannel->getPaused(&paused);

	m_result=masterChannel->setPaused(!paused);
	ERRCHECK(m_result);
}