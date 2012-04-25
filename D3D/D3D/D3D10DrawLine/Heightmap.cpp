#include "Heightmap.h"

HeightMap::HeightMap()
{
	mHeightOffset		= NULL;
	mHeightScale		= NULL;
	
}

HeightMap::~HeightMap()
{
	mHeightOffset		= NULL;
	mHeightScale		= NULL;
}

void HeightMap::loadRAW(int m,
						int n, 
						const string& filename, 
						float heightScale, 
						float heightOffset)
{
	mHeightMapFilename	= filename;
	mHeightScale		= heightScale;
	mHeightOffset		= heightOffset;
	size = m*n;
	height = m;
	width = n;
	mHeightMap			= new float[size];
	//A height for each vertex
	unsigned char* vertexHeight = new unsigned char[size];

	//Open file of reading
	ifstream fin;
	fin.open(filename.c_str(), ios_base::binary);
	if(!fin)
		return;
	//Read all the RAW bytes in once
	fin.read((char *)&vertexHeight[0], (streamsize)size*4);
	fin.close();

	for(int i = 0; i < size; i++)
	{
		mHeightMap[i] = 0.0f;
	}

	//Scale and Offset the heights
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int k = i*height + j;
			 
			mHeightMap[k] = (float) vertexHeight[k] * mHeightScale + mHeightOffset;
		}
	}
	SAFE_DELETE_ARRAY(vertexHeight);
	delete [] vertexHeight;
	filter3x3();
}

void HeightMap::filter3x3()
{
	float* FilteredHeightMap = new float[size];	
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			int k = i*width + j;
			FilteredHeightMap[k] = sampleHeight3x3(i,j);
		}
	}
	mHeightMap = FilteredHeightMap;
}

float HeightMap::sampleHeight3x3(int i, int j)
{
	float average = 0.0f;
	float sample = 0.0f;

	for (int m = i-1; m <= i+1; m++)
	{
		for (int n = j-1; n <= j+1; n++)
		{
			if (InBounds(m,n))
			{
				int k = m*width + n;
				average += mHeightMap[k];
				sample += 1.0f;
			}
		}
	}
	return average/sample;
}

bool HeightMap::InBounds(int m, int n)
{
	if(m >=0 && m < width && n >=0 && n < width)
	{
		return true;
	}
	else
		return false;
}
float HeightMap::GetY(int i, int j)
{
	int k = i*width + j;
	return mHeightMap[k];
}
float HeightMap::getHeight(float x, float z)
{
	//// Transform from terrain local space to "cell" space.
	x += width/2;
	z += height/2;

	//// Get the row and column we are in.
	int iX, iZ;
	iX = (int)x;
	iZ = (int)z;

	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = mHeightMap[iX*height + iZ];
	float B = mHeightMap[iX*height + (iZ + 1)];
	float C = mHeightMap[(iX+1)*height + iZ];
	float D = mHeightMap[(iX+1)*height + (iZ + 1)];

	// Where we are relative to the cell.
	float s = x - iX;
	float t = z - iZ;

	
	if( s + t <= 1.0f)
	{
		float uy = B - A;
		float vy = C - A;
		return A + s*uy + t*vy;
	}
	else 
	{
		float uy = C - D;
		float vy = B - D;
		return D + (1.0f-s)*uy + (1.0f-t)*vy;
	}
}
