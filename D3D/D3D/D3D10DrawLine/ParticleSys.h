#ifndef PARTICLESYS_H
#define PARTICLESYS_H

#include "stdafx.h"
#include "Camera.h"
#include "Buffer.h"
#include <string>
#include <vector>

class PSystem
{
public:
	
	PSystem();
	~PSystem();

	// Time elapsed since the system was reset.
	float getAge()const;
	void setEyePos(const D3DXVECTOR3& eyePosW);
	void setEmitPos(const D3DXVECTOR3& emitPosW);
	void setEmitDir(const D3DXVECTOR3& emitDirW);
	void createTex();
	void init(ID3D10Device* device, ID3D10Effect* FX, UINT maxParticles, char* name);
	ID3D10Effect* loadEffect(char* name);
	void reset();
	void update(float dt, float gameTime);
	void draw(Camera camera);

private:
	void buildVB();

	PSystem(const PSystem& rhs);
	PSystem& operator=(const PSystem& rhs);
 
private:
 
	UINT mMaxParticles;
	bool mFirstRun;

	char* mName;
	float mGameTime;
	float mTimeStep;
	float mAge;

	D3DXVECTOR4 mEyePosW;
	D3DXVECTOR4 mEmitPosW;
	D3DXVECTOR4 mEmitDirW;

	ID3D10Device* md3dDevice;
	Buffer* mInitVB;	
	Buffer* mDrawVB;
	Buffer* mStreamOutVB;
 
	ID3D10InputLayout* Particle;

	ID3D10ShaderResourceView* mTexArrayRV;
	ID3D10ShaderResourceView* mRandomTexRV;

	ID3D10EffectTechnique* mStreamOutTech;
	ID3D10EffectTechnique* mDrawTech;
	ID3D10EffectMatrixVariable* mfxViewProjVar;
	ID3D10EffectScalarVariable* mfxGameTimeVar;
	ID3D10EffectScalarVariable* mfxTimeStepVar;
	ID3D10EffectVectorVariable* mfxEyePosVar;
	ID3D10EffectVectorVariable* mfxEmitPosVar;
	ID3D10EffectVectorVariable* mfxEmitDirVar;
	ID3D10EffectShaderResourceVariable* mfxTexArrayVar;
	ID3D10EffectShaderResourceVariable* mfxRandomTexVar;

	D3DX10INLINE float PSystem::RandF()
{
	return (float)(rand()) / (float)RAND_MAX;
}

// Returns random float in [a, b].
D3DX10INLINE float PSystem::RandF(float a, float b)
{
	return a + RandF()*(b-a);
}
};

#endif // PARTICLESYS_H