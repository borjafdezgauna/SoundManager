// AudioRendererTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <thread>
#include "../SoundManager/SoundManager.h"


int main()
{
    std::cout << "Sound manager test\n"; 
	SoundManager soundManager;

	soundManager.setVerbose(true);

	int soundtrack = soundManager.createAudioObject("../snd/soundtrack-01.wav");
	soundManager.play(soundtrack, 1.f);

	int explosion= soundManager.createAudioObject("../snd/explosion.wav");
	for (int i = 0; i < 30; i++)
	{
		soundManager.play(explosion, 0.5f -1.f, 0.f, 0.f, 0.f, 0.f, 0.f);
		std::this_thread::sleep_for(0.5s);

		soundManager.play(explosion, 1.f, 1.f, 0.f, 0.f, -1.f, 0.f, 0.f);
		std::this_thread::sleep_for(0.5s);
	}
}