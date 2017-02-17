#pragma once

#include <vector>
#include <string>
#include <memory>
#include <iostream>

#include <WPILib.h>
#include "COREAuton.h"
#include "COREHardware.h"
#include "COREConstant.h"
#include "CORETask.h"
#include "CORELog.h"

using namespace std;

namespace CORE {
    class COREAuton;
    class CORESubsystem {
    public:
        CORESubsystem(string name);
        virtual void robotInit() = 0;
        virtual void teleopInit() = 0;
        virtual void teleop() = 0;

        virtual void teleopEnd() {}

class COREController : public CORETask{
public:
	COREController();

	virtual void enabledLoop() = 0;
	virtual void enable(){};
	virtual void disable(){};

	bool isEnabled(){
		return m_enabled;
	}
	virtual ~COREController(){};
protected:
	bool m_enabled = false;
};

class COREVariableControlledSubsystem : public CORESubsystem{
public:
	COREVariableControlledSubsystem(string name);
	virtual void robotInit() = 0;
	virtual void teleopInit() = 0;
	virtual void teleop();
	virtual void teleopEnd() {}
    virtual void disabled() {}
	virtual void test() {}
	bool setController(string id);
	bool setController(COREController * controller);
    string getName() {
        return m_name;
    }
	virtual ~COREVariableControlledSubsystem(){}

private:
    string m_name;
    COREController* m_currentController = 0;
};

class COREScheduler {
private:
    static vector<COREAuton*> m_autons;
	static vector<std::shared_ptr<CORETask>> m_tasks;
	static shared_ptr<SendableChooser<COREAuton*>> m_autonChooser;
    static COREAuton* m_selectedAuton;
    static CORETimer m_autonTimer;
public:
	static void addSubsystem(shared_ptr<CORESubsystem> subsystem);
    static void addAuton(COREAuton * auton);
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
	static vector<std::shared_ptr<CORESubsystem>> m_subsystems;
}
