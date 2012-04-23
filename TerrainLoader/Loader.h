#ifndef LOADER_H
#define LOADER_H
#include <string>
#include "d3dApp.h"

using namespace std;

class Loader
{
public:
	virtual void loadFromFile(string pFilename) = 0;
};

#endif