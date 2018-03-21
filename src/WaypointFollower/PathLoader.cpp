#include "PathLoader.h"
#include "CORELogging/CORELog.h"

using namespace CORE;
using namespace std;

Path PathLoader::loadPath(string fileName, bool flipY, bool flipX) {
	CORELog::logInfo("Loading File: " + fileName);
    string fileStarter = "/home/lvuser/Paths/";
    ifstream inFile(fileStarter + fileName);
    if (inFile.is_open()) {
        string text((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        return loadTextPath(text);
    }
    cout << "Failed to open: " << fileName << endl;
    return Path({Waypoint({0, 0}, 0)}, flipY, flipX);
}

Path PathLoader::loadTextPath(string text) {
    vector<Waypoint> points;
    json json;
    try {
        json = json::parse(text);
    } catch (const std::exception& e) {
        CORELog::logError("Error parsing json path! " + string(e.what()));
        return Path({Waypoint({0, 0}, 0)});
    }

    //CORELog::logInfo("Json text contents:\n" + json.dump(4));
    try {
        for(int i = 0; i < json.size(); i++) {
            Waypoint waypoint(Translation2d(json[i]["x"].get<double>(), json[i]["y"].get<double>()), 100);
            if(json[i]["name"].get<string>() != "point") {
                waypoint.event = json[i]["name"].get<string>();
                waypoint.rotation = Rotation2d::fromRadians(json[i]["theta"].get<double>());
                waypoint.angleProvided = true;
            }
            points.push_back(waypoint);
        }
    } catch (const std::exception& e) {
        CORELog::logError("Error reading json path! " + string(e.what()));
        return Path({Waypoint({0, 0}, 0)});
    }

    if(!points.empty()){
        CORELog::logInfo("Path text was loaded");
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
        return Path({Waypoint({0, 0}, 0)});
    }
}
