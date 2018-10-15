#include "stdafx.h"
#include "SoundManager.h"
#include "AudioObject.h"

#include "../OpenALSoft/include/AL/al.h"
#include "../OpenALSoft/include/AL/alc.h"


SoundManager::SoundManager()
{
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

	m_bInitialized = true;
}


SoundManager::~SoundManager()
{
	for (size_t i = 0; i < m_audioObjects.size(); i++)
		delete m_audioObjects[i];

	m_device = alcGetContextsDevice((ALCcontext*)m_context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext((ALCcontext*)m_context);
	alcCloseDevice((ALCdevice*)m_device);
}

int SoundManager::createAudioObject(string filename)
{
	int audioId = getAudioObjectId(filename);

	if (audioId >= 0)
		return audioId;

	AudioObject* pNewAudioObject= new AudioObject(filename);

	m_audioObjects.push_back(pNewAudioObject);

	return m_audioObjects.size()-1;
}

int SoundManager::getAudioObjectId(string filename)
{
	for (size_t i = 0; i < m_audioObjects.size(); i++)
	{
		if (m_audioObjects[i]->getSourceFilename() == filename)
			return i;
	}
	return -1;
}

void SoundManager::play(int audioObjectId, double x= 0, double y= 0, double z= 0, double gain= 1.0)
{
	if (audioObjectId<0 || (size_t)audioObjectId>m_audioObjects.size())
		return;

	m_audioObjects[audioObjectId]->play(x,y,z,gain);

	int error= alGetError();
}