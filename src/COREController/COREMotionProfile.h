#pragma once

namespace CORE {
    class ControllerInput;
    class ControllerOutput;
    class COREMotionProfile {
    public:
        COREMotionProfile(ControllerInput * inputDevice, ControllerOutput * outputDevice);
        void setMaxVel(double maxVelocity);
        void setMaxAccel(double maxAcceleration);
        void setMinAccel(double minAcceleration);
        void setMaxJerk(double maxJerk);
        double getMaxVel();
        double getMaxAccel();
        double getMinAccel();
        double getMaxJerk();
    public:
        double m_maxVel = -1;
        double m_maxAccel = -1;
        double m_minAccel = -1;
        double m_maxJerk = -1;
    };
}