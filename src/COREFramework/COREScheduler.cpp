#include "COREScheduler.h"
//#include "COREAuton.h"

using namespace CORE;
using namespace std;

CORESubsystem::CORESubsystem() {
	COREScheduler::AddSubsystem(this);
}

COREController::COREController() : CORETask() {
}

COREVariableControlledSubsystem::COREVariableControlledSubsystem() {
}

void COREVariableControlledSubsystem::Teleop() {
	if (m_currentController != nullptr) {
		//if(m_currentController->isEnabled()){
			m_currentController->EnabledLoop();
		//}
	}
}

bool COREVariableControlledSubsystem::SetController(COREController* controller) {
	if (controller == nullptr) {
		m_currentController = nullptr;
		return false;
	}
	m_currentController = controller;
	return true;
}

vector<CORESubsystem*> COREScheduler::m_subsystems;
vector<COREAuton*> COREScheduler::m_autons;
vector<CORETask*> COREScheduler::m_tasks;
COREJoystick* COREScheduler::m_driverJoystick;
COREJoystick* COREScheduler::m_operatorJoystick;
frc::SendableChooser<COREAuton*>* COREScheduler::m_autonChooser;

COREAuton* COREScheduler::m_selectedAuton;
CORETimer COREScheduler::m_autonTimer;

void COREScheduler::AddSubsystem(CORESubsystem* subsystem) {
	if (!subsystem) {
		CORELog::LogError("Subsystem is a null pointer!");
	} else {
		m_subsystems.push_back(subsystem);
	}
}

void COREScheduler::AddAuton(COREAuton* auton) {
	m_autons.push_back(auton);
	CORELog::LogInfo(auton->GetName() + " autonomous added");
}

void COREScheduler::AddTask(CORETask* task) {
	CORELog::LogInfo("Adding task");
	m_tasks.push_back(task);
}

void COREScheduler::RobotInit() {
	CORELog::RobotInit();
	COREConstantsManager::UpdateConstants();
	// COREDashboard::robotInit();
	m_driverJoystick = new COREJoystick(0);
	m_operatorJoystick = new COREJoystick(1);
	if (!m_driverJoystick) {
		CORELog::LogError("Driver joystick is a null pointer!");
	}
	if (!m_operatorJoystick) {
		CORELog::LogError("Operator joystick is a null pointer!");
	}
	for (auto subsystem : m_subsystems) {
		subsystem->driverJoystick = m_driverJoystick;
		subsystem->operatorJoystick = m_operatorJoystick;
		subsystem->RobotInit();
	}
	for (auto task : m_tasks) {
		if (!task->IsDisabled()) {
			task->RobotInitTask();
		}
	}
    m_autonChooser = new frc::SendableChooser<COREAuton*>();
    m_autonChooser->SetDefaultOption("Do Nothing", nullptr);
    for(auto auton : m_autons) {
        auton->PutToDashboard(m_autonChooser);
    }
    frc::SmartDashboard::PutData("Autonomous", m_autonChooser);
}

void COREScheduler::Disabled() {
	CORELog::Disabled();
	for (auto task : m_tasks) {
		task->DisabledTask();
	}
	for (auto subsystem : m_subsystems) {
		subsystem->Disabled();
	}
	COREHardwareManager::ZeroMotors();
}

void COREScheduler::AutonInit() {
	CORELog::AutonInit();
	COREConstantsManager::UpdateConstants();
	for (auto task : m_tasks) {
		if (!task->IsDisabled()) {
			task->AutonInitTask();
		}
	}
	if (!m_autons.empty()) {
        m_selectedAuton = m_autonChooser->GetSelected();
		if (m_selectedAuton == nullptr) {
			return;
		}
		m_selectedAuton->AutonInit();
		CORELog::LogInfo("Starting " + m_selectedAuton->GetName() + " autonomous");
	} else {
		CORELog::LogWarning("No autonomous routines added!");
		m_selectedAuton = nullptr;
	}
	m_autonTimer.Reset();
	m_autonTimer.Start();
}

bool COREScheduler::Auton() {
	for (auto task : m_tasks) {
		if (!task->IsDisabled()) {
			task->PreLoopTask();
		}
	}
	if (m_selectedAuton != nullptr) {
		m_selectedAuton->Auton();
		for (auto task : m_tasks) {
			if (!task->IsDisabled()) {
				task->PostLoopTask();
			}
		}
		bool complete = m_selectedAuton->Complete();
		if (complete) {
			CORELog::LogInfo(m_selectedAuton->GetName() + " autonomous complete! Took: " + to_string(m_autonTimer.Get()) + " seconds");
		}
		return complete;
	} else {
		CORELog::LogWarning("No autonomous selected!");
		return true;
	}
}

void COREScheduler::TeleopInit() {
	CORELog::TeleopInit();
	COREConstantsManager::UpdateConstants();
	for (auto subsystem : m_subsystems) {
		subsystem->TeleopInit();
	}
	for (auto task : m_tasks) {
		if (!task->IsDisabled()) {
			task->TeleopInitTask();
		}
	}
}

void COREScheduler::Teleop() {
	for (auto task : m_tasks) {
		if (!task->IsDisabled()) {
			task->PreLoopTask();
		}
	}
	for (auto subsystem : m_subsystems) {
		subsystem->Teleop();
	}
	for (auto task : m_tasks) {
		if (!task->IsDisabled()) {
			task->PostLoopTask();
		}
	}
}

void COREScheduler::TestInit() {
	CORELog::TestInit();
	COREConstantsManager::UpdateConstants();
	for (auto subsystem : m_subsystems) {
		subsystem->TestInit();
	}
	for (auto task : m_tasks) {
		if (!task->IsDisabled()) {
			task->TestInitTask();
		}
	}
}

void COREScheduler::Test() {
	for (auto task : m_tasks) {
		if (!task->IsDisabled()) {
			task->TestTask();
		}
	}
	for (auto subsystem : m_subsystems) {
		subsystem->Test();
	}
}

void COREScheduler::CleanUp() {
	CORELog::LogInfo("Cleaning up COREScheduler!");
	delete m_selectedAuton;
	for (auto i = m_autons.begin(); i != m_autons.end(); i++) {
		delete *i;
	}
	m_autons.clear();
	for (auto i = m_tasks.begin(); i != m_tasks.end(); i++) {
		delete *i;
	}
	m_tasks.clear();
	for (auto i = m_subsystems.begin(); i != m_subsystems.end(); i++) {
		delete *i;
	}
	m_subsystems.clear();
	COREConstantsManager::CleanUp();
}
void COREScheduler::Log() {
	for (auto subsystem : m_subsystems) {
		subsystem->Log();
	}
}
