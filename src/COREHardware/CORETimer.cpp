#include "CORETimer.h"

using namespace CORE;

double CORETimer::Get() {
	if(started && !stopped) {
		return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - startTime).count();
	}
	else if(started && stopped) {
		return std::chrono::duration_cast<std::chrono::duration<double>>(stopTime - startTime).count();
	}
	//TODO: Log -> Error, timer has not started!
	return -1;
}
void CORETimer::Reset() {
	started = false;
	startTime = std::chrono::high_resolution_clock::now();
}
void CORETimer::Stop() {
	if(!started) {
		//TODO: Log -> Error, timer stopped without starting!
	}
	else {
		stopped = true;
		stopTime = std::chrono::high_resolution_clock::now();
	}
}
void CORETimer::Start() {
	started = true;
	startTime = std::chrono::high_resolution_clock::now();
}
