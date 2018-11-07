#include "stdafx.h"
#include "SoundManager.h"
#include "AudioObject.h"

#include "../OpenALSoft/include/AL/al.h"
#include "../OpenALSoft/include/AL/alc.h"

SoundManager* SoundManager::m_pInstance = nullptr;

SoundManager::SoundManager()
{
	m_pInstance = this;

	m_device = alcOpenDevice(NULL);
	if (!m_device)
		return;

	m_context = alcCreateContext((ALCdevice*)m_device, NULL);
	if (!alcMakeContextCurrent((ALCcontext*)m_context))
		return;

	//listener properties
	ALfloat listenerOri[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
	alListener3f(AL_POSITION, 0, 0, 0);
	alListener3f(AL_VELOCITY, 0, 0, 0);
	alListenerfv(AL_ORIENTATION, listenerOri);

	//create source pool
	alGenSources(NUM_MAX_SOURCES, m_soundSources);
	for (size_t i = 0; i < NUM_MAX_SOURCES; i++)
		m_freeSoundSources.push_back(m_soundSources[i]);

	m_bInitialized = true;
}


SoundManager::~SoundManager()
{
	if (!m_bInitialized)
		return;

	//make sure to stop all sources before freeing them
	int source_state;
	for (size_t i = 0; i < NUM_MAX_SOURCES; i++)
	{
		alGetSourcei(m_soundSources[i], AL_SOURCE_STATE, &source_state);

		if (source_state == AL_PLAYING)
			alSourceStop(m_soundSources[i]);

		alDeleteSources(1, &m_soundSources[i]);
	}

	m_device = alcGetContextsDevice((ALCcontext*)m_context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext((ALCcontext*)m_context);
	alcCloseDevice((ALCdevice*)m_device);
}

AudioObject* SoundManager::load(string filename)
{
	if (!m_bInitialized)
		return nullptr;

	AudioObject* pAudioObj = getAudioObject(filename);

	if (pAudioObj != nullptr)
		return pAudioObj;

	AudioObject* newAudioObj= new AudioObject(filename);

	m_audioObjects[filename] = newAudioObj;

	return m_audioObjects[filename];
}

AudioObject* SoundManager::getAudioObject(string filename)
{
	if (!m_bInitialized)
		return nullptr;

	auto it = m_audioObjects.find(filename);
	if (it != m_audioObjects.end())
		return it->second;

	return nullptr;
}

void SoundManager::play(string filename, float gain, float x, float y, float z, float dirX, float dirY, float dirZ)
{
	if (!m_bInitialized)
		return;

	if ( filename.empty())
		return;

	AudioObject* pAudioObj = getAudioObject(filename);

	if (pAudioObj != nullptr)
		pAudioObj->play( gain, x, y, z, dirX, dirY, dirZ);
}

void SoundManager::play(AudioObject* pAudioObj, float gain, float x, float y, float z , float dirX, float dirY, float dirZ)
{
	if (!m_bInitialized)
		return;

	if (pAudioObj == nullptr)
	{
		if (m_bVerbose)
			cout << "ERROR: Null pointer passed to SoundManager::play()\n";
		return;
	}

	pAudioObj->play(gain, x, y, z, dirX, dirY, dirZ);
}
unsigned int SoundManager::getFirstFreeSoundSource()
{
	if (m_freeSoundSources.size() > 0)
	{
		//there are free sources
		unsigned int source = m_freeSoundSources.front();
		m_freeSoundSources.pop_front();
		m_busySoundSources.push_back(source);
		return source;
	}
	return 0;
}

unsigned int SoundManager::getSoundSource()
{
	if (!m_bInitialized)
		return 0;

	if (getVerbose())
		cout << "Sound source requested: " << m_freeSoundSources.size() << " free and " << m_busySoundSources.size() << " busy\n";

	unsigned int firstFree = getFirstFreeSoundSource();
	if (firstFree > 0)
		return firstFree;

	//check the state of all the sources
	auto it = m_busySoundSources.begin();
	while (it!=m_busySoundSources.end())
	{
		int source_state;
		alGetSourcei((*it), AL_SOURCE_STATE, &source_state);
		if (source_state == AL_STOPPED)
		{
			//move from busy to free
			m_freeSoundSources.push_back(*it);
			it= m_busySoundSources.erase(it);
		}
		else it++;
	}
	firstFree = getFirstFreeSoundSource();
	if (firstFree > 0)
		return firstFree;

	//no free sources, need to free one
	//for now, the first
	unsigned int freedSource = m_busySoundSources.front();
	alSourceStop(freedSource);
	return freedSource;
}