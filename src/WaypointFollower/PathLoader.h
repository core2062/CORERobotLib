#pragma once

#include "Path.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>

class PathLoader{
public:
	static Path * loadPath(std::string fileName, double speedScale = 1.0, bool flipY = false, bool flipX = false);
};
