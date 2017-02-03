#pragma once

#include <WPILib.h>
#include <vector>

using namespace std;

namespace CORE {
    class ICOREDataLogger {
    public:
        virtual string getName() = 0;
        virtual void updateConstant(Preferences * prefs) = 0;
    };

    template <class T>
    class COREDataLogger : public ICOREDataLogger {
    public:
        COREDataLogger(string name, T defaultValue);
        string getName();
        void updateConstant(Preferences * prefs);
        T value;
    private:
        string m_name;
    };

    template <>
    class COREDataLogger<double> : public ICOREDataLogger {
    public:
        COREDataLogger(string name, double defaultValue);
        string getName();
        void updateConstant(Preferences * prefs);
        double value;
    private:
        string m_name;
    };

    template <>
    class COREDataLogger<string> : public ICOREDataLogger {
    public:
        COREDataLogger(string name, string defaultValue);
        string getName();
        void updateConstant(Preferences * prefs);
        string value;
    private:
        string m_name;
    };

    template <>
    class COREDataLogger<bool> : public ICOREDataLogger {
    public:
        COREDataLogger(string name, bool defaultValue);
        string getName();
        void updateConstant(Preferences * prefs);
        bool value;
    private:
        string m_name;
    };

    class COREDataLoggerManager {
    public:
        static void robotInit();
        static void updateConstants();
        static void addLogger(ICOREDataLogger *instance);
    private:
        static vector<ICOREDataLogger*> m_constants;
        static string m_defaultConstantsFile; //TODO: Put constants to CSV file
        static Preferences * m_prefs;
    };


    /*class COREDataLog {
    public:
        COREDataLog(string name);
        void putNumber(double value);
        double getNumber();
        void putString(string value);
        string getString();
        void putBoolean(bool value);
        bool getBoolean();
        string getName();
        void setDefaultNumber(double value);
        void setDefaultString(string value);
        void setDefaultBoolean(bool value);
    private:
        string m_name;
        double m_defaultNumber = 0;
        string m_defaultString = "NULL";
        bool m_defaultBoolean = false;
    };*/
}