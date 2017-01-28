#pragma once

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "COREAuton.h"
#include "COREHardware.h"
#include <WPILib.h>
#include "CORETask.h"

using namespace std;

namespace CORE {
class COREAuton;
class CORESubsystem {
public:
	CORESubsystem(string name);
	virtual void robotInit() = 0;
	virtual void teleopInit() = 0;
	virtual void teleop() = 0;
	virtual void teleopEnd() {
		//TODO: Log -> Teleop end not implemented for: NAME
	}
    virtual void disabled() {

    }
	virtual void test() {

	}
    string getName() {
        return m_name;
    }
	virtual ~CORESubsystem(){}

private:
    string m_name;
};

class COREScheduler {
private:
	static vector<std::shared_ptr<CORESubsystem>> m_subsystems;
    static vector<shared_ptr<COREAuton>> m_autons;
	static vector<std::shared_ptr<CORETask>> m_tasks;
    static shared_ptr<SendableChooser<COREAuton*>> m_autonChooser;
    static shared_ptr<COREAuton> m_selectedAuto;
public:
	static void addSubsystem(shared_ptr<CORESubsystem> subsystem);
    static void addAuton(shared_ptr<COREAuton> auton);
    static void addTask(std::shared_ptr<CORETask> task);
	static void robotInit();
    static void autonInit();
    static bool auton();
	static void teleopInit();
	static void teleop();
	static void teleopEnd();
	static void disabled();
	static void test();
};
}
