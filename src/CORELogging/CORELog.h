#pragma once

#include <vector>
#include <queue>
#include <string>
#include <memory>
#include <ctime>
#include <fstream>
#include <iostream>
#include <cmath>
#include "COREUtilities/CORETimer.h"
#include "COREUtilities/COREDriverstation.h"

using namespace std;

#undef ERROR

namespace CORE {
    class CORELog {
    public:
        enum loggingLevel {
            INFO,
            WARNING,
            ERROR
        };
        static void LogInfo(ostringstream& message);
        static void LogInfo(string message);
        static void LogWarning(ostringstream& message);
        static void LogWarning(string message);
        static void LogError(ostringstream& message);
        static void LogError(string message);
        static void SensorError(int sensorID, string sensorName, double value);
        static void SensorError(int sensorID, string sensorName, bool value);
        static void SensorLog(int sensorID, string sensorName, double value);
        static void SensorLog(int sensorID, string sensorName, bool value);
        static void NavXLog(double x, double y, double radians);
        static void UpdateLog();
        static string GetName();
        static void robotInit();
        static void AutonInit();
        static void teleopInit();
        static void testInit();

        static void disabled();
        static void SetConsoleLoggingLevel(loggingLevel level);
            private:
        static void WriteLastDuration();
        static string GetFileName();
        static string GetRobotMode();
        static ofstream m_file;
        static vector<string> m_fileCache;
        static string m_fileName;
        static CORETimer m_matchTimer;
        static loggingLevel m_consoleLoggingLevel;
    };
}
