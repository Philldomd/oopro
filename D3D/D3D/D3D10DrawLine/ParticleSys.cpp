#include "ParticleSys.h"


#include <stdlib.h>

namespace
{
	struct ParticleVertex
	{
		D3DXVECTOR3 initialPos;
		D3DXVECTOR3 initialVel;
		D3DXVECTOR2 size;
		float age;
		unsigned int type;
	};
}
PSystem::PSystem()
: md3dDevice(0), mInitVB(0), mDrawVB(0), mStreamOutVB(0), mTexArrayRV(0), mRandomTexRV(0)
{
	mFirstRun = true;
	mGameTime = 0.0f;
	mTimeStep = 0.0f;
	mAge      = 0.0f;

	mEyePosW  = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	mEmitPosW = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	mEmitDirW = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 0.0f);
}

PSystem::~PSystem()
{
	mInitVB->~Buffer();
	mDrawVB->~Buffer();
	mStreamOutVB->~Buffer();
}

float PSystem::getAge()const
{
	return mAge;
}

void PSystem::setEyePos(const D3DXVECTOR3& eyePosW)
{
	mEyePosW = D3DXVECTOR4(eyePosW.x, eyePosW.y, eyePosW.z, 1.0f);
}

void PSystem::setEmitPos(const D3DXVECTOR3& emitPosW)
{
	mEmitPosW = D3DXVECTOR4(emitPosW.x, emitPosW.y+12, emitPosW.z, 1.0f);
}

void PSystem::setEmitDir(const D3DXVECTOR3& emitDirW)
{
	mEmitDirW = D3DXVECTOR4(emitDirW.x, emitDirW.y, emitDirW.z, 0.0f);
}

void PSystem::init(ID3D10Device* device, ID3D10Effect* FX, 
				   UINT maxParticles,char* name)
{
	md3dDevice = device;
	mName = name;
	mMaxParticles = maxParticles;

	createTex(); 
	
	//
	// Set IA stage.
	//
	D3D10_INPUT_ELEMENT_DESC particleDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"SIZE",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"AGE",      0, DXGI_FORMAT_R32_FLOAT,       0, 32, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TYPE",     0, DXGI_FORMAT_R32_UINT,        0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};
	
	FX = loadEffect(mName);
	

 
	mStreamOutTech  = FX->GetTechniqueByName("StreamOutTech");	
	mDrawTech       = FX->GetTechniqueByName("DrawTech");	
	mfxViewProjVar  = FX->GetVariableByName("gViewProj")->AsMatrix();
	mfxGameTimeVar  = FX->GetVariableByName("gGameTime")->AsScalar();
	mfxTimeStepVar  = FX->GetVariableByName("gTimeStep")->AsScalar();
	mfxEyePosVar    = FX->GetVariableByName("gEyePosW")->AsVector();
	mfxEmitPosVar   = FX->GetVariableByName("gEmitPosW")->AsVector();
	mfxEmitDirVar   = FX->GetVariableByName("gEmitDirW")->AsVector();	
	mfxRandomTexVar = FX->GetVariableByName("gRandomTex")->AsShaderResource();
	mfxRandomTexVar->SetResource(mRandomTexRV);
	
	D3D10_PASS_DESC PassDesc;
	mStreamOutTech->GetPassByIndex(0)->GetDesc(&PassDesc);
	md3dDevice->CreateInputLayout(particleDesc, 5, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &Particle);
	buildVB();
}

void PSystem::reset()
{
	mFirstRun = true;
	mAge      = 0.0f;
}

void PSystem::update(float dt, float gameTime)
{
	mGameTime = gameTime;
	mTimeStep = dt;

	mAge += dt;
}

