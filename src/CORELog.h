#pragma once

#include <vector>
#include <queue>
#include <string>
#include <memory>
#include <ctime>
#include <fstream>
#include <cmath>
#include "COREHardware/CORETimer.h"

#include <WPILib.h>

using namespace std;
namespace CORE {
    enum robotMode {
        DISABLED,
        TELEOP,
        AUTON
    };
    class CORELog {
    public:
        enum loggingLevel {
            INFO = 0,
            WARNING = 1,
            ERROR = 2
        };
        static void logInfo(string message);
        static void logWarning(string message);
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
        static string getRobotMode();
        static ofstream m_file;
        static vector<string> m_fileCache;
        static string m_fileName;
        static robotMode m_robotMode;
        static CORETimer m_matchTimer;
        static loggingLevel m_consoleLoggingLevel;
    };
}
