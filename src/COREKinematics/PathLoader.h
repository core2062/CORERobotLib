#pragma once

#include "COREUtilities/COREVector.h"
#include "Path.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>

using namespace std;

struct DataPoint {
    double x = 0;
    double y = 0;
    double speed = 100;
    double angle = -7;
    std::string event = "";
};

class PathLoader {
public:
    static Path *loadPath(std::string fileName, double speedScale = 1.0, bool flipY = false, bool flipX = false,
                          bool reversePath = false);
};
