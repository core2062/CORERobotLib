#include "PathLoader.h"
#include "CORELogging/CORELog.h"

using namespace CORE;
using namespace std;

Path PathLoader::loadPath(string fileName, double speedScale, bool flipY, bool flipX, bool reversePath) {
	CORELog::logInfo("Loading File: " + fileName);
	vector<Waypoint> points;
    string line;
    string fileStarter = "/media/sda1/COREAutoPaths/";
    ifstream inFile(fileStarter + fileName);
    if(!inFile.is_open()){
    	CORELog::logWarning("Path " + fileStarter + " not found on USB!");
    	inFile.close();
    	fileStarter = "/home/lvuser/Paths/";
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
            	CORELog::logInfo(fileName + " was loaded");
            	cout << fileName << " has " << points.size() << " points" << endl;
            	for(auto i : points){
            		cout << i.position.getX() << " " << i.position.getY() << endl;
            	}
            	return Path(points, flipY, flipX);
            }
            else{
            	CORELog::logError(fileName + " was empty!");
            	return Path({Waypoint({-1,-1}, -1)}, flipY, flipX);
            }
    	}
    	else {
    		CORELog::logError("Could not find " + fileName + " in either directory");
    	}

    cout << "Failed to open: " << fileName << endl;
    return Path({Waypoint({-1,-1}, -1)}, flipY, flipX);
}

Path PathLoader::loadTextPath(string text, double speedScale, bool flipY, bool flipX, bool reversePath) {
    vector<Waypoint> points;
    string line;
    istringstream input(text);
    while (getline(input, line)) {
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
        CORELog::logInfo("Text path was loaded");
        cout << "Has " << points.size() << " points" << endl;
        for(auto i : points){
            cout << i.position.getX() << " " << i.position.getY() << endl;
        }
        return Path(points, flipY, flipX);
    }
    else{
        CORELog::logError("Text path was empty!");
        return Path({Waypoint({-1,-1}, -1)}, flipY, flipX);
    }
}
