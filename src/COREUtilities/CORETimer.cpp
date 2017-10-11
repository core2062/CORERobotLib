#include <iostream>
#include "CORETimer.h"

using namespace CORE;
using namespace std::chrono;

double CORETimer::Get() {
    if(m_started && !m_stopped) {
        return getTime() - m_startTime;
    } else if(m_started && m_stopped) {
        return m_stopTime - m_startTime;
    }
    //TODO: Log -> Error, m_timer has not m_started!
    return 0;
}

void CORETimer::Reset() {
    m_started = false;
    m_startTime = getTime();
}

void CORETimer::Stop() {
    if(!m_started) {
        //TODO: Log -> Error, m_timer m_stopped without starting!
    } else {
        m_stopped = true;
        m_stopTime = getTime();
    }
}

void CORETimer::Start() {
    m_started = true;
    m_startTime = getTime();
}

double CORETimer::getTime() {

    return duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count() / 1000000.0;

    //return frc::GetFPGATime();


}
