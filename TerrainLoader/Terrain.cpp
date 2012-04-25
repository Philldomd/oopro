#include "Terrain.h"
#include "Object.h"

Terrain::Terrain(ID3D10Device* pDevice, int pHeight, int pWidth)
{
	mDevice = pDevice;
	mHeight = pHeight;
	mWidth = pWidth;
	//DEBUG 
	
	objLoader = new OBJLoader();
	m_mesh = new Mesh();
	objLoader->LoadOBJ("Box.obj", m_mesh);

	m_mesh->Initialize(mDevice);

	mLoader = new WorldLoader(mDevice);
	mLoader->loadFromFile("MapTest.png", mWidth,mHeight, 10, m_objects);
	
	mNrOfVertices = mHeight * mWidth;
	mNrOfIndices = (mWidth * 2) * (mHeight - 1) + (mHeight - 2);
}
Terrain::~Terrain()
{
	mVertexBuffer->~Buffer();
	mVertexBuffer = NULL;
	mIndexBuffer->~Buffer();
	mIndexBuffer = NULL;
}

HRESULT Terrain::init()
{
	//Vertex* initVertices = new Vertex[mWidth * mHeight];
	////mNrOfVertices = 0;
	////createVertices(&initVertices);
	//for(UINT i = 0; i < m_objects.size(); i++)
	//{
	//	if(m_objects.at(i) != NULL)
	//	{
	//		(initVertices)[i].position = m_objects.at(i)->getPosition();
	//		(initVertices)[i].texCoord = D3DXVECTOR2(1, 1);
	//		(initVertices)[i].normal = D3DXVECTOR3(0,1,0);
	//		//mNrOfVertices++;
	//	}
	//}

	////Create Vertex Buffer Description
	//BUFFER_INIT_DESC bdVertex;
	//bdVertex.ElementSize = sizeof(Vertex);
	//bdVertex.InitData = initVertices;
	//bdVertex.NumElements = mNrOfVertices;
	//bdVertex.Type = VERTEX_BUFFER;
	//bdVertex.Usage = BUFFER_DEFAULT; //BUFFER_DEFAULT

	////Create Buffer
	//mVertexBuffer = new Buffer();
	//if(FAILED(mVertexBuffer->Init(mDevice, bdVertex)))
	//{
	//	return E_FAIL;
	//}
	//SAFE_DELETE_ARRAY(initVertices);

	//int* initIndices = NULL;

	//createIndices(&initIndices);

	////Index Buffer Description
	//BUFFER_INIT_DESC bdIndex;
	//bdIndex.ElementSize = sizeof( int );
	//bdIndex.InitData = initIndices;
	//bdIndex.NumElements = mNrOfIndices;
	//bdIndex.Type = INDEX_BUFFER;
	//bdIndex.Usage = BUFFER_DEFAULT; //BUFFER_DEFAULT

	////Create Index Buffer
	//mIndexBuffer = new Buffer();
	//if(FAILED(mIndexBuffer->Init(mDevice, bdIndex)))
	//{
	//	return E_FAIL;
	//}
	//SAFE_DELETE_ARRAY(initIndices);

	fx::TerrainFX->SetFloat("gTexScale", mTextureRepeat);

	D3D10_BUFFER_DESC bufferDesc =
    {
        mLoader->getNrWalls() * sizeof( D3DXMATRIX ),
        D3D10_USAGE_DYNAMIC,
        D3D10_BIND_VERTEX_BUFFER,
        D3D10_CPU_ACCESS_WRITE,
        0
    };


	D3DXMATRIX* initMatrices = new D3DXMATRIX[mLoader->getNrWalls()];
	int index = 0;
	for(UINT i = 0; i < m_objects.size(); i++)
	{
		if(m_objects.at(i) != NULL)
		{
			initMatrices[index] = m_objects.at(i)->getWorldMatrix();
			index++;
			mWallIndex = (int)i;
		}
	}

	//Instance Buffer Description
	BUFFER_INIT_DESC bdInstance;
	bdInstance.ElementSize = sizeof( D3DXMATRIX );
	bdInstance.InitData = initMatrices;
	bdInstance.NumElements = mLoader->getNrWalls();
	bdInstance.Type = VERTEX_BUFFER;
	bdInstance.Usage = BUFFER_DEFAULT;

	mInstanceData = new Buffer();
	mInstanceData->Init(mDevice, bdInstance);

	SAFE_DELETE(initMatrices);

	return S_OK;
}

void Terrain::prepToRender(D3DXMATRIX& pWorld, D3DXVECTOR3 pEyePos)//, D3DXMATRIX& pView, D3DXMATRIX& pProjection)
{
	//// Set Input Assembler params
	//mDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	//fx::TerrainFX->SetMatrix("gWVP", pWorld);
	fx::TerrainFX->SetFloat3("gEyePosW", pEyePos);
	fx::InstanceFX->SetMatrix("g_mWorldViewProj",pWorld);

	////set Vertex buffer
	//mVertexBuffer->Apply(0);
	////set Index buffer
	////mIndexBuffer->Apply(0);
}

