#include "WorldLoader.h"

WorldLoader::WorldLoader(ID3D10Device* p_device)
{
	m_device = p_device;
	
	//Create all factories
	m_wallFactory = new WallFactory();
	m_pacmanFactory = new PacmanFactory();
	m_enemyFactory = new EnemyFactory();
	m_candyFactory = new CandyFactory();
	m_powerUpFactory = new PowerUpFactory();
	m_cherryFactory = new CherryFactory();

	//DEBUG variables
	nrCandy = nrCherry = nrPowerUp = nrEnemies = nrPlayer = nrWalls = 0;
}
WorldLoader::~WorldLoader()
{
	m_device = NULL;
	m_fileColorData.clear();
	m_fileColorData.shrink_to_fit();
}

void WorldLoader::loadFromFile(	string p_filename, UINT p_terrainWidth, 
								UINT p_terrainHeight, UINT p_terrainY,
								vector<Object*>& p_objects)
{
	//Set Variables
	m_terrainHeight = p_terrainHeight;
	m_terrainWidth = p_terrainWidth;
	m_terrainY = p_terrainY;

	//Image Info
	D3DX10_IMAGE_INFO imageInfo;
	HRESULT imageInfoHR;

	//Get image size
	D3DX10GetImageInfoFromFile(
		(LPCSTR)p_filename.c_str(),
		NULL,
		&imageInfo,
		&imageInfoHR
		);

	//If file found
	if(imageInfoHR == S_OK)
	{
		ID3D10Texture2D* spTexture = 0;
		D3D10_MAPPED_TEXTURE2D mapped;

		m_width = imageInfo.Width;
		m_height = imageInfo.Height;

		//Calculate terrain scale (Desired Size / Map Size )
		m_terrainScale.x = (float)m_terrainWidth / m_width;
		m_terrainScale.y = (float)m_terrainHeight / m_height;

		//Image Load Info
		D3DX10_IMAGE_LOAD_INFO  loadInfo;
		loadInfo.Width          = D3DX10_FROM_FILE;
		loadInfo.Height         = D3DX10_FROM_FILE;
		loadInfo.Depth          = D3DX10_FROM_FILE;
		loadInfo.FirstMipLevel  = 0;
		loadInfo.MipLevels      = D3DX10_FROM_FILE;
		loadInfo.Usage          = D3D10_USAGE_STAGING;
		loadInfo.BindFlags      = 0;
		//loadInfo.CpuAccessFlags = D3D10_CPU_ACCESS_WRITE | D3D10_CPU_ACCESS_READ;
		loadInfo.CpuAccessFlags = D3D10_CPU_ACCESS_READ;
		loadInfo.MiscFlags      = 0;
		loadInfo.Format         = DXGI_FORMAT_R8G8B8A8_UNORM;
		loadInfo.Filter         = D3DX10_FILTER_NONE;
		loadInfo.MipFilter      = D3DX10_FILTER_NONE;
		loadInfo.pSrcInfo       = 0;

		D3DX10CreateTextureFromFileA(
				m_device,
				(LPCSTR)p_filename.c_str(),
				&loadInfo,
				NULL,
				(ID3D10Resource**)&spTexture,
				NULL
				);
		//Allocate memory
		ZeroMemory(&mapped, sizeof(D3D10_MAPPED_TEXTURE2D));

		//Get pointer to data
		spTexture->Map(0, D3D10_MAP_READ, 0, &mapped);

		UCHAR* pData = (UCHAR*)mapped.pData;

		//Save all Colors to vector
		for (UINT i = 0; i < m_height; i++)
		{
			UINT rowStart = i * mapped.RowPitch;
			for (UINT j = 0; j < m_width; j++)
			{
				UINT colStart = j * 4;
				Color color;
				color.r = (int)pData[rowStart + colStart + 0];
				color.g = (int)pData[rowStart + colStart + 1];
				color.b = (int)pData[rowStart + colStart + 2];
				color.a = (int)pData[rowStart + colStart + 3];
				checkColorRules((float)i, (float)j, &color, p_objects);
				m_fileColorData.push_back(color);
			}
		}
		//ShrinkToFit
		m_fileColorData.shrink_to_fit();

		//Release pointer
		spTexture->Unmap( D3D10CalcSubresource(0, 0, 1) );

		//Release Texture and pData
		SAFE_RELEASE(spTexture);
		pData = NULL;

		findCorners();
	}
	else{}
}

