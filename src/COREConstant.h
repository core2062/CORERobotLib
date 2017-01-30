#pragma once

#include <string>
#include <memory>
#include <vector>

#include "CORELog.h"
#include "WPILib.h"

using namespace std;

namespace CORE {
    class ICOREConstant {
        friend class COREConstantsManager;
    private:
        virtual string getName() = 0;
        virtual void updateConstant() = 0;
    };


    class COREConstantsManager {
    public:
        static void robotInit();
        static void updateConstants();
        static void addConstant(ICOREConstant * instance);
    private:
        static vector<ICOREConstant*> m_constants;
        static string m_defaultConstantsFile; //TODO: Put constants to CSV file
    };

    template <class T>
    class COREConstant : public ICOREConstant {
    public:
        COREConstant(string name, T defaultValue) {
            m_name = "INVALID TYPE";
            Preferences::GetInstance()->PutString(m_name, "INVALID TYPE SPECIFIED FOR: " + name);
            CORELog::logError("Invalid COREConstant type specified with name: " + name);
        }
        T Get();
    private:
        string getName();
        void updateConstant();
        string m_name;
        T m_value;
    };

    template <>
    class COREConstant<double> : public ICOREConstant {
    public:
        COREConstant(string name, double defaultValue) {
            m_name = name;
            Preferences::GetInstance()->PutDouble(m_name, defaultValue);
            COREConstantsManager::addConstant(this);
        }
        double Get();
    private:
        string getName();
        void updateConstant();
        string m_name;
        double m_value;
    };

    template <>
    class COREConstant<string> : public ICOREConstant {
    public:
        COREConstant(string name, string defaultValue) {
            m_name = name;
            Preferences::GetInstance()->PutString(m_name, defaultValue);
            COREConstantsManager::addConstant(this);
        }
        string Get();
    private:
        string getName();
        void updateConstant();
        string m_name;
        string m_value;
    };

    template <>
    class COREConstant<bool> : public ICOREConstant {
    public:
        COREConstant(string name, bool defaultValue) {
            m_name = name;
            Preferences::GetInstance()->PutBoolean(m_name, defaultValue);
            COREConstantsManager::addConstant(this);
        }
        bool Get();
    private:
        string getName();
        void updateConstant();
        string m_name;
        bool m_value;
    };
}
