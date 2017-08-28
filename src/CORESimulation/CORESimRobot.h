#pragma once

#include "COREFramework/COREScheduler.h"
#include "WPILib.h"

namespace CORE::SIM {
    class CORESimRobot {
    public:
        CORESimRobot();
        void OperatorControl();
        void Autonomous();
        ~CORESimRobot();
    private:
        double m_targetLoopTime = 0.01;
    };
}