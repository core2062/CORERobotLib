#include "PathLoader.h"

Path PathLoader::loadPath(std::string fileName, bool flipX, bool flipY) {
	std::vector<Waypoint> points;
    std::string line;
    std::string fileStarter = "/home/lvuser/COREAutoPaths/";
    std::ifstream inFile(fileStarter + fileName);
    if (inFile.is_open()) {
        while (std::getline(inFile, line)) {
            Waypoint p;
            std::size_t comma = line.find(',');
            std::string data = line.substr(0, comma);
            p.position.setY(atof(data.c_str()));
            line.erase(0, comma+1);
            comma = line.find(',');
            data = line.substr(0, comma);
            p.position.setY(atof(data.c_str()));
            line.erase(0, comma+1);
            if (line.find(',') != std::string::npos) {
                comma = line.find(',');
                data = line.substr(0, comma);
                p.speed = atof(data.c_str());
                line.erase(0, comma+1);
                while (line.find('"') != std::string::npos) {
                    line.erase(line.find('"'), 1);
                }
                p.event = line;
            }
            else {
                p.speed = atof(line.c_str());
            }
            points.push_back(p);
        }
        if(!points.empty()){
        	return Path(points, flipX, flipY);
        }
        else{
        	std::cout << "File: " << fileName << " was empty!" << std::endl;
        	return Path(Waypoint({-1,-1}, -1), flipX, flipY);
        }
    }
    std::cout << "Failed to open: " << fileName << std::endl;
    return Path(Waypoint({-1,-1}, -1), flipX, flipY);
}
