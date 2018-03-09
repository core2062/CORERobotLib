#pragma once

#include <chrono>
#include <time.h>

#ifdef ARM
#include "WPILib.h"
#endif

namespace CORE {

    class CORETimer {
    private:
        double m_startTime = 0;
        double m_stopTime = 0;
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