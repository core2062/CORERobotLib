#pragma once

#include <string>
#include <memory>
#include <vector>

#include "CORELogging/CORELog.h"
#ifndef NOT_REAL
#include <Preferences.h>
#endif

using namespace std;

namespace CORE {
    class ICOREConstant {
        friend class COREConstantsManager;
    public:
        virtual ~ICOREConstant() {}
    private:
        virtual string GetName() = 0;
        virtual void UpdateConstant() = 0;
    };

    class COREConstantsManager {
    public:
        static void UpdateConstants();
        static void AddConstant(ICOREConstant* instance);
        static void CleanUp();
    private:
        static vector<ICOREConstant*> m_constants;
        static string m_defaultConstantsFile; //TODO: Put constants to CSV file
    };

    template<class T>
    class COREConstant : public ICOREConstant {
    public:
        COREConstant(string name) {
            m_name = name;
            if(!Preferences::GetInstance()->ContainsKey(m_name)) {
                Preferences::GetInstance()->PutString(m_name, "INVALID TYPE SPECIFIED FOR: " + name);
            }
            UpdateConstant();
            COREConstantsManager::AddConstant(this);
        }

        COREConstant(string name, T defaultValue) : m_value(defaultValue) {
            m_name = name;
#ifndef NOT_REAL
            Preferences::GetInstance()->PutString(m_name, "INVALID TYPE SPECIFIED FOR: " + name);
            m_value = defaultValue;
#endif
            CORELog::LogError("Invalid COREConstant type specified with name: " + name);
        }

        T Get() {
            return m_value;
        }

        void Set(T value) {
            m_value = value;
            Preferences::GetInstance()->PutString(m_name, m_value);
        }

    private:
        string GetName() {
            return m_name;
        }

        void UpdateConstant() {
            m_value = "INVALID TYPE SPECIFIED";
        }

        string m_name;
        T m_value;
    };

    template<>
    class COREConstant<double> : public ICOREConstant {
    public:
        COREConstant(string name) {
            m_name = name;
            if(!Preferences::GetInstance()->ContainsKey(m_name)) {
                Preferences::GetInstance()->PutDouble(m_name, 0);
            }
            UpdateConstant();
            COREConstantsManager::AddConstant(this);
        }

        COREConstant(string name, double defaultValue) {
            m_name = name;
#ifndef NOT_REAL
            Preferences::GetInstance()->PutDouble(m_name, defaultValue);
            m_value = defaultValue;
#endif
            COREConstantsManager::AddConstant(this);
        }

        double Get() {
            return m_value;
        }

        void Set(double value) {
            m_value = value;
            Preferences::GetInstance()->PutDouble(m_name, m_value);
        }

    private:
        string GetName() {
            return m_name;
        }

        void UpdateConstant() {
#ifndef NOT_REAL
            m_value = Preferences::GetInstance()->GetDouble(m_name);
#endif
        }

        string m_name;
        double m_value;
    };

    template<>
    class COREConstant<string> : public ICOREConstant {
    public:
        COREConstant(string name) {
            m_name = name;
            if(!Preferences::GetInstance()->ContainsKey(m_name)) {
                Preferences::GetInstance()->PutString(m_name, "null");
            }
            UpdateConstant();
            COREConstantsManager::AddConstant(this);
        }

        COREConstant(string name, string defaultValue) {
            m_name = name;
#ifndef NOT_REAL
            Preferences::GetInstance()->PutString(m_name, defaultValue);
            m_value = defaultValue;
#endif
            COREConstantsManager::AddConstant(this);
        }

        string Get() {
            return m_value;
        }

        void Set(string value) {
            m_value = value;
            Preferences::GetInstance()->PutString(m_name, m_value);
        }

    private:
        string GetName() {
            return m_name;
        }

        void UpdateConstant() {
#ifndef NOT_REAL
            m_value = Preferences::GetInstance()->GetString(m_name);
#endif
        }

        string m_name;
        string m_value;
    };

    template<>
    class COREConstant<bool> : public ICOREConstant {
    public:
        COREConstant(string name) {
            m_name = name;
            if(!Preferences::GetInstance()->ContainsKey(m_name)) {
                Preferences::GetInstance()->PutBoolean(m_name, false);
            }
            UpdateConstant();
            COREConstantsManager::AddConstant(this);
        }

        COREConstant(string name, bool defaultValue) {
            m_name = name;
#ifndef NOT_REAL
            Preferences::GetInstance()->PutBoolean(m_name, defaultValue);
            m_value = defaultValue;
#endif
            COREConstantsManager::AddConstant(this);
        }

        bool Get() {
            return m_value;
        }

        void Set(bool value) {
            m_value = value;
            Preferences::GetInstance()->PutBoolean(m_name, m_value);
        }

    private:
        string GetName() {
            return m_name;
        }

        void UpdateConstant() {
#ifndef NOT_REAL
            m_value = Preferences::GetInstance()->GetBoolean(m_name);
#endif
        }

        string m_name;
        bool m_value;
    };

    template<>
    class COREConstant<int> : public ICOREConstant {
    public:
        COREConstant(string name) {
            m_name = name;
            if(!Preferences::GetInstance()->ContainsKey(m_name)) {
                Preferences::GetInstance()->PutInt(m_name, 0);
            }
            UpdateConstant();
            COREConstantsManager::AddConstant(this);
        }

        COREConstant(string name, int defaultValue) {
            m_name = name;
#ifndef NOT_REAL
            Preferences::GetInstance()->PutInt(m_name, defaultValue);
            m_value = defaultValue;
#endif
            COREConstantsManager::AddConstant(this);
        }

        int Get() {
            return m_value;
        }

        void Set(int value) {
            m_value = value;
            Preferences::GetInstance()->PutInt(m_name, m_value);
        }

    private:
        string GetName() {
            return m_name;
        }

        void UpdateConstant() {
#ifndef NOT_REAL
            m_value = Preferences::GetInstance()->GetInt(m_name);
#endif
        }

        string m_name;
        int m_value;
    };
}
