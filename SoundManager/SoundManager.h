#pragma once

#include <string>
#include <vector>
#include <list>
using namespace std;

class AudioObject;
#define NUM_MAX_SOURCES 32

class SoundManager
{
	static SoundManager* m_pInstance;

	bool m_bInitialized = false;
	vector<AudioObject*> m_audioObjects;

	unsigned int m_soundSources[NUM_MAX_SOURCES];
	list<unsigned int> m_freeSoundSources;
	list<unsigned int> m_busySoundSources;

	void *m_device;
	void *m_context;

	unsigned int getFirstFreeSoundSource();
public:
	SoundManager();
	virtual ~SoundManager();

	int getAudioObjectId(string filename);

	int createAudioObject(string filename);

	void play(int audioObjectId, double x, double y, double z, double gain);

	unsigned int getSoundSource();

	static SoundManager* getInstance() { return m_pInstance; }
};

