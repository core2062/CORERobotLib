#include "COREScheduler.h"
//#include "COREAuton.h"

using namespace CORE;
using namespace std;

CORESubsystem::CORESubsystem() {
	COREScheduler::addSubsystem(this);
}

COREController::COREController() : CORETask() {
}

COREVariableControlledSubsystem::COREVariableControlledSubsystem() {
}

void COREVariableControlledSubsystem::teleop() {
	if (m_currentController != nullptr) {
		//if(m_currentController->isEnabled()){
			m_currentController->enabledLoop();
		//}
	}
}

bool COREVariableControlledSubsystem::setController(COREController* controller) {
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
//SendableChooser<COREAuton*>* COREScheduler::m_autonChooser;

COREAuton* COREScheduler::m_selectedAuton;
CORETimer COREScheduler::m_autonTimer;

void COREScheduler::addSubsystem(CORESubsystem* subsystem) {
	if (!subsystem) {
		CORELog::logError("Subsystem is a null pointer!");
	} else {
		m_subsystems.push_back(subsystem);
	}
}

void COREScheduler::addAuton(COREAuton* auton) {
	m_autons.push_back(auton);
	CORELog::logInfo(auton->getName() + " autonomous added");
}

void COREScheduler::addTask(CORETask* task) {
	CORELog::logInfo("Adding task");
	m_tasks.push_back(task);
}

void COREScheduler::robotInit() {
	CORELog::robotInit();
	COREConstantsManager::updateConstants();
	m_driverJoystick = new COREJoystick(0);
	m_operatorJoystick = new COREJoystick(1);
	if (!m_driverJoystick) {
		CORELog::logError("Driver joystick is a null pointer!");
	}
	if (!m_operatorJoystick) {
		CORELog::logError("Operator joystick is a null pointer!");
	}
	for (auto subsystem : m_subsystems) {
		subsystem->driverJoystick = m_driverJoystick;
		subsystem->operatorJoystick = m_operatorJoystick;
		subsystem->robotInit();
	}
	for (auto task : m_tasks) {
		if (!task->isDisabled()) {
			task->robotInitTask();
		}
	}
//    m_autonChooser = new SendableChooser<COREAuton*>();
//    m_autonChooser->AddDefault("Do Nothing", nullptr);
//    for(auto auton : m_autons) {
//        auton->putToDashboard(m_autonChooser);
//    }
//    SmartDashboard::PutData("Autonomous", m_autonChooser);
}

void COREScheduler::disabled() {
	CORELog::disabled();
	for (auto task : m_tasks) {
		task->disabledTask();
	}
	for (auto subsystem : m_subsystems) {
		subsystem->disabled();
	}
	COREHardwareManager::zeroMotors();
}

void COREScheduler::autonInit() {
	CORELog::autonInit();
	COREConstantsManager::updateConstants();
	for (auto task : m_tasks) {
		if (!task->isDisabled()) {
			task->autonInitTask();
		}
	}
	if (!m_autons.empty()) {
//        m_selectedAuton = m_autonChooser->GetSelected();
		if (m_selectedAuton == nullptr) {
			return;
		}
		m_selectedAuton->autonInit();
		CORELog::logInfo("Starting " + m_selectedAuton->getName() + " autonomous");
	} else {
		CORELog::logWarning("No autonomous routines added!");
		m_selectedAuton = nullptr;
	}
	m_autonTimer.Reset();
	m_autonTimer.Start();
}

bool COREScheduler::auton() {
	for (auto task : m_tasks) {
		if (!task->isDisabled()) {
			task->preLoopTask();
		}
	}
	if (m_selectedAuton != nullptr) {
		m_selectedAuton->auton();
		for (auto task : m_tasks) {
			if (!task->isDisabled()) {
				task->postLoopTask();
			}
		}
		bool complete = m_selectedAuton->complete();
		if (complete) {
			CORELog::logInfo(m_selectedAuton->getName() + " autonomous complete! Took: " + to_string(m_autonTimer.Get()) + " seconds");
		}
		return complete;
	} else {
		CORELog::logWarning("No autonomous selected!");
		return true;
	}
}

void COREScheduler::teleopInit() {
	CORELog::teleopInit();
	COREConstantsManager::updateConstants();
	for (auto subsystem : m_subsystems) {
		subsystem->teleopInit();
	}
	for (auto task : m_tasks) {
		if (!task->isDisabled()) {
			task->teleopInitTask();
		}
	}
}

void COREScheduler::teleop() {
	m_driverJoystick->preLoopTask();
	m_operatorJoystick->preLoopTask();
	for (auto task : m_tasks) {
		if (!task->isDisabled()) {
			task->preLoopTask();
		}
	}
	for (auto subsystem : m_subsystems) {
		subsystem->teleop();
	}
}

void COREScheduler::testInit() {
	CORELog::testInit();
	COREConstantsManager::updateConstants();
	for (auto subsystem : m_subsystems) {
		subsystem->testInit();
	}
	for (auto task : m_tasks) {
		if (!task->isDisabled()) {
			task->testInitTask();
		}
	}
}

void COREScheduler::test() {
	for (auto task : m_tasks) {
		if (!task->isDisabled()) {
			task->testTask();
		}
	}
	for (auto subsystem : m_subsystems) {
		subsystem->test();
	}
}

void COREScheduler::cleanUp() {
	CORELog::logInfo("Cleaning up COREScheduler!");
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
	COREConstantsManager::cleanUp();
}
