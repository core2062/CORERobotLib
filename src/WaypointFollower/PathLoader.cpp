#include "PathLoader.h"

Path * PathLoader::loadPath(std::string fileName, double speedScale, bool flipY, bool flipX) {
	std::cout << "Loading File: " << fileName << std::endl;
	std::vector<Waypoint> points;
    std::string line;
    std::string fileStarter = "/home/lvuser/COREAutoPaths/";
    std::ifstream inFile(fileStarter + fileName);
    if (inFile.is_open()) {
        while (std::getline(inFile, line)) {
            while (line.find('\r') != std::string::npos) {
                line.erase(line.find('\r'), 1);
            }
            while (line.find('\n') != std::string::npos) {
                line.erase(line.find('\n'), 1);
            }
            Waypoint p;
            std::size_t comma = line.find(',');
            std::string data = line.substr(0, comma);
            p.position.setX(atof(data.c_str()));
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
                p.speed *= speedScale;
            }
            points.push_back(p);
        }
        if(!points.empty()){
        	std::cout << fileName << " has " << points.size() << " points" << std::endl;
        	for(auto i : points){
        		std::cout << i.position.getX() << " " << i.position.getY() << std::endl;
        	}
        	return new Path(points, flipY, flipX);
        }
        else{
        	std::cout << "File: " << fileName << " was empty!" << std::endl;
        	return new Path({Waypoint({-1,-1}, -1)}, flipY, flipX);
        }
    }
    std::cout << "Failed to open: " << fileName << std::endl;
    return new Path({Waypoint({-1,-1}, -1)}, flipY, flipX);
}