#include "PathLoader.h"
#include <algorithm>
#include "CORELog.h"

Path * PathLoader::loadPath(string fileName, double speedScale, bool flipY, bool flipX, bool reversePath) {
	cout << "Loading File: " << fileName << endl;
	vector<Waypoint> points;
    string line;
    string fileStarter = "/media/sda1/COREAutoPaths/";
    ifstream inFile(fileStarter + fileName);
    if(!inFile.is_open()){
    	CORE::CORELog::logWarning("Path " + fileStarter + " not found on USB!");
    	inFile.close();
    	fileStarter = "/home/lvuser/COREAutoPaths/";
    	ifstream inFile(fileStarter + fileName);
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
//            	if (reversePath){
//            		reverse(points.begin(), points.end());
//            		cout << "Reversing Path" << endl;
//            	}

            	cout << fileName << " has " << points.size() << " points" << endl;
            	for(auto i : points){
            		cout << i.position.getX() << " " << i.position.getY() << endl;
            	}
            	return new Path(points, flipY, flipX);
            }
            else{
            	cout << "File: " << fileName << " was empty!" << endl;
            	return new Path({Waypoint({-1,-1}, -1)}, flipY, flipX);
            }
    	}
    	else {
    		cout << "Could not find file: " << fileStarter + fileName << endl;
    	}

    cout << "Failed to open: " << fileName << endl;
    return new Path({Waypoint({-1,-1}, -1)}, flipY, flipX);

    if (!inFile.is_open()){
    	 fileStarter = "ftp://roborio-2062-frc.local/media/sda1";
    	//TODO Add anything more that is needed
    }
}