void Terrain::render(D3DXMATRIX& pView, D3DXMATRIX& pProjection)
{
	//D3D10_TECHNIQUE_DESC techDesc;

	//fx::TerrainFX->GetTechnique()->GetDesc( &techDesc );

	//for( UINT p = 0; p < techDesc.Passes; p++ )
	//{
	//	fx::TerrainFX->Apply(p);
	//	//mDevice->DrawIndexed( mNrOfIndices, 0, 0);
	//	mDevice->Draw(mNrOfVertices, 0);
	//}
	mDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	//for(UINT i = 0; i < m_objects.size(); i++)
	//{
	//	if( m_objects.at(i) != NULL && i%2 == 0)
	//	{
	//		m_mesh->SetBuffer();
	//		fx::TerrainFX->SetMatrix("gWVP", m_objects.at(i)->getWorldMatrix() * pView * pProjection);
	//		//fx::TerrainFX->SetMatrix("gWorld", m_objects.at(i)->getNormalMatrix());
	//	
	//		D3D10_TECHNIQUE_DESC techDesc;

	//		fx::TerrainFX->GetTechnique()->GetDesc( &techDesc );

	//		for( UINT p = 0; p < techDesc.Passes; p++ )
	//		{
	//			fx::TerrainFX->Apply(p);
	//			//mDevice->DrawIndexed( mNrOfIndices, 0, 0);
	//			mDevice->Draw(m_mesh->getNrVertices(), 0);
	//		}
	//	}
	//}

	ID3D10Buffer* pVB[2];
    UINT Strides[2];
    UINT Offsets[2] = {0,0};

	// Render the Walls instanced
	pVB[0] = m_mesh->GetBufferPointer()->GetBufferPointer();
	pVB[1] = mInstanceData->GetBufferPointer();
	Strides[0] = ( UINT )m_mesh->GetBufferPointer()->GetVertexSize();
    Strides[1] = sizeof( D3DXMATRIX );

    mDevice->IASetVertexBuffers( 0, 2, pVB, Strides, Offsets );
    //pd3dDevice->IASetIndexBuffer( g_MeshIsland.GetIB10( 0 ), g_MeshIsland.GetIBFormat10( 0 ), 0 );

    D3D10_TECHNIQUE_DESC techDesc;
	fx::InstanceFX->GetTechnique()->GetDesc( &techDesc );

    /*SDKMESH_SUBSET* pSubset = NULL;
    SDKMESH_MATERIAL* pMat = NULL;
    D3D10_PRIMITIVE_TOPOLOGY PrimType;*/
	
    for( UINT p = 0; p < techDesc.Passes; ++p )
    {
            //pSubset = g_MeshIsland.GetSubset( 0, subset );

            //PrimType = g_MeshIsland.GetPrimitiveType10( ( SDKMESH_PRIMITIVE_TYPE )pSubset->PrimitiveType );
            //pd3dDevice->IASetPrimitiveTopology( PrimType );

           /* pMat = g_MeshIsland.GetMaterial( pSubset->MaterialID );
            if( pMat )
                g_pDiffuseTex->SetResource( pMat->pDiffuseRV10 );*/

		fx::InstanceFX->Apply(0);
		mDevice->DrawInstanced(m_mesh->getNrVertices(), mLoader->getNrWalls(), 0, 0 );
        
    }

}

void Terrain::createVertices(Vertex** pVertices)
{
	*pVertices = new Vertex[mWidth * mHeight];

	float halfWidth = ((float)mWidth - 1.0f)/2;
	float halfHeight = ((float)mHeight - 1.0f)/2;
	float invTwoDX = 1.0f / (2.0f * 1.0f);//Cellspacing 1.0f
	float invTwoDZ = 1.0f / (2.0f * 1.0f);

	//strech texture over area 
	mTextureRepeat = 10.0f;
	mTextureScale = 1.0f;

	float du, dv;
	dv = mTextureRepeat / (mHeight - 1);
	du = mTextureRepeat / (mWidth - 1);

	for(int i = 0; i < mWidth; i++)
	{
		for(int j = 0; j < mHeight; j++)
		{
			(* pVertices)[i*mWidth+j].position = D3DXVECTOR3(i - halfWidth,  0, j - halfHeight);
			(* pVertices)[i*mWidth+j].texCoord = D3DXVECTOR2(du*j * mTextureScale, dv*i * mTextureScale);
			(* pVertices)[i*mHeight+j].normal = D3DXVECTOR3(0,1,0);
		}
	}
}

void Terrain::createIndices(int** pIndices)
{
	int index = 0;
	*pIndices = new int[mNrOfIndices];

	for( int i = 0; i < mHeight -1; i++)
	{
		//Even rows move left to right, odd move right to left.
		if( i % 2 == 0 )
		{
			//Even row
			int x;
			for( x = 0; x < mWidth; x++)
			{
				(*pIndices)[index++] = x + (i * mWidth);
				(*pIndices)[index++] = x + (i * mWidth) + mWidth;
			}
			//Insert degenerate vertex if this isn't the last row
			if( i != mHeight - 2 )
			{
				(*pIndices)[index++] = --x + (i * mWidth);
			}
		}
		else
		{
			//Odd row
			int x;
			for( x = mWidth -1; x >= 0; x-- )
			{
				(*pIndices)[index++] = x + (i * mWidth);
				(*pIndices)[index++] = x + (i * mWidth) + mWidth;
			}
			//Insert degenerate vertex if this isn't the last row
			if( i != mHeight -2 )
			{
				(*pIndices)[index++] = ++x + (i * mWidth);
			}
		}
	}
}

