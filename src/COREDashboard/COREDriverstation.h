#pragma once

#include "CORELogging/CORELog.h"

#ifndef NOT_REAL
#include "WPILib.h"
#endif

namespace CORE {
    class COREDriverstation {
    public:
        enum gameMode{
            TELEOP,
            AUTON,
            TEST,
            DISABLE
        };

        enum gameAlliance {
            RED = -1,
            BLUE = 1,
            INVALID = 0
        };

        static gameMode getMode();
        static gameAlliance getAlliance();
        static int getStation();
        static bool isEnabled();
        static bool isOperatorControl();
        static bool isAutonomous();
        static bool isCompetition();
        static void updateRobotState();
#ifdef NOT_REAL
        static void overrideRobotMode(gameMode newMode);
        static void overrideAlliance(gameAlliance newAlliance);
#endif
    private:
        static gameMode m_mode;
        static gameAlliance m_alliance;
        static int m_station;
        static bool m_isCompetition;
    };
}