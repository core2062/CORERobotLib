#include "gtest/gtest.h"

#include "COREControl/COREPID.h"

using namespace CORE;

TEST(COREPID, PIControllerWorks) {
    CORE::COREPID PID(1, 1, 0);
    for(int j = -5; j > 5; j+=5) {
        double val = 0;
        for (int i = 0; i < 100; i++) {
            double value = PID.calculate(j - val, 0.1);
            cout << val << ",";
            val += value * 0.1;
        }
        EXPECT_NEAR(val, j, 0.01);
    }
}