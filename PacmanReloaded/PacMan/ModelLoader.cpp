#include "ModelLoader.h"


ModelLoader::ModelLoader(ID3D10Device* p_d3dDevice)
{
	m_d3dDevice = p_d3dDevice;

	m_indices.clear();
	m_vertices.clear();

	m_positions.clear();
	m_positions.shrink_to_fit();
	m_texCoords.clear();
	m_texCoords.shrink_to_fit();
	m_normals.clear();
	m_normals.shrink_to_fit();

	m_topCorner = D3DXVECTOR3((float)INT_MAX, (float)INT_MAX, (float)INT_MAX);
	m_bottomCorner = D3DXVECTOR3(0,0,0);

	char stringBuffer[MAX_PATH];
	GetModuleFileName( NULL, stringBuffer, MAX_PATH );
	std::string::size_type charPosition = string( stringBuffer ).find_last_of( "\\/" );
	std::string baseDirectory = string( stringBuffer ).substr( 0, charPosition);
	charPosition = baseDirectory.find_last_of( "\\/" );
	baseDirectory = baseDirectory.substr( 0, charPosition);

	m_resourceFolder = baseDirectory.append("\\PacMan\\Resources\\Models\\");
	
}


Model ModelLoader::addStaticModel( string p_modelName, string p_OBJFileName )
{
	loadModelFromOBJFile(p_OBJFileName);

	createBuffers();

	Model tmpModel;

	tmpModel.m_bufferName = p_modelName;
	tmpModel.m_vertexBuffer = m_vertexBuffer;
	tmpModel.m_indexBuffer = m_indexBuffer;
	tmpModel.m_offset = 0;
	tmpModel.m_stride = sizeof(Vertex);
	tmpModel.m_size = m_indices.size();
	tmpModel.m_material = m_material;
	tmpModel.m_bottomBoundingCorner = m_bottomCorner;
	tmpModel.m_topBoundingCorner = m_topCorner;

	return tmpModel;
}

HRESULT ModelLoader::createBuffers()
{
	//Create Vertex Buffer Description
	BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = sizeof(Vertex);
	bufferDesc.InitData = &m_vertices[0];
	bufferDesc.NumElements = m_vertices.size();
	bufferDesc.Type = VERTEX_BUFFER;
	bufferDesc.Usage = BUFFER_DEFAULT; 

	//Create Buffer
	m_vertexBuffer = new Buffer();
	if(FAILED(m_vertexBuffer->init(m_d3dDevice, bufferDesc)))
	{
		return E_FAIL;
	}

	//Create Vertex Buffer Description
	BUFFER_INIT_DESC indexBufferDesc;
	indexBufferDesc.ElementSize = sizeof( int );
	indexBufferDesc.InitData = &m_indices[0];
	indexBufferDesc.NumElements = m_indices.size();
	indexBufferDesc.Type = INDEX_BUFFER;
	indexBufferDesc.Usage = BUFFER_DEFAULT;

	//Create Buffer
	m_indexBuffer = new Buffer();
	if(FAILED(m_indexBuffer->init(m_d3dDevice, indexBufferDesc)))
	{
		return E_FAIL;
	}

	return S_OK;
}

void ModelLoader::calculateBoundingBox(D3DXVECTOR3 p_vector3)
{
	if(m_topCorner == D3DXVECTOR3((float)INT_MAX, (float)INT_MAX, (float)INT_MAX))
	{
		m_topCorner = p_vector3;
		m_bottomCorner = p_vector3;
	}
	if(p_vector3.x < m_bottomCorner.x) 
		m_bottomCorner.x = p_vector3.x;
	if(p_vector3.y < m_bottomCorner.y)
		m_bottomCorner.y = p_vector3.y;
	if(p_vector3.z < m_bottomCorner.z)
		m_bottomCorner.z = p_vector3.z;
	if(p_vector3.x > m_topCorner.x)
		m_topCorner.x = p_vector3.x;
	if(p_vector3.y > m_topCorner.y)
		m_topCorner.y = p_vector3.y;
	if(p_vector3.z > m_topCorner.z)
		m_topCorner.z = p_vector3.z;
}


