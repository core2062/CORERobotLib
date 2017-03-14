#include "CORELog.h"
#include "CORERobot.h"

using namespace CORE;

ofstream CORELog::m_file;
string CORELog::m_fileName;
vector<string> CORELog::m_fileCache;
CORETimer CORELog::m_matchTimer;
CORELog::loggingLevel CORELog::m_consoleLoggingLevel;

void CORELog::writeLastDuration() {
    logInfo(getRobotMode() + " lasted " + to_string(m_matchTimer.Get()) + " seconds");
}

string CORELog::getRobotMode() {
    string robotModeName;
    switch(CORERobot::getMode()) {
        case CORERobot::DISABLE:
            robotModeName = "DISABLED";
            break;
        case CORERobot::AUTON:
            robotModeName = "AUTON";
            break;
        case CORERobot::TELEOP:
            robotModeName = "TELEOP";
            break;
        default:
        	robotModeName = "INVALID";
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
    m_consoleLoggingLevel = INFO;
    time_t currentTime = time(0);
    struct tm* now = localtime(&currentTime);
    m_fileName = "/home/lvuser/CORELogs/";
    if(DriverStation::GetInstance().IsFMSAttached()) {
        string alliance;
        switch(CORERobot::getAlliance()) {
            case CORERobot::RED:
                alliance = "Red";
                break;
            case CORERobot::BLUE:
                alliance = "Blue";
                break;
            default:
                alliance = "Invalid";
                break;
        }
        m_fileName = alliance + " Alliance - Station "
                     + to_string(CORERobot::getStation()) + " - ";
    }
    m_fileName += to_string(now->tm_mon) + "-" + to_string(now->tm_mday) + "--" + to_string(now->tm_hour)
                  + "-" + to_string(now->tm_min) + ".txt";
//    m_file.open(m_fileName);
    cout << "INFO: Log file written to: /home/lvuser/CORELogs/" << m_fileName << endl;
    m_fileCache.clear();
    m_matchTimer.Reset();
    m_matchTimer.Start();
}

void CORELog::autonInit() {
    m_file.open(m_fileName);
    writeLastDuration();
    m_matchTimer.Reset();
    m_matchTimer.Start();
}

void CORELog::teleopInit() {
    m_file.open(m_fileName);
    writeLastDuration();
    m_matchTimer.Reset();
    m_matchTimer.Start();
}

void CORELog::disabled() {
    writeLastDuration();
    m_matchTimer.Reset();
    m_matchTimer.Start();
    updateLog();
}

void CORELog::setConsoleLoggingLevel(CORELog::loggingLevel level) {
    m_consoleLoggingLevel = level;
}
