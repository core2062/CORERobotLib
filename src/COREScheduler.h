#pragma once

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "COREAuto.h"
#include "COREHardware.h"
#include <WPILib.h>
#include "CORETask.h"

using namespace std;

namespace CORE {
class COREAuto;
class CORESubsystem {
public:
	CORESubsystem();
	virtual void robotInit() = 0;
	virtual void teleopInit() = 0;
	virtual void teleop() = 0;
	virtual void teleopEnd() {
		//TODO: Log -> Teleop end not implemented for: NAME
	}
	virtual void test() {

	}
	virtual ~CORESubsystem(){}
};

class COREScheduler {
private:
	static vector<std::shared_ptr<CORESubsystem>> m_subsystems;
    static vector<shared_ptr<COREAuto>> m_autons;
	static vector<std::shared_ptr<CORETask>> m_tasks;
    static shared_ptr<SendableChooser<COREAuto*>> m_autonChooser;
    static shared_ptr<COREAuto> m_selectedAuto;
public:
	static void addSubsystem(shared_ptr<CORESubsystem> subsystem);
    static void addAuton(shared_ptr<COREAuto> auton);
    static void addTask(std::shared_ptr<CORETask> task);
	static void robotInit();
    static void autonInit();
    static bool auton();
	static void teleopInit();
	static void teleop();
	static void teleopEnd();
	static void test();
};
}
