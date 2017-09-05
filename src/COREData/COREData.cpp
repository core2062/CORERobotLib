#include "COREData.h"

using namespace CORE;
using namespace nlohmann;

map<string, ICOREData*> COREDataManager::m_data;

string COREDataManager::getJSON(bool onlyIfChanged) {
    string driverJsonData;
    string debugJsonData;
    for (auto data : m_data) {
        auto jsonKeyPair = data.second->getJSON(onlyIfChanged);
        bool isDriverValue = data.second->isDriverValue();
        if(jsonKeyPair.empty()) {
            continue;
        }
        if(isDriverValue) {
            if(!driverJsonData.empty()) {
                driverJsonData += ",";
            }
            driverJsonData += jsonKeyPair;
        }
        if(!debugJsonData.empty()) {
            debugJsonData += ",";
        }
        debugJsonData += jsonKeyPair;
    }
    if(!driverJsonData.empty()) {
        driverJsonData = "\"driver\":{" + driverJsonData;
        driverJsonData += "},";
    }
    if(!debugJsonData.empty()) {
        debugJsonData = "\"debug\":{" + debugJsonData;
        debugJsonData += "}";
    }
    string stuff = "{" + driverJsonData + debugJsonData + "}";
    CORELog::logInfo(stuff);
    return stuff;
}

void COREDataManager::addData(ICOREData *instance) {
    m_data.insert(pair<string,ICOREData *>(instance->getName(), instance));
}

void COREDataManager::updateData(json jsonData) {
    for(json::iterator it = jsonData.begin(); it != jsonData.end(); ++it) {
        if(m_data.find(it.key()) == m_data.end()) {
            CORELog::logWarning("Can not find key: \"" + it.key()
                                + "\" in registered COREData keys! Ignoring newly recived value from web dashboard");
            continue;
        }
//        m_data[it.key()] = it.value();
    }
}

void COREDataManager::cleanUp() {
    for(auto i = m_data.begin(); i != m_data.end(); i++){
        delete i->second;
    }
    m_data.clear();
}
