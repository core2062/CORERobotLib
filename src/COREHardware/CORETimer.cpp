#include "CORETimer.h"

using namespace CORE;

double CORETimer::Get() {
    if (m_started && !m_stopped) {
        return std::chrono::duration_cast<std::chrono::duration<double>>(
                std::chrono::high_resolution_clock::now() - m_startTime).count();
    } else if (m_started && m_stopped) {
        return std::chrono::duration_cast<std::chrono::duration<double>>(m_stopTime - m_startTime).count();
	}
    //TODO: Log -> Error, m_timer has not m_started!
	return -1;
}
void CORETimer::Reset() {
    m_started = false;
    m_startTime = std::chrono::high_resolution_clock::now();
}
void CORETimer::Stop() {
    if (!m_started) {
        //TODO: Log -> Error, m_timer m_stopped without starting!
	}
	else {
        m_stopped = true;
        m_stopTime = std::chrono::high_resolution_clock::now();
	}
}
void CORETimer::Start() {
    m_started = true;
    m_startTime = std::chrono::high_resolution_clock::now();
}
