#pragma once

#include <string>
#include <memory>
#include <vector>

#include "CORELog.h"
#include "WPILib.h"

using namespace std;

namespace CORE {
    class ICOREConstant {
    public:
        virtual string getName() = 0;
        virtual void updateConstant(Preferences * prefs) = 0;
    };

    template <class T>
    class COREConstant : public ICOREConstant {
    public:
        COREConstant(string name, T defaultValue);
        string getName();
        void updateConstant(Preferences * prefs);
        T value;
    private:
        string m_name;
    };

    template <>
    class COREConstant<double> : public ICOREConstant {
    public:
        COREConstant(string name, double defaultValue);
        string getName();
        void updateConstant(Preferences * prefs);
        double value;
    private:
        string m_name;
    };

    template <>
    class COREConstant<string> : public ICOREConstant {
    public:
        COREConstant(string name, string defaultValue);
        string getName();
        void updateConstant(Preferences * prefs);
        string value;
    private:
        string m_name;
    };

    template <>
    class COREConstant<bool> : public ICOREConstant {
    public:
        COREConstant(string name, bool defaultValue);
        string getName();
        void updateConstant(Preferences * prefs);
        bool value;
    private:
        string m_name;
    };

    class COREConstantsManager {
    public:
        static void robotInit();
        static void updateConstants();
        static void addConstant(ICOREConstant * instance);
    private:
        static vector<ICOREConstant*> m_constants;
        static string m_defaultConstantsFile; //TODO: Put constants to CSV file
        static Preferences * m_prefs;
    };
}