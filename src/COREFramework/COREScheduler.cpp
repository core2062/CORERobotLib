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
vector<function<void()>> COREScheduler::m_robotInitCallBacks;
vector<function<void()>> COREScheduler::m_autonInitCallBacks;
vector<function<void()>> COREScheduler::m_autonEndCallBacks;
vector<function<void()>> COREScheduler::m_teleopInitCallBacks;
vector<function<void()>> COREScheduler::m_preLoopCallBacks;
vector<function<void()>> COREScheduler::m_postLoopCallBacks;
vector<function<void()>> COREScheduler::m_teleopEndCallBacks;
vector<function<void()>> COREScheduler::m_disabledCallBacks;
COREJoystick* COREScheduler::m_driverJoystick;
COREJoystick* COREScheduler::m_operatorJoystick;
//SendableChooser<COREAuton*>* COREScheduler::m_autonChooser;

COREAuton* COREScheduler::m_selectedAuton;
CORETimer COREScheduler::m_autonTimer;

void COREScheduler::addSubsystem(CORESubsystem* subsystem) {
    if(!subsystem) {
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
    m_tasks.push_back(task);
}

void COREScheduler::robotInit() {
    CORELog::robotInit();
    COREConstantsManager::updateConstants();
    m_driverJoystick = new COREJoystick(0);
    m_operatorJoystick = new COREJoystick(1);
    if(!m_driverJoystick) {
        CORELog::logError("Driver joystick is a null pointer!");
    }
    if(!m_operatorJoystick) {
        CORELog::logError("Operator joystick is a null pointer!");
    }
    for(auto subsystem : m_subsystems) {
        subsystem->driverJoystick = m_driverJoystick;
        subsystem->operatorJoystick = m_operatorJoystick;
        subsystem->robotInit();
    }
    for(auto task : m_tasks) {
        if(!task->isDisabled()) {
            task->robotInitTask();
        }
    }
    for(auto callBack : m_robotInitCallBacks) {
        callBack();
    }
//    m_autonChooser = new SendableChooser<COREAuton*>();
//    m_autonChooser->AddDefault("Do Nothing", nullptr);
//    for(auto auton : m_autons) {
//        auton->putToDashboard(m_autonChooser);
//    }
//    SmartDashboard::PutData("Autonomous", m_autonChooser);
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
    for(auto callBack : m_autonInitCallBacks) {
        callBack();
    }
    if(!m_autons.empty()) {
//        m_selectedAuton = m_autonChooser->GetSelected();
        if(m_selectedAuton == nullptr) {
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
//	COREHardwareManager::updateEncoders();
    for(auto task : m_tasks) {
        if(!task->isDisabled()) {
            task->preLoopTask();
        }
    }
    for(auto callBack : m_preLoopCallBacks) {
        callBack();
    }
    if(m_selectedAuton != nullptr) {
        m_selectedAuton->auton();
        for(auto task : m_tasks) {
            if(!task->isDisabled()) {
                task->postLoopTask();
            }
        }
        for(auto callBack : m_postLoopCallBacks) {
            callBack();
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
        if(!task->isDisabled()) {
            task->autonEndTask();
        }
	}
    for(auto callBack : m_autonEndCallBacks) {
        callBack();
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
    for(auto callBack : m_teleopInitCallBacks) {
        callBack();
    }
    //TODO: Log -> TeleopInit Complete
}

void COREScheduler::teleop() {
    for(auto subsystem : m_subsystems) {
        subsystem->teleop();
    }
}

void COREScheduler::teleopEnd() {
    for(auto task : m_tasks) {
        if(!task->isDisabled()) {
            task->teleopEndTask();
        }
    }
    for(auto callBack : m_teleopEndCallBacks) {
        callBack();
    }
    for(auto subsystem : m_subsystems) {
        subsystem->teleopEnd();
    }
    //TODO: Log -> TeleopEnd Complete
}

void COREScheduler::disabled() {
    CORELog::disabled();
    for(auto task : m_tasks) {
    	task->disabledTask();

    }
    for(auto callBack : m_disabledCallBacks) {
        callBack();
    }
    for(auto subsystem : m_subsystems) {
        subsystem->disabled();
    }
    COREHardwareManager::zeroMotors();
}

void COREScheduler::test() {
	for(auto task : m_tasks) {
		if(!task->isDisabled()) {
			task->disabledTask();
		}
	}
	for(auto callBack : m_testCallBacks) {
		callBack();
	}
	for(auto subsystem : m_subsystems) {
        subsystem->test();
    }
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
//	COREHardwareManager::cleanUp();
	COREConstantsManager::cleanUp();
}

template<class T>
void COREScheduler::addRobotInitCallBack(T* const object, void(T::* const callBack)()) {
    m_robotInitCallBacks.emplace_back(bind(callBack, object, _1, _2));
}

template<class T>
void COREScheduler::addAutonInitCallBack(T* const object, void(T::* const callBack)()) {
    m_autonInitCallBacks.emplace_back(bind(callBack, object, _1, _2));
}

template<class T>
void COREScheduler::addAutonEndCallBack(T* const object, void(T::* const callBack)()) {
    m_autonEndCallBacks.emplace_back(bind(callBack, object, _1, _2));
}

template<class T>
void COREScheduler::addTeleopInitCallBack(T* const object, void(T::* const callBack)()) {
    m_teleopInitCallBacks.emplace_back(bind(callBack, object, _1, _2));
}

template<class T>
void COREScheduler::addPreLoopCallBack(T* const object, void(T::* const callBack)()) {
    m_preLoopCallBacks.emplace_back(bind(callBack, object, _1, _2));
}

template<class T>
void COREScheduler::addPostLoopCallBack(T* const object, void(T::* const callBack)()) {
    m_postLoopCallBacks.emplace_back(bind(callBack, object, _1, _2));
}

template<class T>
void COREScheduler::addTeleopEndCallBack(T* const object, void(T::* const callBack)()) {
    m_teleopEndCallBacks.emplace_back(bind(callBack, object, _1, _2));
}

template<class T>
void COREScheduler::addDisabledCallBack(T* const object, void(T::* const callBack)()) {
    m_disabledCallBacks.emplace_back(bind(callBack, object, _1, _2));
}

template<class T>
void COREScheduler::addTestCallBack(T* const object, void(T::* const callBack)()) {
    m_testCallBacks.emplace_back(bind(callBack, object, _1, _2));
}

template<class T>
void COREScheduler::addTestInitCallBack(T* const object, void(T::* const callBack)()) {
    m_testInitCallBacks.emplace_back(bind(callBack, object, _1, _2));
}

template<class T>
void COREScheduler::addTestEndCallBack(T* const object, void(T::* const callBack)()) {
    m_testEndCallBacks.emplace_back(bind(callBack, object, _1, _2));
}


void COREScheduler::preLoop() {
    COREHardwareManager::updateEncoders();
    for(auto task : m_tasks) {
        if(!task->isDisabled()) {
            task->preLoopTask();
        }
    }
    for(auto callBack : m_preLoopCallBacks) {
        callBack();
    }
}

void COREScheduler::postLoop() {
    for(auto task : m_tasks) {
        if(!task->isDisabled()) {
            task->postLoopTask();
        }
    }
    for(auto callBack : m_postLoopCallBacks) {
        callBack();
    }
    COREHardwareManager::updateMotors();
}
void COREScheduler::testInit(){
	CORELog::testInit();
	COREConstantsManager::updateConstants();
	for(auto subsystem : m_subsystems) {
		subsystem->testInit();
	}
	for(auto task : m_tasks) {
		if(!task->isDisabled()) {
			task->testInitTask();
		}
	}
	for(auto callBack : m_testInitCallBacks) {
		callBack();
	}
}

