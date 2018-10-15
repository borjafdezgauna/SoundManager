#pragma once

#include <string>
#include <vector>
using namespace std;

class AudioObject;

class SoundManager
{
	bool m_bInitialized = false;
	vector<AudioObject*> m_audioObjects;

	void *m_device;
	void *m_context;
public:
	SoundManager();
	virtual ~SoundManager();

	int getAudioObjectId(string filename);

	int createAudioObject(string filename);

	void play(int audioObjectId, double x, double y, double z, double gain);
};

