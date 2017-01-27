#include "CORELog.h"

using namespace CORE;


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

CORELog::CORELog(string fileName) {
    m_robotMode = DISABLED;
    time_t currentTime = time(0);
    struct tm * now = localtime(& currentTime);
    m_fileName = "";
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
        //Red Alliance - Station 1 - MM-DD--H-MN
    }
    m_fileName += to_string(now->tm_mon) + "--" + to_string(now->tm_mday) + "-" + to_string(now->tm_hour)
                  + "-" + to_string(now->tm_min);
    m_file.open(m_fileName);

}

/*void CORELog::logData(string message) {

}*/

void CORELog::logInfo(string message) {
    m_file << "[INFO - " << getRobotMode() << "] - " << round(m_matchTimer.Get() * 1000.0 ) / 1000.0 << " "
           << message << "\n";
}

//[INFO - TELEOP] - matchtime

void CORELog::logWarning(string message) {
    m_file << "[WARN - " << getRobotMode() << "] - " << round(m_matchTimer.Get() * 1000.0 ) / 1000.0 << " "
           << message << "\n";
}

void CORELog::logError(string message) {
    m_file << "[ERROR - " << getRobotMode() << "] - " << round(m_matchTimer.Get() * 1000.0 ) / 1000.0 << " "
           << message << "\n";
}

void CORELog::updateLog() {

}

string CORELog::getName() {
    return std::string();
}

void CORELog::robotInitTask() {
    writeLastDuration();
    m_robotMode = DISABLED;
    m_matchTimer.Reset();
    m_matchTimer.Start();
}

void CORELog::autonInitTask() {
    writeLastDuration();
    m_robotMode = AUTON;
    m_matchTimer.Reset();
    m_matchTimer.Start();
}

void CORELog::teleopInitTask() {
    writeLastDuration();
    m_robotMode = TELEOP;
    m_matchTimer.Reset();
    m_matchTimer.Start();
}

void CORELog::disabledTask() {
    writeLastDuration();
    m_robotMode = DISABLED;
    m_matchTimer.Reset();
    m_matchTimer.Start();
}