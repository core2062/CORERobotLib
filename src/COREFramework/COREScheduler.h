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

		virtual void RobotInit() = 0;
		virtual void TeleopInit() = 0;
		virtual void TestInit() {}
		virtual void Teleop() = 0;
		virtual void TeleopEnd() {}
		virtual void Test() {}
		virtual void Disabled() {}
		virtual void Log() {}

		COREJoystick* driverJoystick;
		COREJoystick* operatorJoystick;
	};

	class COREController: public CORETask {
	public:
		COREController();
		virtual ~COREController() {}

		virtual void EnabledLoop() = 0;
		virtual void Enable() {
			m_enabled = true;
		}
		virtual void Disable() {
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

		virtual void RobotInit() = 0;
		virtual void TeleopInit() = 0;
		virtual void Teleop() override;
		virtual void TeleopEnd() {}
		virtual void Test() {}
		virtual void Disabled() {}
		bool SetController(COREController * controller);

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
		static void AddSubsystem(CORESubsystem* subsystem);
		static void AddAuton(COREAuton * auton);
		static void AddTask(CORETask* task);

		static void RobotInit();
		static void Disabled();
		static void AutonInit();
		static bool Auton();
		static void TeleopInit();
		static void Teleop();
		static void TestInit();
		static void Test();
		static void Log();
		static void CleanUp();
	};
}
