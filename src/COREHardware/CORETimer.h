#pragma once

#include <chrono>

namespace CORE {

class CORETimer {
private:
	std::chrono::high_resolution_clock::time_point m_startTime, m_stopTime;
	bool m_started = false;
	bool m_stopped = false;
public:
	double Get();
	void Stop();
	void Reset();
	void Start();
};

}