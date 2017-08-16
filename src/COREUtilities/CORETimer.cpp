#include "CORETimer.h"

using namespace CORE;
using namespace std::chrono;

double CORETimer::Get() {
    if(m_started && !m_stopped) {
        return duration_cast<duration<double>>(
                high_resolution_clock::now() - m_startTime).count();
    } else if(m_started && m_stopped) {
        return duration_cast<duration<double>>(m_stopTime - m_startTime).count();
    }
    //TODO: Log -> Error, m_timer has not m_started!
    return 0;
}

void CORETimer::Reset() {
    m_started = false;
    m_startTime = high_resolution_clock::now();
}

void CORETimer::Stop() {
    if(!m_started) {
        //TODO: Log -> Error, m_timer m_stopped without starting!
    } else {
        m_stopped = true;
        m_stopTime = high_resolution_clock::now();
    }
}

void CORETimer::Start() {
    m_started = true;
    m_startTime = high_resolution_clock::now();
}

double CORETimer::GetFPGATimestamp() {
    return duration_cast<seconds>(high_resolution_clock::now().time_since_epoch()).count();
}
