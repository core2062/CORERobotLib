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
        COREConstant(string name, T defaultValue) : m_value(defaultValue) {
            m_name = "INVALID TYPE";
            Preferences::GetInstance()->PutString(m_name, "INVALID TYPE SPECIFIED FOR: " + name);
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

    template <>
    class COREConstant<double> : public ICOREConstant {
    public:
        COREConstant(string name, double defaultValue) : m_value(defaultValue) {
            m_name = name;
            Preferences::GetInstance()->PutDouble(m_name, defaultValue);
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
            m_value = Preferences::GetInstance()->GetDouble(m_name);
        }
        string m_name;
        double m_value;
    };

    template <>
    class COREConstant<string> : public ICOREConstant {
    public:
        COREConstant(string name, string defaultValue) : m_value(defaultValue) {
            m_name = name;
            Preferences::GetInstance()->PutString(m_name, defaultValue);
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
            m_value = Preferences::GetInstance()->GetString(m_name);
        }
        string m_name;
        string m_value;
    };

    template <>
    class COREConstant<bool> : public ICOREConstant {
    public:
        COREConstant(string name, bool defaultValue) : m_value(defaultValue) {
            m_name = name;
            Preferences::GetInstance()->PutBoolean(m_name, defaultValue);
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
            m_value = Preferences::GetInstance()->GetBoolean(m_name);
        }
        string m_name;
        bool m_value;
    };

    template <>
	class COREConstant<int> : public ICOREConstant {
	public:
		COREConstant(string name, int defaultValue) : m_value(defaultValue) {
			m_name = name;
			Preferences::GetInstance()->PutInt(m_name, defaultValue);
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
			m_value = Preferences::GetInstance()->GetInt(m_name);
		}
		string m_name;
		int m_value;
	};
}
