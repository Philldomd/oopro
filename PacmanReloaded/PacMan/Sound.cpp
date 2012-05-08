#include "Sound.h"

Sound::Sound(FMOD::Sound* pSound, FMOD::System* pSystem)
{
	mSound = pSound;
	mSystem = pSystem;
	mPaused = false;
	mVolym = 1.0f;
	mDestructed = false;
}

Sound::~Sound()
{
	mChannel->stop();
	mSound->release();
	mSystem->release();
}

void Sound::Init(const char* NAME)
{
	mResult = FMOD::System_Create(&mSystem);
	ERRCHECK(mResult);
	mResult = mSystem->getVersion(&mVersion);
	ERRCHECK(mResult);

	if ( mVersion < FMOD_VERSION )
	{
		LPCSTR error = (LPCSTR)mVersion;
		MessageBoxA(0,error, "Error! You are using an old version of FMOD %08x. This program requiers %08x\n", 0);
	}
	mResult = mSystem->init(1,FMOD_INIT_NORMAL,0);
	ERRCHECK(mResult);

	mResult = mSystem->createSound(NAME, FMOD_HARDWARE, 0, &mSound);
	ERRCHECK(mResult);
}

void Sound::Play(FMOD::Channel* pChannel)
{
	if(!mDestructed)
	{
		mChannel = pChannel;
		mResult = mSystem->playSound(FMOD_CHANNEL_FREE, mSound, mPaused, &mChannel);
		ERRCHECK(mResult);
	}
	else{}
}

void Sound::Loop(FMOD::Channel* pChannel)
{
	if(!mDestructed)
	{
		mSound->setMode(FMOD_LOOP_NORMAL);
		mChannel = pChannel;
		mResult = mSystem->playSound(FMOD_CHANNEL_FREE, mSound, mPaused, &mChannel);
		ERRCHECK(mResult);
	}
	else{}
}

bool Sound::PlayingBool()
{
	bool playing;
	mChannel->isPlaying(&playing);
	return playing;
}
void Sound::Pause()
{
	mChannel->getPaused(&mPaused);
	mChannel->setPaused(!mPaused);
}

void Sound::Destruct()
{
	mDestructed = !mDestructed;
}

void Sound::Stop()
{
	mChannel->stop();
}

void Sound::Volym(float pVolym)
{
	mVolym = mVolym - pVolym;
	mChannel->setVolume(mVolym);
}

void Sound::ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		LPCSTR error = FMOD_ErrorString(result);
		MessageBoxA(0,error, "FMOD error!", 0);
	}
}