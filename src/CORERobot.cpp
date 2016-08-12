#include "CORERobot.h"
#include <windows.h>

CORERobot::CORERobot()
{

}

void CORERobot::waitLoopTime() {
	if(!loopStarted) {
		loopTimer.Reset();
		loopTimer.Start();
		loopStarted = true;
	}
	double loopTime = loopTimer.Get()<.1 ? 0.1 - loopTimer.Get() : 0.0;
	//SmartDashboard::PutNumber("Timer", loopTimer.Get());
	if(loopTimer.Get() >= .12) {
		//outLog.appendLog("[PROBLEM] Loop Time High! Timer at: ", loopTimer.Get());
	}
	loopTimer.Reset();
#ifdef NSIMULATION
	Wait(loopTime);
#else
	Sleep(loopTime);
#endif
}

