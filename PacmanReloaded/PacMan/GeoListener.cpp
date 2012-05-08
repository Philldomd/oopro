#include "GeoListener.h"


Listener::Listener(){}
Listener::~Listener(){}
void Listener::initGeometry(vector<Object*> p_objects)
{
	m_result = m_system->createGeometry(p_objects.size() * 12, p_objects.size() * 8, &m_geometry);
	ERRCHECK(m_result);
}


void Listener::ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		LPCSTR error = FMOD_ErrorString(result);
		MessageBoxA(0,error, "FMOD error!", 0);
	}
}