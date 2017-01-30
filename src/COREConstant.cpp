#include "COREConstant.h"

using namespace CORE;

/*template <class T>
COREConstant<T>::COREConstant(string name, T defaultValue) : m_value(defaultValue) {
    m_name = "INVALID TYPE";
    Preferences::GetInstance()->PutString(m_name, "INVALID TYPE SPECIFIED FOR: " + name);
    CORELog::logError("Invalid COREConstant type specified with name: " + name);
}

template <class T>
COREConstant<double>::COREConstant(string name, double defaultValue) : m_value(defaultValue) {
    m_name = name;
    Preferences::GetInstance()->PutDouble(m_name, defaultValue);
    COREConstantsManager::addConstant(this);
}

template <class T>
COREConstant<string>::COREConstant(string name, string defaultValue) : m_value{defaultValue} {
    m_name = name;
    Preferences::GetInstance()->PutString(m_name, defaultValue);
    COREConstantsManager::addConstant(this);
}

template <class T>
COREConstant<bool>::COREConstant(string name, bool defaultValue) : m_value{defaultValue} {
    m_name = name;
    Preferences::GetInstance()->PutBoolean(m_name, defaultValue);
    COREConstantsManager::addConstant(this);
}*/

template <class T>
T COREConstant<T>::Get() {
    return m_value;
}

template <class T>
string COREConstant<T>::getName() {
    return m_name;
}

void COREConstant<double>::updateConstant() {
    Preferences::GetInstance()->GetDouble(m_name);
}

void COREConstant<string>::updateConstant() {
    Preferences::GetInstance()->GetString(m_name);
}

void COREConstant<bool>::updateConstant() {
    Preferences::GetInstance()->GetDouble(m_name);
}

vector<ICOREConstant*> COREConstantsManager::m_constants;

void COREConstantsManager::robotInit() {

}

void COREConstantsManager::updateConstants() {
    for(auto constant : m_constants) {
        constant->updateConstant();
    }
}

void COREConstantsManager::addConstant(ICOREConstant * instance) {
    m_constants.push_back(instance);
}

