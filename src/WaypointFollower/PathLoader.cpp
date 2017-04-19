#include "PathLoader.h"
#include <algorithm>
#include "CORELog.h"

Path * PathLoader::loadPath(std::string fileName, double speedScale, bool flipY, bool flipX, bool reversePath) {
	CORE::CORELog::logInfo("Loading File: " + fileName);
	std::vector<Waypoint> points;
    std::string line;
    std::string fileStarter = "/media/sda1/COREAutoPaths/";
    std::ifstream inFile(fileStarter + fileName);
    if(!inFile.is_open()){
    	CORE::CORELog::logWarning("Path " + fileStarter + " not found on USB!");
    	fileStarter = "/home/lvuser/COREAutoPaths/";
    	inFile.open(fileStarter + fileName);
    }
    if (inFile.is_open()) {
    		while (std::getline(inFile, line)) {
    			while (line.find('\r') != std::string::npos) {
    				line.erase(line.find('\r'), 1);
    			}
    			while (line.find('\n') != std::string::npos) {
    				line.erase(line.find('\n'), 1);
    			}
    			DataPoint p;
    			//std::size_t comma = line.find(',');
    			double * numToSet = &p.x;
    			while (line.find(',') != std::string::npos) {
    				std::size_t comma = line.find(',');
    				std::string data = line.substr(0, comma);
    				*numToSet = atof(data.c_str());
    				line.erase(0, comma + 1);
    				numToSet += 1;
    			}
    			if (line.find('"') != std::string::npos) {
    				while (line.find('"') != std::string::npos) {
    					line.erase(line.find('"'), 1);
    				}
    				p.event = line;
    			}
    			else {
    				*numToSet = atof(line.c_str());
    			}

    			Waypoint wp;
    			wp.position = Translation2d(p.x,p.y);
    			wp.speed = 100;
    			wp.event = p.event;
    			points.push_back(wp);
    		}

            if(!points.empty()){
//            	if (reversePath){
//            		std::reverse(points.begin(), points.end());
//            		std::cout << "Reversing Path" << std::endl;
//            	}
            	CORE::CORELog::logInfo(fileName + " was loaded");
            	std::cout << fileName << " has " << points.size() << " points" << std::endl;
            	for(auto i : points){
            		std::cout << i.position.getX() << " " << i.position.getY() << std::endl;
            	}
            	return new Path(points, flipY, flipX);
            }
            else{
            	CORE::CORELog::logError(fileName + " was empty!");
            	return new Path({Waypoint({-1,-1}, -1)}, flipY, flipX);
            }
    	}
    	else {
    		CORE::CORELog::logError("Could not find " + fileName + " in either directory");
    	}

    std::cout << "Failed to open: " << fileName << std::endl;
    return new Path({Waypoint({-1,-1}, -1)}, flipY, flipX);
}
