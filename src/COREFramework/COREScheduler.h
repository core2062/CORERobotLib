#pragma once

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <functional>

#include "COREAuton.h"
#include "COREHardwareManager.h"
#include "COREUtilities/COREConstant.h"
#include "CORETask.h"
#include "CORELogging/CORELog.h"
#include "COREUtilities/CORENamedObject.h"
#include "COREHardware/COREJoystick.h"
#include "COREDashboard/COREDashboard.h"

using namespace std;
using namespace std::placeholders;

using namespace CORE;

namespace CORE {
	class COREAuton;

	class CORESubsystem: public CORENamedObject {
	public:
		CORESubsystem();
		virtual ~CORESubsystem() {}

		virtual void robotInit() = 0;
		virtual void teleopInit() = 0;
		virtual void testInit() {}
		virtual void teleop() = 0;
		virtual void teleopEnd() {}
		virtual void test() {}
		virtual void disabled() {}

		COREJoystick* driverJoystick;
		COREJoystick* operatorJoystick;
	};

	class COREController: public CORETask {
	public:
		COREController();
		virtual ~COREController() {}

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

	protected:
		bool m_enabled = false;
	};

	class COREVariableControlledSubsystem: public CORESubsystem {
	public:
		COREVariableControlledSubsystem();
		virtual ~COREVariableControlledSubsystem() {}

		virtual void robotInit() = 0;
		virtual void teleopInit() = 0;
		virtual void teleop() override;
		virtual void teleopEnd() {}
		virtual void test() {}
		virtual void disabled() {}
		bool setController(COREController * controller);

	protected:
		COREController* m_currentController = 0;
	};

	class COREScheduler {
	private:
		static vector<COREAuton*> m_autons;
		static vector<CORETask*> m_tasks;
		static frc::SendableChooser<COREAuton*>* m_autonChooser;
		static vector<CORESubsystem*> m_subsystems;
		static COREAuton* m_selectedAuton;
		static CORETimer m_autonTimer;
		static COREJoystick* m_driverJoystick;
		static COREJoystick* m_operatorJoystick;

	public:
		static void addSubsystem(CORESubsystem* subsystem);
		static void addAuton(COREAuton * auton);
		static void addTask(CORETask* task);

		static void robotInit();
		static void disabled();
		static void autonInit();
		static bool auton();
		static void teleopInit();
		static void teleop();
		static void testInit();
		static void test();
		static void cleanUp();
	};
}
