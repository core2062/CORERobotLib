#include "CORERobot.h"
#include <iostream>

using namespace CORE;

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
	Wait(loopTime);
	loopTimer.Reset();
	loopTimer.Start();
}

void CORERobot::RobotInit() {
	CORESubsystemsManager::robotInit();
}

void CORERobot::Disabled() {

}

void CORERobot::Autonomous() {

}

void CORERobot::OperatorControl() {
	CORESubsystemsManager::teleopInit();
#ifdef __arm__
	while(IsOperatorControl() && IsEnabled()) {
#else
	while(true) {
#endif
		CORESubsystemsManager::teleop();
		waitLoopTime();
	}
	CORESubsystemsManager::teleopEnd();
}

void CORERobot::Test() {
	CORESubsystemsManager::test();
}

