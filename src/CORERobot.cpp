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
	double loopTime = loopTimer.Get() < TARGETLOOPTIME ? TARGETLOOPTIME - loopTimer.Get() : 0.0;
	//SmartDashboard::PutNumber("Timer", loopTimer.Get());
	if(loopTimer.Get() >= TARGETLOOPTIME*1.2) {
		//outLog.appendLog("[PROBLEM] Loop Time High! Timer at: ", loopTimer.Get());
	}
#ifdef NSIMULATION
	Wait(loopTime);
#else
	Sleep(loopTime*1000);
#endif
	loopTimer.Reset();
}