void WorldLoader::checkColorRules(float p_x, float p_y, Color* p_color, vector<Object*>& p_objects)
{
	//Calculate world coordinates
	D3DXVECTOR3 position = D3DXVECTOR3(p_x * m_terrainScale.x, (float)m_terrainY, p_y * m_terrainScale.y);

	//If it's a Wall (0,0,255)
	if(p_color->r == 0 && p_color->g == 0 && p_color->b == 255)
	{
		p_objects.push_back(m_wallFactory->createObjectInstance(m_device, position, m_terrainScale));
		nrWalls++;
	}
	//If it's a Candy (255,255,0)
	else if(p_color->r == 255 && p_color->g == 255 && p_color->b == 0)
	{
		p_objects.push_back(m_candyFactory->createObjectInstance());
		nrCandy++;
	}
	//If it's a PowerUp (255,0,0)
	else if(p_color->r == 255 && p_color->g == 0 && p_color->b == 0)
	{
		p_objects.push_back(m_powerUpFactory->createObjectInstance());
		nrPowerUp++;
	}
	//If it's player spawn (255,255,255)
	else if(p_color->r == 255 && p_color->g == 255 && p_color->b == 255)
	{
		p_objects.push_back(m_pacmanFactory->createObjectInstance());
		nrPlayer++;
	}
	//If it's Enemy spawn (255,0,255)
	else if(p_color->r == 255 && p_color->g == 0 && p_color->b == 255)
	{
		p_objects.push_back(m_enemyFactory->createObjectInstance());
		nrEnemies++;
	}
	//If it's Cherry spawn (0,255,0)
	else if(p_color->r == 0 && p_color->g == 255 && p_color->b == 0)
	{
		p_objects.push_back(m_cherryFactory->createObjectInstance());
		nrCherry++;
	}
	//Else undefined Rule
	else{ }
}

void WorldLoader::findCorners()
{
	//Load all corners to a vector
	vector<D3DXVECTOR2> corners;

	for(UINT i = 0; i < m_height; i++)
	{
		for(UINT j = 0; j < m_width; j++)
		{
			//Check for Corners with alpha value 127.5 as int 128 50%
			if(isCorner(j, i, Color(0,0,0,128)))
				corners.push_back(D3DXVECTOR2((float)j,(float)i));
		}
	}
}

UINT WorldLoader::nrOfNeighborsWithSameColor(int p_x, int p_y, Color p_color)
{
	UINT nrNeightbors = 0;

	//Checks if the current texel is correct color
	if(m_fileColorData.at( (m_width * p_y) + p_x).a == p_color.a)
	{
		//Checks the Texel to the Left
		if( p_x - 1 >= 0)
			if(m_fileColorData.at( (m_width * p_y) + p_x - 1 ).a == p_color.a)
				nrNeightbors++;
		//Checks the Texel to the Right
		if( p_x + 1 < (int)m_width)
			if(m_fileColorData.at( (m_width * p_y) + p_x + 1 ).a == p_color.a)
				nrNeightbors++;
		//Checks the Texel Above
		if( p_y - 1 >= 0)
			if(m_fileColorData.at( (m_width * ( p_y - 1 )) + p_x ).a == p_color.a)
				nrNeightbors++;
		//Checks the Texel Below
		if( p_y + 1 < (int)m_height)
			if(m_fileColorData.at( (m_width * ( p_y + 1 )) + p_x ).a == p_color.a)
				nrNeightbors++;
	}
	return nrNeightbors;
}

bool WorldLoader::isCorner(int p_x, int p_y, Color p_color)
{
	int nrNeighbors = nrOfNeighborsWithSameColor(p_x, p_y, p_color);
	if( nrNeighbors > 1) // 0 If you want end points aswell 1 if you dont want them
	{
		//2 nrNeighbors doesnt mean it's a corner
		if(nrNeighbors != 2)
		{
			return true;
		}
		else
		{
			if(p_x - 1 >= 0 && p_x + 1 < (int)m_width)
				//Check if texel got neighbor on the right and left
				if(	m_fileColorData.at( (m_width * p_y) + p_x + 1 ).a == p_color.a &&
					m_fileColorData.at( (m_width *  p_y) + p_x - 1 ).a == p_color.a)
					return false;

			if(p_y - 1 >= 0 && p_y + 1 < (int)m_height)
				//Check if texel got neighbor above and below
				if( m_fileColorData.at( (m_width * (p_y + 1)) + p_x ).a == p_color.a &&
					m_fileColorData.at( (m_width *  (p_y - 1)) + p_x ).a == p_color.a)
					return false;

			//The texel have corner
			return true;
		}
	}
	else
		return false;
}

D3DXVECTOR2* WorldLoader::getTerrainScale()
{
	return &m_terrainScale;
}