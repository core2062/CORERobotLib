#include "PathLoader.h"
#include "CORELogging/CORELog.h"

using namespace CORE;
using namespace std;

Path PathLoader::loadPath(string fileName, bool flip) {
	CORELog::logInfo("Loading File: " + fileName);
    string fileStarter = "/home/lvuser/Paths/";
    ifstream inFile(fileStarter + fileName);
    if (inFile.is_open()) {
        string text((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        return loadTextPath(text, flip);
    }
    cout << "Failed to open: " << fileName << endl;
    return Path({Waypoint({0, 0}, Rotation2d(), 0)});
}

Path PathLoader::loadTextPath(string text, bool flip) {
    vector<Waypoint> points;
    json json;
    try {
        json = json::parse(text);
    } catch (const std::exception& e) {
        CORELog::logError("Error parsing json path! " + string(e.what()));
        return Path({Waypoint({0, 0}, Rotation2d(), 0)});
    }

    //CORELog::logInfo("Json text contents:\n" + json.dump(4));
    try {
        for (auto point : json) {
            Waypoint waypoint({point["x"].get<double>(), point["y"].get<double>()},
                              Rotation2d::fromRadians(point["theta"].get<double>()), 100);
            if(flip) {
                waypoint.position = waypoint.position.flipX();
                waypoint.rotation = waypoint.rotation.inverse();
            }
            if(point["name"].get<string>() != "point") {
                waypoint.event = point["name"].get<string>();
            }
            points.push_back(waypoint);
        }
    } catch (const std::exception& e) {
        CORELog::logError("Error reading json path! " + string(e.what()));
        return Path({Waypoint({0, 0}, Rotation2d(), 0)});
    }

    if(!points.empty()){
        CORELog::logInfo("Path text was loaded successfully");
        cout << "Path text has " << points.size() << " points" << endl;
//        for(auto i : points){
//            cout << i.position.getX() << " " << i.position.getY();
//            if(i.angleProvided) {
//                cout << " " << i.rotation.getRadians();
//            }
//            cout << endl;
//        }
        return Path(points);
    } else{
        CORELog::logError("Loaded path text was empty!");
        return Path({Waypoint({0, 0}, Rotation2d(), 0)});
    }
}
