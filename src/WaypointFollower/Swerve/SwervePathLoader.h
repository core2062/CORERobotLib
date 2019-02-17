#pragma once

#include "SwervePath.h"
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
	static SwervePath LoadPath(string fileName, bool flip);
    static SwervePath LoadTextPath(std::string text, bool flip);
};
