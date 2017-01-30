#include "COREConstant.h"

using namespace CORE;

template <class T>
COREConstant<T>::COREConstant(string name, T defaultValue) : value(defaultValue) {
    m_name = "INVALID TYPE";
    Preferences::GetInstance()->PutString(m_name, "INVALID TYPE SPECIFIED FOR: " + name);
    CORELog::logError("Invalid COREConstant type specified with name: " + name);
}

COREConstant<double>::COREConstant(string name, double defaultValue) : value(defaultValue) {
    m_name = name;
    Preferences::GetInstance()->PutDouble(m_name, defaultValue);
    COREConstantsManager::addConstant(this);
}

COREConstant<string>::COREConstant(string name, string defaultValue) : value(defaultValue) {
    m_name = name;
    Preferences::GetInstance()->PutString(m_name, defaultValue);
    COREConstantsManager::addConstant(this);
}

COREConstant<bool>::COREConstant(string name, bool defaultValue) : value(defaultValue) {
    m_name = name;
    Preferences::GetInstance()->PutBoolean(m_name, defaultValue);
    COREConstantsManager::addConstant(this);
}

template <class T>
string COREConstant<T>::getName() {
    return m_name;
}

void COREConstant<double>::updateConstant(Preferences * prefs) {
    prefs->GetDouble(m_name);
}

void COREConstant<string>::updateConstant(Preferences * prefs) {
    prefs->GetString(m_name);
}

void COREConstant<bool>::updateConstant(Preferences * prefs) {
    prefs->GetDouble(m_name);
}

vector<ICOREConstant*> COREConstantsManager::m_constants;

void COREConstantsManager::robotInit() {
    m_prefs = Preferences::GetInstance();
}

void COREConstantsManager::updateConstants() {
    for(auto constant : m_constants) {
        constant->updateConstant(m_prefs);
    }
}

void COREConstantsManager::addConstant(ICOREConstant * instance) {
    m_constants.push_back(instance);
}