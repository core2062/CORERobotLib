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
		virtual void test() {}
		virtual void disabled() {}
        virtual string getName() {
        	return m_name;
        }
        virtual ~CORESubsystem() {}
    private:
        string m_name;
    };

	class COREController : public CORETask {
	public:
		COREController();

		virtual void enabledLoop() = 0;
		virtual void enable() {
			m_enabled = true;
		}
		virtual void disable() {
			m_enabled = false;
		}
		bool isEnabled() {
			return m_enabled;
		}
		virtual ~COREController() {}
	protected:
		bool m_enabled = false;
	};

	class COREVariableControlledSubsystem : public CORESubsystem {
	public:
		COREVariableControlledSubsystem(string name);
		virtual void robotInit() = 0;
		virtual void teleopInit() = 0;
		virtual void teleop() override;
		virtual void teleopEnd() {}
		virtual void test() {}
		virtual void disabled() {}
		bool setController(COREController * controller);
		string getName() {
			return m_name;
		}
		virtual ~COREVariableControlledSubsystem(){}

	protected:
		string m_name;
		COREController* m_currentController = 0;
	};

	class COREScheduler {
	private:
		static vector<COREAuton*> m_autons;
		static vector<CORETask*> m_tasks;
		static SendableChooser<COREAuton*>* m_autonChooser;
		static vector<CORESubsystem*> m_subsystems;
		static COREAuton* m_selectedAuton;
		static CORETimer m_autonTimer;
	public:
		static void addSubsystem(CORESubsystem* subsystem);
		static void addAuton(COREAuton * auton);
		static void addTask(CORETask* task);
		static void robotInit();
		static void autonInit();
		static bool auton();
		static void autonEnd();
		static void teleopInit();
		static void teleop();
		static void teleopEnd();
		static void disabled();
		static void test();
		static void cleanUp();
	};
}
