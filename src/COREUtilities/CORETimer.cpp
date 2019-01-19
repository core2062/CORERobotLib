#include <iostream>
#include "CORETimer.h"

using namespace CORE;
using namespace std::chrono;

double CORETimer::Get() {
    if(m_started && !m_stopped) {
        return GetTime() - m_startTime;
    } else if(m_started && m_stopped) {
        return m_stopTime - m_startTime;
    }
    //TODO: Log -> Error, m_timer has not m_started!
    return 0;
}

void CORETimer::Reset() {
    m_started = false;
    m_startTime = GetTime();
}

void CORETimer::Stop() {
    if(!m_started) {
        //TODO: Log -> Error, m_timer m_stopped without starting!
    } else {
        m_stopped = true;
        m_stopTime = GetTime();
    }
}

void CORETimer::Start() {
    m_started = true;
    m_startTime = GetTime();
}

double CORETimer::GetTime() {

    return duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count() / 1000000.0;

    //return frc::GetFPGATime();


}
