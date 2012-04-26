#include "OBJLoader.h"

OBJLoader::OBJLoader()
{}
OBJLoader::~OBJLoader()
{}

HRESULT OBJLoader::LoadOBJ(char* p_filename, Mesh* p_mesh)
{
	ifstream inF(p_filename);
	string buffer;

	if(inF.is_open())
	{
		while(getline(inF, buffer))
		{
			istringstream line(buffer);
			string garbage;
			
			if (buffer.substr(0,2) == "vn")
			{
				float x, y, z;
				line >> garbage >> x >> y >> z;
				m_normalArray.push_back(D3DXVECTOR3(x, y, z));
			}
			else if (buffer.substr(0,2) == "vt")
			{
				float x, y;
				line >> garbage >> x >> y;
				m_texCoordArray.push_back(D3DXVECTOR2(x, 1-y));
			}
			else if (buffer.substr(0,1) == "v")
			{
				float x, y, z;
				line >> garbage >> x >> y >> z;
				m_positionArray.push_back(D3DXVECTOR3(x, y, z));
			}
			else if(buffer.substr(0,1) == "f")
			{
				UINT iPosition, iNormal, iTexCoord;
			
				for (UINT iFace = 0; iFace < 3; iFace++)
				{
					if(iFace == 0)
					{
						line.ignore();
						line >> iPosition;
					}
					else
					{
						line >> iPosition;
						
					}

					iPosition--;

					if('/' == line.peek())
					{
						line.ignore();

						if('/' != line.peek())
						{
							line >> iTexCoord;
							iTexCoord--;
						}
						if('/' == line.peek())
						{
							line.ignore();
							line >> iNormal;
							iNormal--;
						}
						p_mesh->AddVertex(&m_positionArray[iPosition], &m_texCoordArray[iTexCoord],  &m_normalArray[iNormal]);
					}
				}
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