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

2. Access the sound manager (if not defined in the same file) and load all the audio files required in your program. Loading is done on the same thread, so it's best to load all the files before starting the main loop:

```
SoundManager* pSoundManager= SoundManager::getInstance();
pSoundManager->load("../snd/audiofile-01.wav");
pSoundManager->load("../snd/audiofile-02.wav");
...
```

3. Play audio objects. They will automatically be played by a different thread, so that the main thread is not blocked:

```
pSoundManager->play("../snd/audiofile-01.wav", GAIN, POS_X, POS_Y, POS_Z, DIR_X, DIR_Y, DIR_Z);
```

4. All resources are automatically freed when the sound manager object is destroyed. If you created the SoundManager using _new_, call _delete_ on the pointer. Otherwise, no need to do anything
  
