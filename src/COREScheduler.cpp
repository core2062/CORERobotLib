#include "COREScheduler.h"

using namespace CORE;
using namespace std;

CORESubsystem::CORESubsystem(string name) {
    m_name = name;
//    shared_ptr<CORESubsystem> pointer(this);
    COREScheduler::addSubsystem(this);

}

COREController::COREController() : CORETask() {

}

COREVariableControlledSubsystem::COREVariableControlledSubsystem(string name) : CORESubsystem(name) {
}

void COREVariableControlledSubsystem::teleop() {
	if(m_currentController != nullptr){
		//if(m_currentController->isEnabled()){
			m_currentController->enabledLoop();
		//}
	}
}

bool COREVariableControlledSubsystem::setController(
		COREController* controller) {
	if(controller == nullptr){
		m_currentController = nullptr;
		return false;
	}
	m_currentController = controller;
	return true;
}

vector<CORESubsystem*> COREScheduler::m_subsystems;
vector<COREAuton*> COREScheduler::m_autons;
vector<CORETask*> COREScheduler::m_tasks;
SendableChooser<COREAuton*>* COREScheduler::m_autonChooser;

COREAuton* COREScheduler::m_selectedAuton;
CORETimer COREScheduler::m_autonTimer;

void COREScheduler::addSubsystem(CORESubsystem* subsystem) {
    m_subsystems.push_back(subsystem);
    if(subsystem == nullptr) {
    	CORELog::logError("Subsystem added as null pointer!");
    } else {
    	CORELog::logInfo(subsystem->getName() + " subsystem added");
    }
}

void COREScheduler::addAuton(COREAuton* auton) {
    m_autons.push_back(auton);
    CORELog::logInfo(auton->getName() + " autonomous added");
}

void COREScheduler::addTask(CORETask* task) {
    m_tasks.push_back(task);
}

void COREScheduler::robotInit() {
    CORELog::robotInit();
    COREConstantsManager::updateConstants();
    for(auto subsystem : m_subsystems) {
        subsystem->robotInit();
    }
    for(auto task : m_tasks) {
        task->robotInitTask();
    }
    m_autonChooser = new SendableChooser<COREAuton*>();
    for(auto auton : m_autons) {
        auton->putToDashboard(m_autonChooser);
    }
    SmartDashboard::PutData("Autonomous", m_autonChooser);
    //TODO: Log -> RobotInit Complete
}

void COREScheduler::autonInit() {
    CORELog::autonInit();
    COREConstantsManager::updateConstants();
    for(auto task : m_tasks) {
        if(!task->isDisabled()) {
            task->autonInitTask();
        }
    }
    if(!m_autons.empty()) {
        m_selectedAuton = m_autonChooser->GetSelected();
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
	COREHardwareManager::updateEncoders();
    for(auto task : m_tasks) {
        if(!task->isDisabled()) {
            task->preLoopTask();
        }
    }
    if(m_selectedAuton != nullptr) {
        m_selectedAuton->auton();
        for(auto task : m_tasks) {
            if(!task->isDisabled()) {
                task->postLoopTask();
            }
        }
        COREHardwareManager::updateMotors();
        bool complete = m_selectedAuton->complete();
        if(complete) {
            CORELog::logInfo(m_selectedAuton->getName() + " autonomous complete! Took: "
                             + to_string(m_autonTimer.Get()) + " seconds");
        }
        return complete;
    } else {
        CORELog::logWarning("No autonomous selected!");
        return true;
    }
}

void COREScheduler::autonEnd() {
	for(auto task : m_tasks) {
		task->autonEndTask();
	}
}

void COREScheduler::teleopInit() {
    CORELog::teleopInit();
    COREConstantsManager::updateConstants();
    for(auto subsystem : m_subsystems) {
        subsystem->teleopInit();
    }
    for(auto task : m_tasks) {
        if(!task->isDisabled()) {
            task->teleopInitTask();
        }
    }
    //TODO: Log -> TeleopInit Complete
}

void COREScheduler::teleop() {
	COREHardwareManager::updateEncoders();
    for(auto task : m_tasks) {
        if(!task->isDisabled()) {
            task->preLoopTask();
        }
    }
    for(auto subsystem : m_subsystems) {
        subsystem->teleop();
    }
    for(auto task : m_tasks) {
        if(!task->isDisabled()) {
            task->postLoopTask();
        }
    }
    COREHardwareManager::updateMotors();
}

void COREScheduler::teleopEnd() {
    for(auto task : m_tasks) {
        if(!task->isDisabled()) {
            task->teleopEndTask();
        }
    }
    for(auto subsystem : m_subsystems) {
        subsystem->teleopEnd();
    }
    //TODO: Log -> TeleopEnd Complete
}

void COREScheduler::disabled() {
    CORELog::disabled();
    for(auto task : m_tasks) {
        if(!task->isDisabled()) {
            task->disabledTask();
        }
    }
    for(auto subsystem : m_subsystems) {
        subsystem->disabled();
    }
    COREHardwareManager::zeroMotors();
}

void COREScheduler::test() {
    for(auto subsystem : m_subsystems) {
        subsystem->test();
    }
    //TODO: Do something
}

void COREScheduler::cleanUp() {
	CORELog::logInfo("Cleaning up COREScheduler!");
	delete m_selectedAuton;
	for (auto i = m_autons.begin(); i != m_autons.end(); i++){
		delete *i;
	}
	m_autons.clear();
	for (auto i = m_tasks.begin(); i != m_tasks.end(); i++){
		delete *i;
	}
	m_tasks.clear();
	for (auto i = m_subsystems.begin(); i != m_subsystems.end(); i++){
		delete *i;
	}
	m_subsystems.clear();
	COREHardwareManager::cleanUp();
	COREConstantsManager::cleanUp();
}
