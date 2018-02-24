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
        virtual string getName() = 0;
        virtual void updateConstant() = 0;
    };

    class COREConstantsManager {
    public:
        static void updateConstants();
        static void addConstant(ICOREConstant* instance);
        static void cleanUp();
    private:
        static vector<ICOREConstant*> m_constants;
        static string m_defaultConstantsFile; //TODO: Put constants to CSV file
    };

    template<class T>
    class COREConstant : public ICOREConstant {
    public:
        COREConstant(string name, T defaultValue) : m_value(defaultValue) {
            m_name = "INVALID TYPE";
#ifndef NOT_REAL
            Preferences::GetInstance()->PutString(m_name, "INVALID TYPE SPECIFIED FOR: " + name);
#endif
            CORELog::logError("Invalid COREConstant type specified with name: " + name);
        }

        T Get() {
            return m_value;
        }

    private:
        string getName() {
            return m_name;
        }

        void updateConstant() {
            m_value = "INVALID TYPE SPECIFIED";
        }

        string m_name;
        T m_value;
    };

    template<>
    class COREConstant<double> : public ICOREConstant {
    public:
        COREConstant(string name, double defaultValue) : m_value(defaultValue) {
            m_name = name;
#ifndef NOT_REAL
            Preferences::GetInstance()->PutDouble(m_name, defaultValue);
#endif
            COREConstantsManager::addConstant(this);
        }

        double Get() {
            return m_value;
        }

    private:
        string getName() {
            return m_name;
        }

        void updateConstant() {
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
        COREConstant(string name, string defaultValue) : m_value(defaultValue) {
            m_name = name;
#ifndef NOT_REAL
            Preferences::GetInstance()->PutString(m_name, defaultValue);
#endif
            COREConstantsManager::addConstant(this);
        }

        string Get() {
            return m_value;
        }

    private:
        string getName() {
            return m_name;
        }

        void updateConstant() {
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
        COREConstant(string name, bool defaultValue) : m_value(defaultValue) {
            m_name = name;
#ifndef NOT_REAL
            Preferences::GetInstance()->PutBoolean(m_name, defaultValue);
#endif
            COREConstantsManager::addConstant(this);
        }

        bool Get() {
            return m_value;
        }

    private:
        string getName() {
            return m_name;
        }

        void updateConstant() {
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
        COREConstant(string name, int defaultValue) : m_value(defaultValue) {
            m_name = name;
#ifndef NOT_REAL
            Preferences::GetInstance()->PutInt(m_name, defaultValue);
#endif
            COREConstantsManager::addConstant(this);
        }

        int Get() {
            return m_value;
        }

    private:
        string getName() {
            return m_name;
        }

        void updateConstant() {
#ifndef NOT_REAL
            m_value = Preferences::GetInstance()->GetInt(m_name);
#endif
        }

        string m_name;
        int m_value;
    };
}
