#pragma once

#include "Path.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>

class PathLoader{
	static Path loadPath(std::string fileName, bool flipX = false, bool flipY = false);
};
