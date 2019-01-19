#include "CORELog.h"

using namespace CORE;

ofstream CORELog::m_file;
string CORELog::m_fileName;
vector<string> CORELog::m_fileCache;
CORETimer CORELog::m_matchTimer;
CORELog::loggingLevel CORELog::m_consoleLoggingLevel;

void CORELog::WriteLastDuration() {
    LogInfo(GetRobotMode() + " lasted " + to_string(m_matchTimer.Get()) + " seconds");
}

string CORELog::GetFileName() {
    time_t currentTime = time(0);
    struct tm* now = localtime(&currentTime);
	string fileName = "";
    if(COREDriverstation::IsCompetition()) {
        string alliance;
        switch(COREDriverstation::GetAlliance()) {
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
                     + to_string(COREDriverstation::GetStation()) + " - ";
    }
    fileName += to_string(now->tm_mon) + "-" + to_string(now->tm_mday) + "--" + to_string(now->tm_hour)
                  + "-" + to_string(now->tm_min) + ".txt";
    return fileName;
}

string CORELog::GetRobotMode() {
    string robotModeName;
    switch(COREDriverstation::GetMode()) {
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

void CORELog::LogInfo(ostringstream& message) {
//	logInfo(message.str());
}

void CORELog::LogInfo(string message) {
    m_fileCache.push_back("[INFO - " + GetRobotMode() + "] - " + to_string(round(m_matchTimer.Get() * 1000.0) / 1000.0)
                          + " " + message + "\n");
    if(m_consoleLoggingLevel <= INFO) {
        cout << "INFO: " << message << "\n";
    }
}

void CORELog::LogWarning(ostringstream& message) {
//	logWarning(message.str());
}

void CORELog::LogWarning(string message) {
    m_fileCache.push_back(
            "[WARNING - " + GetRobotMode() + "] - " + to_string(round(m_matchTimer.Get() * 1000.0) / 1000.0)
            + " " + message + "\n");
    if(m_consoleLoggingLevel <= WARNING) {
        cout << "WARNING: " << message << "\n";
    }
}

void CORELog::LogError(ostringstream& message) {
//	logError(message.str());
}

void CORELog::LogError(string message) {
    m_fileCache.push_back("[ERROR - " + GetRobotMode() + "] - " + to_string(round(m_matchTimer.Get() * 1000.0) / 1000.0)
                          + " " + message + "\n");
    if(m_consoleLoggingLevel <= WARNING) {
        cout << "ERROR: " << message << "\n";
    }
}

void CORELog::SensorError(int sensorID, string sensorName, double value) {
	m_fileCache.push_back("[ERROR] - " + to_string(round(m_matchTimer.Get() * 1000.0) / 1000.0)
			+ ", " + to_string(sensorID) + ", " + sensorName + ", " + to_string(value)  + "\n");
}

void CORELog::SensorError(int sensorID, string sensorName, bool value) {
	m_fileCache.push_back("[ERROR] - " + to_string(round(m_matchTimer.Get() * 1000.0) / 1000.0)
			+ ", " + to_string(sensorID) + ", " + sensorName + ", " + to_string(value)  + "\n");
}

void CORELog::SensorLog(int sensorID, string sensorName, double value) {
	m_fileCache.push_back("[INFO] - " + to_string(round(m_matchTimer.Get() * 1000.0) / 1000.0)
			+ ", " + to_string(sensorID) + ", " + sensorName + ", " + to_string(value)  + "\n");
}

void CORELog::SensorLog(int sensorID, string sensorName, bool value) {
	m_fileCache.push_back("[INFO] - " + to_string(round(m_matchTimer.Get() * 1000.0) / 1000.0)
		    + ", " + to_string(sensorID) + ", " + sensorName + ", " + to_string(value)  + "\n");
}

void CORELog::NavXLog(double x, double y, double radians) {
	m_fileCache.push_back("[INFO] - " + to_string(round(m_matchTimer.Get() * 1000.0) / 1000.0)
			  + ", X: " + to_string(x) + ", Y: " + to_string(y) + ", Theta: " + to_string(radians)  + "\n");
}

void CORELog::UpdateLog() {
    CORETimer duration;
    duration.Reset();
    duration.Start();
    for(auto text : m_fileCache) {
        m_file << text;
    }
    m_file.close();
    LogInfo("Writing to log took: " + to_string(duration.Get()));
}

string CORELog::GetName() {
    return std::string();
}

void CORELog::RobotInit() {
    m_consoleLoggingLevel = INFO;
    m_fileName = "/media/sda1/CORELogs/" + GetFileName();
    m_file.open(m_fileName);
    if(!m_file.is_open()) {
    	LogWarning("Unable to open " + m_fileName + ", defaulting to internal storage!");
    	m_file.close();
    	m_fileName = "/home/lvuser/CORELogs/" + GetFileName();
    	ifstream m_file(m_fileName);
    }
    LogInfo("Log file written to: " + m_fileName);
    m_fileCache.clear();
    m_matchTimer.Reset();
    m_matchTimer.Start();
}

void CORELog::AutonInit() {
    m_file.open(m_fileName);
    WriteLastDuration();
    m_matchTimer.Reset();
    m_matchTimer.Start();
}

void CORELog::TeleopInit() {
    m_file.open(m_fileName);
    WriteLastDuration();
    m_matchTimer.Reset();
    m_matchTimer.Start();
}

void CORELog::TestInit() {
    m_file.open(m_fileName);
    WriteLastDuration();
    m_matchTimer.Reset();
    m_matchTimer.Start();
}

void CORELog::Disabled() {
    WriteLastDuration();
    m_matchTimer.Reset();
    m_matchTimer.Start();
    UpdateLog();
}

void CORELog::SetConsoleLoggingLevel(CORELog::loggingLevel level) {
    m_consoleLoggingLevel = level;
}
