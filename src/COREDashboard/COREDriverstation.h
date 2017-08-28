#pragma once

#ifndef FAKE
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
        static bool IsCompetition();
        static void updateRobotState();
    private:
        static gameMode m_mode;
        static gameAlliance m_alliance;
        static int m_station;
        static bool m_isCompetition;
    };
}