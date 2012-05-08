#ifndef SOUND__H
#define SOUND__H


#include <stdio.h>
#include "stdafx.h"
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"

class Sound
{
public:
	Sound(FMOD::Sound* pSound, FMOD::System* pSystem);
	~Sound();
	void Init(const char* NAME);
	void Play(FMOD::Channel* pChannel);
	void Pause();
	bool PlayingBool();
	void Loop(FMOD::Channel* pChannel);
	void Stop();
	void Volym(float pVolym);
	void Destruct();

private:
	void ERRCHECK(FMOD_RESULT result);
	FMOD_RESULT		mResult;	
	FMOD::Sound* mSound;
	FMOD::System* mSystem;
	FMOD::Channel* mChannel;
	unsigned int	mVersion;
	float mVolym;
	bool mPaused;
	bool mDestructed;

};
#endif