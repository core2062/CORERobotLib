#pragma once

#include <string>
#include <memory>
#include <map>
#include <json.hpp>
#include <typeinfo>
#include <string>

#include "CORELogging/CORELog.h"

using namespace std;

namespace CORE {
    class ICOREData {
        friend class COREDataManager;
    public:
        virtual ~ICOREData() = default;
        virtual string GetName() = 0;
        virtual string GetJSON(bool onlyIfChanged = true) = 0;
        virtual bool IsDriverValue() = 0;
    private:
        string m_name;
    };

    class COREDataManager {
    public:
        static void AddData(ICOREData* instance);
        static void UpdateData(nlohmann::json jsonData);
        static void CleanUp();
        static string GetJSON(bool onlyIfChanged = true);
    private:
        static map<string, ICOREData*> m_data;
    };

    template<class T>
    class COREData : public ICOREData {
    public:
        COREData(const char* name, bool showOnDriverDashboard = false);
        COREData(const char* group, const char* name, bool showOnDriverDashboard = false);
        COREData(const char* name, T defaultValue, bool showOnDriverDashboard = false);
        COREData(const char* groupName, const char* name, T defaultValue, bool showOnDriverDashboard = false);
        COREData(const char* name, T * pointerToSource, bool showOnDriverDashboard = false);
        COREData(const char* groupName, const char* name, T * pointerToSource, bool showOnDriverDashboard = false);
        COREData(const char* name, function<T()> sourceFunction, bool showOnDriverDashboard = false);
        COREData(const char* groupName, const char* name, function<T()> sourceFunction, bool showOnDriverDashboard = false);
        void Set(double value);
        void Set(int value);
        void Set(bool value);
        void Set(string value);
        void SetSource(T * pointerToSource);
        void SetSource(function<T()> sourceFunction);
        T Get();
        string GetJSON(bool onlyIfChanged = true) override;
        string GetName() override;
        void SetDriverValue(bool driverValue = true);
        bool IsDriverValue() override;
    private:
        string m_name, m_valueText;
        bool m_showOnDriverDashboard;
        T m_value;
        T m_lastValue;
        T *m_pointerToSource;
        function<T()> m_sourceFunction;
    };
}

#include "COREData.inc"