// AudioRendererTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <thread>
#include "../SoundManager/SoundManager.h"


int main()
{
    std::cout << "Testing the sound manager\n"; 
	SoundManager soundManager;

	int audioObj= soundManager.createAudioObject("../snd/explosion.wav");

	soundManager.play(audioObj,-1.0,0.0,0.0,0.1);

	std::this_thread::sleep_for(1s);
	soundManager.play(audioObj, 1.0, 0.0, 0.0,1.0);
}