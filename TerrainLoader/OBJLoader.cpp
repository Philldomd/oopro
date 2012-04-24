#include "OBJLoader.h"

OBJLoader::OBJLoader()
{}
OBJLoader::~OBJLoader()
{}

HRESULT OBJLoader::LoadOBJ(char* p_filename, Mesh* p_mesh)
{
	WCHAR line[256] = {0};
	wifstream inF(p_filename);
	if(inF.is_open())
	{
		//Get file size
		inF.seekg (0, ios::end);
		long fileSize = (long)inF.tellg();
		inF.seekg (0, ios::beg);
		
		//Allocate memory
		/*m_positionArray.reserve(fileSize);
		m_texCoordArray.reserve(fileSize);
		m_normalArray.reserve(fileSize);
		p_mesh->SetReserveVertexSize(fileSize);*/

		while(!inF.eof())
		{
			inF >> line;
			if(wcscmp( line, L"#") == 0)
			{
				//Comment - skip Line
			}
			else if(wcscmp( line, L"v") == 0)
			{
				//Vertex position
				float x, y, z;
				inF >> x >> y >> z;
				m_positionArray.push_back(D3DXVECTOR3(x,y,z));
			}
			else if(wcscmp( line, L"vt") == 0)
			{
				//Vertex TexCoord
				float x, y;
				inF >> x >> y;
				m_texCoordArray.push_back(D3DXVECTOR2(x,y));
			}
			else if(wcscmp( line, L"vn") == 0)
			{
				//Vertex normal
				float x, y, z;
				inF >> x >> y >> z;
				m_normalArray.push_back(D3DXVECTOR3(x,y,z));
			}
			else if(wcscmp( line, L"f") == 0)
			{
				// Face
				UINT iPosition, iTexCoord, iNormal;

				for( UINT iFace = 0; iFace < 3; iFace++)
				{
					// OBJ format uses 1-based arrays
					inF >> iPosition;
					iPosition--;

					if( '/' == inF.peek() )
					{
						inF.ignore();

						if( '/' != inF.peek() )
						{
							// Optional texture coordinate
							inF >> iTexCoord;
							iTexCoord--;
						}
						if( '/' == inF.peek() )
						{
							inF.ignore();

							// Optional vertex normal
							inF >> iNormal;
							iNormal--;
						}
					}

					//Add Vertex to Mesh
					p_mesh->AddVertex(&m_positionArray[iPosition], &m_texCoordArray[iTexCoord],  &m_normalArray[iNormal]);
				}
			}
			else
			{
				//unrecognized command
			}
		}
		//close stream
		inF.close();

		//Clear unessesary data
		p_mesh->ShrinkToFit();
		Clear();
		ShrinkToFit();
		
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

void OBJLoader::Clear()
{
	m_positionArray.clear();
	m_texCoordArray.clear();
	m_normalArray.clear();
}

//	release unnecessarily allocated memory (uses the swap trick, cf. S. Meyers' Effective STL, Item 17)
void OBJLoader::ShrinkToFit()
{
	vector<D3DXVECTOR3>(m_positionArray).swap(m_positionArray);
	vector<D3DXVECTOR2>(m_texCoordArray).swap(m_texCoordArray);
	vector<D3DXVECTOR3>(m_normalArray).swap(m_normalArray);
}