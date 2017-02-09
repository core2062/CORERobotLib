#pragma once

#include <COREController.h>

namespace CORE {
    class COREMotionProfile : COREController {
    public:
        COREMotionProfile(ControllerInput * inputDevice, ControllerOutput * outputDevice);
        void setMaxVel(double maxVelocity);
        void setMaxAccel(double maxAcceleration);
        double getMaxVel();
        double getMaxAccel();
        void Set(double setPoint) override;
        double Get() override;
        void setActual(double actualPosition) override;
        double getActual() override;
    private:
        void update(int profile) override;
        double m_maxVel = -1;
        double m_maxAccel = -1;
        double m_maxJerk = -1;
        double m_actualVelocity, m_maxOutputSpeed;
        double m_timeToAccel, m_ticksToAccel;
        CORETimer m_timer;
    };
}