ID3D10ShaderResourceView* ModelLoader::createTexture( string p_textureFileName )
{
	string texturePath;
	texturePath.assign(m_resourceFolder.begin(), m_resourceFolder.end());
	texturePath.append("Textures\\");
	texturePath.append(p_textureFileName.begin(),p_textureFileName.end());

	ID3D10ShaderResourceView* resourceView = NULL;
	D3DX10CreateShaderResourceViewFromFile(m_d3dDevice, texturePath.c_str() , 0, 0, &resourceView, 0 );

	return resourceView;
}

int ModelLoader::calculateIndex( Vertex* p_vertex )
{
	for (int i = 0; i < (int)m_vertices.size(); i++)
	{
		if(m_vertices[i].pos == p_vertex->pos)
			return i;
	}
	int indexReturn = m_vertices.size();
	m_vertices.push_back(*p_vertex);
	return indexReturn;
}

void ModelLoader::loadMaterialFromMTL( string p_materialFileName )
{
	m_material = new Material();

	fstream objFile(m_resourceFolder + p_materialFileName);

	if(objFile)
	{
		string line;
		string prefix;

		while(objFile.eof() == false)
		{
			prefix = "NULL"; //leave nothing from the previous iteration
			stringstream lineStream;

			getline(objFile, line);
			lineStream << line;
			lineStream >> prefix;

			if(prefix == "map_Kd")
			{
				string materialTextureName;
				lineStream >> materialTextureName;
				m_material->m_textureResource = createTexture(materialTextureName);
			}
			else if(prefix == "Ns")
			{
				int nShininess;
				lineStream >> nShininess;
				m_material->m_shininess = nShininess;
			}
			else if(prefix == "d" || prefix == "Tr" )
			{
				lineStream >> m_material->m_alpha;
			}
			else if(prefix == "Ks")
			{
				float r, g, b;
				lineStream >> r >> g >> b;
				m_material->m_diffuse = D3DXVECTOR3( r, g, b );
			}
			else if(prefix == "Kd")
			{
				float r, g, b;
				lineStream >> r >> g >> b;
				m_material->m_specular = D3DXVECTOR3( r, g, b );
			}	
			else if(prefix == "Ka")
			{
				float r, g, b;
				lineStream >> r >> g >> b;
				m_material->m_ambient = D3DXVECTOR3( r, g, b );
			}
		}
	}
}

void ModelLoader::loadModelFromOBJFile( string p_OBJFileName )
{
	fstream objFile(m_resourceFolder + p_OBJFileName);

	if(objFile)
	{

		string strMaterialFilename;
		string line;
		string prefix;

		while(objFile.eof() == false)
		{
			prefix = "NULL"; //leave nothing from the previous iteration
			stringstream lineStream;

			getline(objFile, line);
			lineStream << line;
			lineStream >> prefix;

			if(prefix == "mtllib")
			{
				lineStream >> strMaterialFilename;
				loadMaterialFromMTL(strMaterialFilename);
			}
			else if(prefix == "v")
			{
				D3DXVECTOR3 pos;
				lineStream >> pos.x >> pos.y >> pos.z;
				m_positions.push_back(pos);
				calculateBoundingBox(pos);
			}
			else if(prefix == "vt")
			{
				D3DXVECTOR2 uv;
				lineStream >> uv.x >> uv.y;
				uv.y = 1 - uv.y; 
				m_texCoords.push_back(uv);
			}
			else if (prefix == "vn")
			{
				D3DXVECTOR3 normal;
				lineStream >> normal.x >> normal.y >> normal.z;
				m_normals.push_back(normal);
			}
			else if(prefix == "f")
			{
				Vertex tempVertex;
				char tmp;

				int indexPos = 0;
				int texPos = 0;
				int normPos = 0;

				for(int i=0; i<3; i++)
				{

					lineStream >> indexPos >> tmp >> texPos >>  tmp >> normPos;

					ZeroMemory(&tempVertex, sizeof(Vertex));

					tempVertex.pos = m_positions[ indexPos - 1];
					tempVertex.texC =  m_texCoords[ texPos-1 ];
					tempVertex.normal = m_normals[ normPos - 1 ]; 
					tempVertex.diff = D3DXVECTOR4(m_material->m_diffuse, 1.0f);
					tempVertex.spec = D3DXVECTOR4(m_material->m_specular, 1.0f);

					int index = calculateIndex(&tempVertex);
					m_indices.push_back(index);
				}
			}
		}

		m_indices.shrink_to_fit();
	}

}


ModelLoader::~ModelLoader()
{
}






