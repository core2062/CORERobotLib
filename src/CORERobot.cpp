#include "CORERobot.h"
#include <iostream>

#ifndef __arm__
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif
#else
#include "WPILib.h"
#endif

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
#ifdef __arm__
	Wait(loopTime);
#else
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
	Sleep(DWORD(loopTime*1000));
#else
	usleep(loopTime*1000000);
#endif
#endif
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
#ifdef NSIMULATION
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

