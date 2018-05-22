#pragma once

#include "COREController.h"
#include <memory>

using namespace std;

namespace CORE {
    class COREMotionProfile : COREMotionController {
    public:
        COREMotionProfile(ControllerInput* inputDevice, ControllerOutput* outputDevice, double maxVel, double maxAccel);
        COREMotionProfile(shared_ptr<ControllerInput> inputDevice, shared_ptr<ControllerOutput> outputDevice, double maxVel, double maxAccel);
        COREMotionProfile(double maxVel, double maxAccel);
        void SetMaxVel(double maxVelocity);
        void SetMaxAccel(double maxAcceleration);
        double GetMaxVel();
        double GetMaxAccel();
        void Set(double setPoint) override;
        double Get() override;
        void SetActual(double actualPosition) override;
        double GetActual() override;
        void update() /*override*/;
    private:
        double m_maxVel = -1;
        double m_maxAccel = -1;
        double m_maxJerk = -1;
        double m_output = 0;
        double m_actualVelocity, m_maxOutputSpeed;
        double m_timeToAccel, m_ticksToAccel;
        CORETimer m_timer;
    };
}
