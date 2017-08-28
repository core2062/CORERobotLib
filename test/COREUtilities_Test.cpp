#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <COREUtilities/Rotation2d.h>
#include <CORELogging/CORELog.h>

#include "COREUtilities/CORETimer.h"
#include "CORESimulation/WPILib.h"

using namespace CORE;

TEST_CASE("StartStop") {
    CORETimer timer;
    timer.Start();
    Wait(0.01);
    REQUIRE(abs(timer.Get() - 0.01) < 0.01);
    timer.Stop();
    Wait(0.01);
    REQUIRE(abs(timer.Get() - 0.01) < 0.01);
}

TEST_CASE("1==1") {
    REQUIRE(1==1);
}

TEST_CASE("Rotation2d") {
    SECTION("toCompassDegress works") {
        double testData[5][3] = {{0, 1, 0},
                                 {1, 1, 45},
                                 {1, 0, 90},
                                 {0, -1, 180},
                                 {-1, 0, 270}};

        for(int i = 0; i < 5; i++) {
            Rotation2d rotation(testData[i][0], testData[i][1], true);
            REQUIRE(rotation.getCompassDegrees() == testData[i][2]);
        }
    }
    SECTION("rotateBy works") {
        Rotation2d rotation(1, 0, true);
        Rotation2d ninetyDegrees(0, 1, true);
        REQUIRE(rotation.rotateBy(ninetyDegrees).getCompassDegrees() == 0);
    }
    SECTION("opposite works") {
        Rotation2d rotation(0, 1, true); //0 degrees
        REQUIRE(rotation.opposite().getCompassDegrees() == 180);
    }
}

//TEST(CORELog, CORELogInit) {
//    CORE::CORELog::robotInit();
//    ASSERT_NO_THROW(CORE::CORELog::getName());
//}