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
        static void logInfo(ostringstream& message);
        static void logInfo(string message);
        static void logWarning(ostringstream& message);
        static void logWarning(string message);
        static void logError(ostringstream& message);
        static void logError(string message);
        static void updateLog();
        static string getName();
        static void robotInit();
        static void autonInit();
        static void teleopInit();
        static void disabled();
        static void setConsoleLoggingLevel(loggingLevel level);
    private:
        static void writeLastDuration();
        static string getFileName();
        static string getRobotMode();
        static ofstream m_file;
        static vector<string> m_fileCache;
        static string m_fileName;
        static CORETimer m_matchTimer;
        static loggingLevel m_consoleLoggingLevel;
    };
}
