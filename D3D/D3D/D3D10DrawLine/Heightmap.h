#include "stdafx.h"
#include <fstream>
class HeightMap
{
public:
	HeightMap();
	~HeightMap();
	void loadRAW(int m,
		int n, 
		const string& filename, 
		float heightScale, 
		float heightOffset);
	float GetY(int x, int z);
	float getHeight(float x, float z);
private:
	string mHeightMapFilename;
	float mHeightScale;
	float mHeightOffset;
	float* mHeightMap;
	void filter3x3();
	float sampleHeight3x3(int i, int j);
	bool InBounds(int m, int n);
	int size;
	int width;
	int height;
};