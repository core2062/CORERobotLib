#pragma once

#include "CORELogging/CORELog.h"

#ifndef NOT_REAL
#include "frc/WPILib.h"
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

        static gameMode GetMode();
        static gameAlliance GetAlliance();
        static int GetStation();
        static bool IsEnabled();
        static bool IsOperatorControl();
        static bool IsAutonomous();
        static bool IsCompetition();
        static void UpdateRobotState();
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
