#pragma once

#include "TankPath.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>

using namespace std;

struct TankDataPoint {
	double x = 0;
	double y = 0;
	double speed = 100;
	double angle = -7;
	std::string event = "";
};

class TankPathLoader{
public:
	static TankPath * LoadPath(std::string fileName, double speedScale = 1.0, bool flipY = false, 
		bool flipX = false, bool reversePath = false);
};