void PSystem::draw(Camera camera)
{
	D3DXMATRIX V = camera.getViewMatrix();
	D3DXMATRIX P = camera.getProjectionMatrix();

	//
	// Set constants.
	//
	mfxViewProjVar->SetMatrix((float*)&(V*P));
	mfxGameTimeVar->SetFloat(mGameTime);
	mfxTimeStepVar->SetFloat(mTimeStep);
	mfxEyePosVar->SetFloatVector((float*)&mEyePosW);
	mfxEmitPosVar->SetFloatVector((float*)&mEmitPosW);
	mfxEmitDirVar->SetFloatVector((float*)&mEmitDirW);
	mfxRandomTexVar->SetResource(mRandomTexRV);
	
	md3dDevice->IASetInputLayout(Particle);
    md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	UINT stride = sizeof(ParticleVertex);
    UINT offset = 0;

	// On the first pass, use the initialization VB.  Otherwise, use
	// the VB that contains the current particle list.
	if( mFirstRun )
		mInitVB->Apply(0);
	else
		mDrawVB->Apply(0);

	//
	// Draw the current particle list using stream-out only to update them.  
	// The updated vertices are streamed-out to the target VB. 
	//
	ID3D10Buffer* temp = mStreamOutVB->GetBufferPointer();
	offset = 0;
	md3dDevice->SOSetTargets(1,&temp,&offset);
	temp = NULL;

    D3D10_TECHNIQUE_DESC techDesc;
    mStreamOutTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        mStreamOutTech->GetPassByIndex( p )->Apply(0);
        
		if( mFirstRun )
		{
			md3dDevice->Draw(1, 0);
			mFirstRun = false;
		}
		else
		{
			md3dDevice->DrawAuto();
		}
    }

	// done streaming-out--unbind the vertex buffer
	ID3D10Buffer* bufferArray[1] = {0};
	md3dDevice->SOSetTargets(1, bufferArray, &offset);

	// ping-pong the vertex buffers
	std::swap(mDrawVB, mStreamOutVB);

	//
	// Draw the updated particle system we just streamed-out. 
	//
	mDrawVB->Apply(0);

	mDrawTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        mDrawTech->GetPassByIndex( p )->Apply(0);
        
		md3dDevice->DrawAuto();
    }
}

void PSystem::buildVB()
{
	//
	// Create the buffer to kick-off the particle system.
	//
	ParticleVertex p;
	ZeroMemory(&p, sizeof(ParticleVertex));
	p.age  = 0.0f;
	p.type = 0; 

	BUFFER_INIT_DESC vbd;
	vbd.Usage = BUFFER_DEFAULT;
	vbd.ElementSize = sizeof(ParticleVertex) * 1;
	vbd.Type = VERTEX_BUFFER;
	vbd.InitData = &p;
	vbd.NumElements = 1;
	// The initial particle emitter has type 0 and age 0.  The rest
	// of the particle attributes do not apply to an emitter.
	mInitVB = new Buffer();
	mInitVB->Init(md3dDevice, vbd);
	
	//
	// Create the ping-pong buffers for stream-out and drawing.
	//
	BUFFER_INIT_DESC dbf;
	dbf.Usage = BUFFER_STREAM_OUT_TARGET;
	dbf.Type = VERTEX_BUFFER;
	dbf.ElementSize = sizeof(ParticleVertex);
	dbf.NumElements = mMaxParticles;
	dbf.InitData = 0;

	mDrawVB = new Buffer();
	mDrawVB->Init(md3dDevice, dbf);
	mStreamOutVB = new Buffer();
	mStreamOutVB->Init(md3dDevice,dbf);
}


void PSystem::createTex()
{
	// 
	// Create the random data.
	//
	D3DXVECTOR4 randomValues[1024];

	for(int i = 0; i < 1024; ++i)
	{
		randomValues[i].x = RandF(-1.0f, 1.0f);
		randomValues[i].y = RandF(-1.0f, 1.0f);
		randomValues[i].z = RandF(-1.0f, 1.0f);
		randomValues[i].w = RandF(-1.0f, 1.0f);
	}

    D3D10_SUBRESOURCE_DATA initData;
    initData.pSysMem = randomValues;
	initData.SysMemPitch = 1024*sizeof(D3DXVECTOR4);
    initData.SysMemSlicePitch = 1024*sizeof(D3DXVECTOR4);
	//
	// Create the texture.
	//
    D3D10_TEXTURE1D_DESC texDesc;
    texDesc.Width = 1024;
    texDesc.MipLevels = 1;
    texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    texDesc.Usage = D3D10_USAGE_IMMUTABLE;
    texDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;
    texDesc.ArraySize = 1;

	ID3D10Texture1D* randomTex = 0;
    md3dDevice->CreateTexture1D(&texDesc, &initData, &randomTex);
	//
	// Create the resource view.
	//
    D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
    viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE1D;
    viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
	viewDesc.Texture1D.MostDetailedMip = 0;
	
    md3dDevice->CreateShaderResourceView(randomTex, &viewDesc, &mRandomTexRV);

	randomTex->Release();
}
ID3D10Effect* PSystem::loadEffect(char* name)
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG;

  
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	ID3D10Effect* fx = 0;
	hr = D3DX10CreateEffectFromFile(name , 0, 0, 
		"fx_4_0", shaderFlags, 0, md3dDevice, 0, 0, &fx, &compilationErrors, 0);
	if(FAILED(hr))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			compilationErrors->Release();
		}
		DXTrace(__FILE__, (DWORD)__LINE__, hr, name, true);
	}
	return fx;
}