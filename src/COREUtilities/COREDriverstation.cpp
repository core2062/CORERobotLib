#include "COREDriverstation.h"

using namespace CORE;

COREDriverstation::gameMode COREDriverstation::m_mode = COREDriverstation::DISABLE;
COREDriverstation::gameAlliance COREDriverstation::m_alliance = COREDriverstation::INVALID;
int COREDriverstation::m_station = 0;
bool COREDriverstation::m_isCompetition = false;

void COREDriverstation::UpdateRobotState() {
    if(frc::DriverStation::GetInstance().IsDisabled()) {
        m_mode = DISABLE;
    } else if(frc::DriverStation::GetInstance().IsAutonomous()) {
        m_mode = AUTON;
    } else if(frc::DriverStation::GetInstance().IsOperatorControl()) {
        m_mode = TELEOP;
    } else if(frc::DriverStation::GetInstance().IsTest()) {
        m_mode = TEST;
    } else {
        m_mode = DISABLE;
    }

    if(frc::DriverStation::GetInstance().GetAlliance() == frc::DriverStation::kRed) {
        m_alliance = RED;
    } else if(frc::DriverStation::GetInstance().GetAlliance() == frc::DriverStation::kBlue) {
        m_alliance = BLUE;
    } else if(frc::DriverStation::GetInstance().GetAlliance() == frc::DriverStation::kInvalid) {
        m_alliance = INVALID;
    }
    m_station = frc::DriverStation::GetInstance().GetLocation();
    m_isCompetition = frc::DriverStation::GetInstance().IsFMSAttached();
}

COREDriverstation::gameMode COREDriverstation::GetMode(){
    return m_mode;
}

COREDriverstation::gameAlliance COREDriverstation::GetAlliance() {
    return m_alliance;
}

int COREDriverstation::GetStation() {
    return m_station;
}

bool COREDriverstation::IsCompetition() {
    return m_isCompetition;
}

bool COREDriverstation::IsEnabled() {
    UpdateRobotState();
    return m_mode != DISABLE;
}

bool COREDriverstation::IsOperatorControl() {
    UpdateRobotState();
    return m_mode == TELEOP;
}

bool COREDriverstation::IsAutonomous() {
    return m_mode == AUTON;
}
