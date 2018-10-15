#include "AudioObject.h"
#include "../OpenALSoft/include/AL/al.h"


AudioObject::AudioObject(string filename)
{
	AudioFile<float> audioFile;
	m_bLoaded= audioFile.load(filename);
	m_sourceFilename = filename;

	if (m_bLoaded)
	{
		//create audio object and set basic properties
		alGenSources((ALuint)1, &m_id);
		alSourcef(m_id, AL_PITCH, 1);
		alSourcef(m_id, AL_GAIN, 1);
		alSource3f(m_id, AL_POSITION, 0, 0, 0);
		alSource3f(m_id, AL_VELOCITY, 0, 0, 0);
		alSourcei(m_id, AL_LOOPING, AL_FALSE);



		//format
		unsigned int format;
		double minSample,sampleWidth;
		if (audioFile.getBitDepth() == 8)
		{
			minSample = 0;
			sampleWidth = 255;
		}
		else
		{
			minSample = -32768;
			sampleWidth = 65536;
		}
		if (audioFile.isStereo())
		{
			if (audioFile.getBitDepth() == 8)
				format = AL_FORMAT_STEREO8;
			else format = AL_FORMAT_STEREO16;
		}
		else
		{
			if (audioFile.getBitDepth() == 8)
				format = AL_FORMAT_MONO8;
			else format = AL_FORMAT_MONO16;
		}

		//set the audio data: rearrange and send to openAL
		//assuming 16 pcm files for now
		int audioDataSize= audioFile.getNumChannels()*audioFile.getNumSamplesPerChannel();
		short* pData = new short[audioDataSize];
		int i = 0;
		double normSample;
		for (int sample = 0; sample < audioFile.getNumSamplesPerChannel(); sample++)
		{
			for (int channel = 0; channel < audioFile.getNumChannels(); channel++)
			{
				normSample = (1.0 + audioFile.samples[channel][sample]) * .5;
				pData[i] = (short) (minSample + normSample*sampleWidth);
				i++;
			}
		}
		//allocate the buffer object
		alGenBuffers((ALuint)1, &m_buffer);
		alBufferData(m_buffer, format, pData, audioDataSize, audioFile.getSampleRate());
		
		//bind source and buffer
		alSourcei(m_id, AL_BUFFER, m_buffer);
		delete [] pData;
	}
}


AudioObject::~AudioObject()
{
	int source_state;
	alGetSourcei(m_id, AL_SOURCE_STATE, &source_state);

	while (source_state == AL_PLAYING)
	{
		alGetSourcei(m_id, AL_SOURCE_STATE, &source_state);
	}
	alDeleteSources(1, &m_id);
	alDeleteBuffers(1, &m_buffer);
}

string AudioObject::getSourceFilename()
{
	return m_sourceFilename;
}

unsigned int AudioObject::getId()
{
	return m_id;
}

void AudioObject::play(double x, double y, double z, double gain)
{
	int source_state;
	alGetSourcei(m_id, AL_SOURCE_STATE, &source_state);

	//if (source_state == AL_)
	{
		alSource3f(m_id, AL_POSITION, (float)x, (float)y, (float)z);
		alSourcef(m_id, AL_GAIN, (float)gain);
		alSourcePlay(m_id);
	}
}