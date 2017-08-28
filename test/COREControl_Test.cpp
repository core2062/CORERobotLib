//#include <CORESimulation/WPILib.h>
//#include "catch.hpp"
//#include "COREControl/COREPositionPID.h"
//#include "CORELogging/CORELog.h"
//
//using namespace CORE;
//
//TEST_CASE("COREPosPID") {
//    SECTION("Proportional PID works for several kP Values") {
//        for(int i = 1; i != 3; i++) {
//            COREPositionPID pidController(i, 0, 0);
//            pidController.calculate(100);
//            REQUIRE(pidController.calculate() == 100.0 * i);
//            pidController.setActual(100);
//            pidController.Set(0);
//            REQUIRE(pidController.calculate() == -100.0 * i);
//        }
//    }
//    SECTION("Integral PID works for one kI Value") {
//        COREPositionPID pidController(1, 1, 0);
//        pidController.setActual(0);
//        pidController.Set(100);
//        pidController.calculate();
//
//        double lastPos;
//        double maxPos = 0;
//        for(int j = 0; j <= 148; j++) {
//            Wait(0.001);
//            lastPos = pidController.calculate();
//            if(pidController.getActual() > maxPos) {
//                maxPos = pidController.getActual();
//            }
//            pidController.setActual(lastPos * 0.001 + pidController.getActual());
//        }
//        REQUIRE(abs(maxPos - 196) < 2);
//        REQUIRE(abs(pidController.getActual() - 99) < 2);
//    }
//}