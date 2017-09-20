#pragma once

#include "gmock/gmock.h"

namespace test {
    class Joystick {
    public:
        typedef enum {
            kXAxis,
            kYAxis,
            kZAxis,
            kTwistAxis,
            kThrottleAxis,
            kNumAxisTypes
        } AxisType;

        enum JoystickHand {
            kLeftHand = 0, kRightHand = 1
        };

        Joystick(int) {}

        MOCK_CONST_METHOD1(GetX, double(int));
        MOCK_CONST_METHOD1(GetY, double(int));
        MOCK_CONST_METHOD1(GetZ, double(int));
        MOCK_CONST_METHOD0(GetTwist, double());
        MOCK_CONST_METHOD0(GetThrottle, double());
        MOCK_CONST_METHOD1(GetPOV, int(int));
        MOCK_CONST_METHOD0(GetPOV, int());
        MOCK_CONST_METHOD1(GetRawAxis, double(int));
        MOCK_CONST_METHOD1(GetRawButton, bool(int));
        MOCK_CONST_METHOD1(GetAxis, double(AxisType
                axis));
        MOCK_CONST_METHOD1(GetTrigger, bool(int));
        MOCK_CONST_METHOD1(GetTop, bool(int));
        MOCK_CONST_METHOD0(GetMagnitude, double());
        MOCK_CONST_METHOD0(GetDirectionRadians, double());
        MOCK_CONST_METHOD0(GetDirectionDegrees, double());
    };
}