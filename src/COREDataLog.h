#pragma once

#include "WPILib.h"

using namespace std;

namespace CORE {
    class COREDataLog {
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
    };
}