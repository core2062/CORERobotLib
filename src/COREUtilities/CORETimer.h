#pragma once

#include <chrono>
#include <time.h>

#ifdef ARM
#include "WPILib.h"
#endif

namespace CORE {

    class CORETimer {
    private:
        double m_startTime, m_stopTime;
        bool m_started = false;
        bool m_stopped = false;
    public:
        static double getTime();
        double Get();
        void Stop();
        void Reset();
        void Start();
    };

}