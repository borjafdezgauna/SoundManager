#pragma once

#include "AudioFile.h"

#include <string>
using namespace std;

class AudioObject
{
	unsigned int m_buffer;

	bool m_bLoaded = false;
	string m_sourceFilename;

public:
	AudioObject(string filename);
	virtual ~AudioObject();

	string getSourceFilename();

	void play(double x, double y, double z, double gain);
};

