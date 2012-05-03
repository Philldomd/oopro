#ifndef EFFECTS_H
#define EFFECTS_H

#include "d3dUtil.h"
#include "Shader.h"

namespace fx
{
	//extern Shader* TerrainFX;
	extern Shader* InstanceFX;

	void InitAll(ID3D10Device* device);
	void DestroyAll();
};
#endif