#include "CORELog.h"

using namespace CORE;

ofstream CORELog::m_file;
string CORELog::m_fileName;
vector<string> CORELog::m_fileCache;
CORETimer CORELog::m_matchTimer;
CORELog::loggingLevel CORELog::m_consoleLoggingLevel;

void CORELog::writeLastDuration() {
    logInfo(getRobotMode() + " lasted " + to_string(m_matchTimer.Get()) + " seconds");
}

string CORELog::getFileName() {
    time_t currentTime = time(0);
    struct tm* now = localtime(&currentTime);
	string fileName = "";
    if(COREDriverstation::isCompetition()) {
        string alliance;
        switch(COREDriverstation::getAlliance()) {
            case COREDriverstation::RED:
                alliance = "Red";
                break;
            case COREDriverstation::BLUE:
                alliance = "Blue";
                break;
            default:
                alliance = "Invalid";
                break;
        }
        fileName += alliance + " Alliance - Station "
                     + to_string(COREDriverstation::getStation()) + " - ";
    }
    fileName += to_string(now->tm_mon) + "-" + to_string(now->tm_mday) + "--" + to_string(now->tm_hour)
                  + "-" + to_string(now->tm_min) + ".txt";
    return fileName;
}

string CORELog::getRobotMode() {
    string robotModeName;
    switch(COREDriverstation::getMode()) {
        case COREDriverstation::DISABLE:
            robotModeName = "DISABLED";
            break;
        case COREDriverstation::AUTON:
            robotModeName = "AUTON";
            break;
        case COREDriverstation::TELEOP:
            robotModeName = "TELEOP";
            break;
        default:
        	robotModeName = "INVALID";
        	break;
    }
    return robotModeName;
}

void CORELog::logInfo(ostringstream& message) {
//	logInfo(message.str());
}

void CORELog::logInfo(string message) {
    m_fileCache.push_back("[INFO - " + getRobotMode() + "] - " + to_string(round(m_matchTimer.Get() * 1000.0) / 1000.0)
                          + " " + message + "\n");
    if(m_consoleLoggingLevel <= INFO) {
        cout << "INFO: " << message << "\n";
    }
}

void CORELog::logWarning(ostringstream& message) {
//	logWarning(message.str());
}

void CORELog::logWarning(string message) {
    m_fileCache.push_back(
            "[WARNING - " + getRobotMode() + "] - " + to_string(round(m_matchTimer.Get() * 1000.0) / 1000.0)
            + " " + message + "\n");
    if(m_consoleLoggingLevel <= WARNING) {
        cout << "WARNING: " << message << "\n";
    }
}

void CORELog::logError(ostringstream& message) {
//	logError(message.str());
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
    m_fileName = "/media/sda1/CORELogs/" + getFileName();
    m_file.open(m_fileName);
    if(!m_file.is_open()) {
    	logWarning("Unable to open " + m_fileName + ", defaulting to internal storage!");
    	m_file.close();
    	m_fileName = "/home/lvuser/CORELogs/" + getFileName();
    	ifstream m_file(m_fileName);
    }
    logInfo("Log file written to: " + m_fileName);
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

void CORELog::testInit() {
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
