#include "PathLoader.h"
#include <algorithm>
#include "CORELogging/CORELog.h"

Path * PathLoader::LoadPath(std::string fileName, double speedScale, bool flipY, bool flipX, bool reversePath) {
	CORE::CORELog::LogInfo("Loading File: " + fileName);
	std::vector<Waypoint> points;
    std::string line;
    std::string fileStarter = "/media/sda1/COREAutoPaths/";
    std::ifstream inFile(fileStarter + fileName);
    if(!inFile.is_open()){
    	CORE::CORELog::LogWarning("Path " + fileStarter + " not found on USB!");
    	inFile.close();
    	fileStarter = "/home/lvuser/COREAutoPaths/";
    	inFile.open(fileStarter + fileName);
    }
    if (inFile.is_open()) {
    		while (getline(inFile, line)) {
    			while (line.find('\r') != string::npos) {
    				line.erase(line.find('\r'), 1);
    			}
    			while (line.find('\n') != string::npos) {
    				line.erase(line.find('\n'), 1);
    			}
    			DataPoint p;
    			//size_t comma = line.find(',');
    			double * numToSet = &p.x;
    			while (line.find(',') != string::npos) {
    				size_t comma = line.find(',');
    				string data = line.substr(0, comma);
    				*numToSet = atof(data.c_str());
    				line.erase(0, comma + 1);
    				numToSet += 1;
    			}
    			if (line.find('"') != string::npos) {
    				while (line.find('"') != string::npos) {
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
            	CORE::CORELog::LogInfo(fileName + " was loaded");
            	std::cout << fileName << " has " << points.size() << " points" << std::endl;
            	for(auto i : points){
            		cout << i.position.GetX() << " " << i.position.GetY() << endl;
            	}
            	return new Path(points, flipY, flipX);
            }
            else{
            	CORE::CORELog::LogError(fileName + " was empty!");
            	return new Path({Waypoint({-1,-1}, -1)}, flipY, flipX);
            }
    	}
    	else {
    		CORE::CORELog::LogError("Could not find " + fileName + " in either directory");
    	}

    cout << "Failed to open: " << fileName << endl;
    return new Path({Waypoint({-1,-1}, -1)}, flipY, flipX);
}