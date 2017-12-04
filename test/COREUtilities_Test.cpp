#include "gtest/gtest.h"

#include <COREUtilities/Rotation2d.h>
#include <CORELogging/CORELog.h>
#include <thread>

#include "COREUtilities/CORETimer.h"
#include "CORESimulation/WPILib.h"

using namespace CORE;
using namespace std;

TEST(CORETimer, getTimeMultithreaded) {
    thread * thread1, *thread2;
    thread1 = new thread([&]{
        for(int i = 0; i != 10; i++) {
            EXPECT_NO_THROW(CORETimer::getTime());
        }
    });
    for(int i = 0; i != 10; i++) {
        EXPECT_NO_THROW(CORETimer::getTime());
    }
    thread1->join();
}

TEST(CORETimer, startStop) {
    CORETimer timer;
    timer.Start();
    Wait(0.01);
    EXPECT_NEAR(timer.Get(), 0.01, 0.01);
    timer.Stop();
    Wait(0.01);
    EXPECT_NEAR(timer.Get(), 0.01, 0.01);
}

TEST(Rotation2d, toCompassDegrees) {
    double testData[5][3] = {{0, 1, 0},
                             {1, 1, 45},
                             {1, 0, 90},
                             {0, -1, 180},
                             {-1, 0, 270}};

    for (auto &i : testData) {
        Rotation2d rotation(i[0], i[1], true);
        EXPECT_EQ(rotation.getCompassDegrees(), i[2]);
    }
}

TEST(Rotation2d, rotateBy) {
    Rotation2d rotation(1, 0, true);
    Rotation2d ninetyDegrees(0, 1, true);
    EXPECT_EQ(rotation.rotateBy(ninetyDegrees).getCompassDegrees(), 0);
}

TEST(Rotation2d, opposite) {
    Rotation2d rotation(0, 1, true); //0 degrees
    EXPECT_EQ(rotation.opposite().getCompassDegrees(), 180);
}

TEST(Rotation2d, interpolate) {

}
