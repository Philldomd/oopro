#ifndef GEOLISTENER__H
#define GEOLISTENER__H

#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "Object.h"

class Listener
{
public:
	Listener();
	~Listener();
	void initGeometry(vector<Object*> p_objects);
	void initObjects();
	void updateObjectSoundPos(Object* p_objects);
	void doListenerMovement();
	FMOD_RESULT		m_result;

private:

	void ERRCHECK(FMOD_RESULT result)
		
	FMOD::Sound* m_sound;
	FMOD::System* m_system;
	FMOD::Channel* m_channel;
	FMOD::Geometry* m_geometry;
};


#endif