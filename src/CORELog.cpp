#include "CORELog.h"

using namespace CORE;

ofstream CORELog::m_file;
string CORELog::m_fileName;
vector<string> CORELog::m_fileCache;
robotMode CORELog::m_robotMode;
CORETimer CORELog::m_matchTimer;
CORELog::loggingLevel CORELog::m_consoleLoggingLevel;

void CORELog::writeLastDuration() {
    logInfo(getRobotMode() + " lasted " + to_string(m_matchTimer.Get()) + " seconds");
}

string CORELog::getRobotMode() {
    string robotModeName;
    switch(m_robotMode) {
        case DISABLED:
            robotModeName = "DISABLED";
            break;
        case AUTON:
            robotModeName = "AUTON";
            break;
        case TELEOP:
            robotModeName = "TELEOP";
            break;
    }
    return robotModeName;
}

void CORELog::logInfo(string message) {
    m_fileCache.push_back("[INFO - " + getRobotMode() + "] - " + to_string(round(m_matchTimer.Get() * 1000.0) / 1000.0)
                          + " " + message + "\n");
    if(m_consoleLoggingLevel <= INFO) {
        cout << "INFO: " << message << "\n";
    }
}

void CORELog::logWarning(string message) {
    m_fileCache.push_back(
            "[WARNING - " + getRobotMode() + "] - " + to_string(round(m_matchTimer.Get() * 1000.0) / 1000.0)
            + " " + message + "\n");
    if(m_consoleLoggingLevel <= WARNING) {
        cout << "WARNING: " << message << "\n";
    }
}

void CORELog::logError(string message) {
    m_fileCache.push_back("[ERROR - " + getRobotMode() + "] - " + to_string(round(m_matchTimer.Get() * 1000.0) / 1000.0)
                          + " " + message + "\n");
    if(m_consoleLoggingLevel <= WARNING) {
        cout << "ERROR: " << message << "\n";
    }
}

void CORELog::updateLog() {
    CORETimer duration;
    duration.Reset();
    duration.Start();
    for(auto text : m_fileCache) {
        m_file << text;
    }
    m_file.close();
    logInfo("Writing to log took: " + to_string(duration.Get()));
}

string CORELog::getName() {
    return std::string();
}

void CORELog::robotInit() {
    m_robotMode = DISABLED;
    m_consoleLoggingLevel = INFO;
    time_t currentTime = time(0);
    struct tm* now = localtime(&currentTime);
    m_fileName = "/home/lvuser/CORELogs/";
    if(DriverStation::GetInstance().IsFMSAttached()) {
        string alliance;
        switch(DriverStation::GetInstance().GetAlliance()) {
            case DriverStation::Alliance::kRed:
                alliance = "Red";
                break;
            case DriverStation::Alliance::kBlue:
                alliance = "Blue";
                break;
            default:
                alliance = "Unknown";
                break;
        }
        m_fileName = alliance + " Alliance - Station "
                     + to_string(DriverStation::GetInstance().GetLocation()) + " - ";
    }
    m_fileName += to_string(now->tm_mon) + "-" + to_string(now->tm_mday) + "--" + to_string(now->tm_hour)
                  + "-" + to_string(now->tm_min) + ".txt";
//    m_file.open(m_fileName);
    cout << "INFO: Log file written to: /home/lvuser/CORELogs/" << m_fileName << endl;
    m_fileCache.clear();
    m_robotMode = DISABLED;
    m_matchTimer.Reset();
    m_matchTimer.Start();
}

void CORELog::autonInit() {
    m_file.open(m_fileName);
    writeLastDuration();
    m_robotMode = AUTON;
    m_matchTimer.Reset();
    m_matchTimer.Start();
}

void CORELog::teleopInit() {
    m_file.open(m_fileName);
    writeLastDuration();
    m_robotMode = TELEOP;
    m_matchTimer.Reset();
    m_matchTimer.Start();
}

void CORELog::disabled() {
    writeLastDuration();
    m_robotMode = DISABLED;
    m_matchTimer.Reset();
    m_matchTimer.Start();
    updateLog();
}

void CORELog::setConsoleLoggingLevel(CORELog::loggingLevel level) {
    m_consoleLoggingLevel = level;
}
