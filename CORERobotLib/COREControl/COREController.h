#pragma once

#include "COREUtilities/CORETimer.h"
#include <memory>

using namespace std;

/*#include "COREControl/COREPID.h"
#include "COREControl/COREPosPID.h"
#include "COREControl/COREVelPID.h"
#include "COREControl/COREAngPID.h"*/

namespace CORE {
    class ControllerInput {
    private:
        double m_lastPos = 0;
        double m_ticksPerDegree = 0;
        CORETimer* m_timer;
    public:
        virtual double ControllerGetPos() {
            return 0;
        }

        virtual double ControllerGetVel() {
            if(m_timer == nullptr) {
                m_timer = new CORETimer();
                return 0;
            } else {
                double time = m_timer->Get();
                m_timer->Reset();
                m_timer->Start();
                double currentPos = ControllerGetPos();
                double vel = (currentPos - m_lastPos) / time;
                m_lastPos = currentPos;
                return vel;
            }
        }

        virtual double ControllerGetAng() {
            return ControllerGetPos() * m_ticksPerDegree;
        }

        void setTicksPerRotation(double ticks) {
            m_ticksPerDegree = 360.0 / ticks;
        }

        virtual ~ControllerInput() {}
    };

    class ControllerOutput {
    public:
        virtual void ControllerSet(double value) = 0;
        virtual ~ControllerOutput() {}
    };

    class COREMotionController {
    public:
        virtual void Set(double setPoint);
        virtual double Get();
        virtual void setActual(double actualPosition);
        virtual double getActual();
        virtual ~COREMotionController() {}
    protected:
        virtual void update(int profile = -1) = 0;
        shared_ptr<ControllerInput> m_inputDevice;
        shared_ptr<ControllerOutput> m_outputDevice;
        double m_actualPosition = 0;
        double m_setPoint = 0;
    };
}
