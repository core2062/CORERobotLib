#include "COREData.h"

using namespace CORE;
using namespace nlohmann;

map<string, ICOREData*> COREDataManager::m_data;

string COREDataManager::GetJSON(bool onlyIfChanged) {
    string driverJsonData;
    string debugJsonData;
    for (auto data : m_data) {
        auto jsonKeyPair = data.second->GetJSON(onlyIfChanged);
        bool isDriverValue = data.second->IsDriverValue();
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
    CORELog::LogInfo(stuff);
    return stuff;
}

void COREDataManager::AddData(ICOREData *instance) {
    m_data.insert(pair<string,ICOREData *>(instance->GetName(), instance));
}

void COREDataManager::UpdateData(json jsonData) {
    for(json::iterator it = jsonData.begin(); it != jsonData.end(); ++it) {
        if(m_data.find(it.key()) == m_data.end()) {
            CORELog::LogWarning("Can not find key: \"" + it.key()
                                + "\" in registered COREData keys! Ignoring newly recived value from web dashboard");
            continue;
        }
//        m_data[it.key()] = it.value();
    }
}

void COREDataManager::CleanUp() {
    for(auto i = m_data.begin(); i != m_data.end(); i++){
        delete i->second;
    }
    m_data.clear();
}
