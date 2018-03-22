#pragma once

#include "Path.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include "json.hpp"

using namespace std;
using namespace nlohmann;

struct DataPoint {
	double x = 0;
	double y = 0;
	double speed = 100;
	double angle = -7;
	std::string event = "";
};

class PathLoader{
public:
	static Path loadPath(string fileName, bool flip);
    static Path loadTextPath(std::string text, bool flip);
};
