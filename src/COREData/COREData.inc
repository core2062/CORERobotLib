#pragma once

#include <string>
#include "COREData.h"

using namespace std;

namespace CORE {
    template<class T>
    COREData<T>::COREData(const char * name, bool showOnDriverDashboard) : m_value{}, m_valueText("null") {
        m_pointerToSource = nullptr;
        m_sourceFunction = nullptr;
        m_value = 0;
        m_name = name;
        m_showOnDriverDashboard = showOnDriverDashboard;
        COREDataManager::addData(this);
    }

    template<class T>
    COREData<T>::COREData(const char *group, const char *name, bool showOnDriverDashboard) :
            m_value{}, m_valueText("null") {
        m_pointerToSource = nullptr;
        m_sourceFunction = nullptr;
        if(!string(group).empty()) {
            m_name = string(group) + "\\" + name;
        } else {
            m_name = name;
        }
        m_showOnDriverDashboard = showOnDriverDashboard;
        COREDataManager::addData(this);
    }

    template<class T>
    COREData<T>::COREData(const char* name, T defaultValue, bool showOnDriverDashboard) :
            m_value{}, m_valueText("null") {
        m_pointerToSource = nullptr;
        m_sourceFunction = nullptr;
        COREData("", name, showOnDriverDashboard);
    }

    template<class T>
    COREData<T>::COREData(const char* groupName, const char* name, T defaultValue, bool showOnDriverDashboard) :
            m_value{}, m_valueText("null") {
        m_pointerToSource = nullptr;
        m_sourceFunction = nullptr;
        COREData(groupName, name, showOnDriverDashboard);
    }

    template<class T>
    COREData<T>::COREData(const char* name, T *pointerToSource, bool showOnDriverDashboard) :
            m_value{}, m_valueText("null") {
        m_pointerToSource = pointerToSource;
        m_sourceFunction = nullptr;
        COREData("", name, showOnDriverDashboard);
    }

    template<class T>
    COREData<T>::COREData(const char* groupName, const char* name, T *pointerToSource, bool showOnDriverDashboard) :
            m_value{}, m_valueText("null") {
        m_pointerToSource = pointerToSource;
        m_sourceFunction = nullptr;
        COREData(groupName, name, showOnDriverDashboard);
    }

    template<class T>
    COREData<T>::COREData(const char* name, function<T()> sourceFunction, bool showOnDriverDashboard) :
            m_value{}, m_valueText("null") {
        m_pointerToSource = nullptr;
        m_sourceFunction = sourceFunction;
        COREData("", name, showOnDriverDashboard);
    }

    template<class T>
    COREData<T>::COREData(const char* groupName, const char* name, function<T()> sourceFunction,
                          bool showOnDriverDashboard) : m_value{}, m_valueText("null") {
        m_pointerToSource = nullptr;
        m_sourceFunction = sourceFunction;
        COREData(groupName, name, showOnDriverDashboard);
    }

    template<class T>
    void COREData<T>::Set(string value) {
        m_valueText = "\"" + value + "\"";
        m_value = value;
    }

    template<class T>
    void COREData<T>::Set(double value) {
        m_valueText = to_string(value);
        m_value = value;
    }

    template<class T>
    void COREData<T>::Set(int value) {
        m_valueText = to_string(value);
        m_value = value;
    }

    template<class T>
    void COREData<T>::Set(bool value) {
        m_valueText = value ? "true" : "false";
        m_value = value;
    }

    template<class T>
    void COREData<T>::setSource(T *pointerToSource) {
        m_pointerToSource = pointerToSource;
    }

    template<class T>
    void COREData<T>::setSource(function<T()> sourceFunction) {
        m_sourceFunction = sourceFunction;
    }

    template<class T>
    T COREData<T>::Get() {
        return m_value;
    }

    template<class T>
    string COREData<T>::getName() {
        return m_name;
    }

    template<class T>
    string COREData<T>::getJSON(bool onlyIfChanged) {
        if(m_pointerToSource) {
            Set(*m_pointerToSource);
        } else if (m_sourceFunction) {
            Set(m_sourceFunction());
        }

        string data = "";
        if(!onlyIfChanged || m_lastValue != m_value) {
            data = "\"" + m_name + "\"" + ":" + m_valueText;
        }
        m_lastValue = m_value;
        return data;
    }

    template<class T>
    void COREData<T>::setDriverValue(bool driverValue) {
        m_showOnDriverDashboard = driverValue;
    }

    template<class T>
    bool COREData<T>::isDriverValue() {
        return m_showOnDriverDashboard;
    }
}
