#include "COREDriverstation.h"

using namespace CORE;

COREDriverstation::gameMode COREDriverstation::m_mode = COREDriverstation::DISABLE;
COREDriverstation::gameAlliance COREDriverstation::m_alliance = COREDriverstation::INVALID;
int COREDriverstation::m_station = 0;
bool COREDriverstation::m_isCompetition = false;

void COREDriverstation::updateRobotState() {
#ifdef FAKE
    //TODO: Get robot state from webdashboard
    m_mode = DISABLE;
    m_alliance = RED;
    m_station = -1;
    m_isCompetition = false;
#else
    if(DriverStation::GetInstance().IsDisabled()) {
        m_mode = DISABLE;
    } else if(DriverStation::GetInstance().IsAutonomous()) {
        m_mode = AUTON;
    } else if(DriverStation::GetInstance().IsOperatorControl()) {
        m_mode = TELEOP;
    } else if(DriverStation::GetInstance().IsTest()) {
        m_mode = TEST;
    } else {
        m_mode = DISABLE;
    }

    if(DriverStation::GetInstance().GetAlliance() == DriverStation::kRed) {
        m_alliance = RED;
    } else if(DriverStation::GetInstance().GetAlliance() == DriverStation::kBlue) {
        m_alliance = BLUE;
    } else if(DriverStation::GetInstance().GetAlliance() == DriverStation::kInvalid) {
        m_alliance = INVALID;
    }

    m_station = DriverStation::GetInstance().GetLocation();
    m_isCompetition = DriverStation::GetInstance().IsFMSAttached();
#endif
}

COREDriverstation::gameMode COREDriverstation::getMode(){
    return m_mode;
}

COREDriverstation::gameAlliance COREDriverstation::getAlliance() {
    return m_alliance;
}

int COREDriverstation::getStation() {
    return m_station;
}

bool COREDriverstation::IsCompetition() {
    return m_isCompetition;
}