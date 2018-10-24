# SoundManager


This is a C++ sound library based on 3rd-party OpenAL-Soft and AudioFile libraries. It has basic functionality for playing sounds and music in a game.

# Including the library in your solution

1. Include the projects in your solution. For easier integration, all the source code of the dependencies is included. These three projects need to be included in your project:

- OpenALSoft/OpenAL-Lib/OpenAL-Lib.vcxproj
- OpenALSoft/OpenAL-Lib/common.vcxproj
- SoundManager/SoundManager.vcxproj (references the previous two projects)

2. Make your project reference the last one: _SoundManager_

# Example of use:

There is an example project you can use as reference: test/test.vcxproj

# Basic use:

1. Instantiate the sound manager. Only once in your program:

```
#include "../SoundManager/SoundManager.h" //relative path to the main header
SoundManager soundManager;
```

2. Access the sound manager (if not defined in the same file) and create all the audio objects required in your program:

```
SoundManager* pSoundManager= SoundManager::getInstance();
pSoundManager->createAudioObject("../snd/audiofile-01.wav");
pSoundManager->createAudioObject("../snd/audiofile-02.wav");
...
```

3. Play audio objects:

```
int audioObjId= pSoundManager->getAudioObjectId("../snd/audiofile-01.wav");
pSoundManager->play(audioObjId, GAIN, POS_X, POS_Y, POS_Z, DIR_X, DIR_Y, DIR_Z);
```

4. All resources are automatically freed when the sound manager object is destroyed. If you created the SoundManager using _new_, call _delete_ on the pointer. Otherwise, no need to do anything
  
