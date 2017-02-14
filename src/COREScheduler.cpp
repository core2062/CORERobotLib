#include "COREScheduler.h"

using namespace CORE;
using namespace std;

CORESubsystem::CORESubsystem(string name) {
    m_name = name;
    shared_ptr<CORESubsystem> pointer(this);
    COREScheduler::addSubsystem(pointer);

}

CORETask::CORETask() :
        m_disabled(false) {
    shared_ptr<CORETask> pointer(this);
    COREScheduler::addTask(pointer);
}

void CORETask::disableTasks(bool disable) {
    m_disabled = disable;
}

bool CORETask::isDisabled() {
    return m_disabled;
}

vector<shared_ptr<CORESubsystem>> COREScheduler::m_subsystems;
vector<COREAuton*> COREScheduler::m_autons;
vector<shared_ptr<CORETask>> COREScheduler::m_tasks;
shared_ptr<SendableChooser<COREAuton*>> COREScheduler::m_autonChooser;

COREAuton* COREScheduler::m_selectedAuton;
CORETimer COREScheduler::m_autonTimer;

void COREScheduler::addSubsystem(shared_ptr<CORESubsystem> subsystem) {
    m_subsystems.push_back(subsystem);
    CORELog::logInfo(subsystem->getName() + " subsystem added");
}

void COREScheduler::addAuton(COREAuton* auton) {
    m_autons.push_back(auton);
    CORELog::logInfo(auton->getName() + " autonomous added");
}

void COREScheduler::addTask(shared_ptr<CORETask> task) {
    m_tasks.push_back(task);
}

void COREScheduler::robotInit() {
    CORELog::robotInit();
    COREConstantsManager::robotInit();
    for(auto subsystem : m_subsystems) {
        subsystem->robotInit();
    }
    for(auto task : m_tasks) {
        task->robotInitTask();
    }
    m_autonChooser = make_shared<SendableChooser<COREAuton*>>();
    for(auto auton : m_autons) {
        auton->putToDashboard(m_autonChooser);
    }
    SmartDashboard::PutData("Autonomous", m_autonChooser.get());
    //TODO: Log -> RobotInit Complete
}

void COREScheduler::autonInit() {
    CORELog::autonInit();
    for(auto task : m_tasks) {
        if(!task->isDisabled()) {
            task->autonInitTask();
        }
    }
    if(!m_autons.empty()) {
#ifdef __arm__
        m_selectedAuton = m_autonChooser->GetSelected();
#else
        m_selectedAuton = m_autons[0];
        //TODO: Simulated auto switcher
#endif
        m_selectedAuton->autonInit();
    } else {
        CORELog::logWarning("No autonomous routines added!");
        m_selectedAuton = nullptr;
    }
    m_autonTimer.Reset();
    m_autonTimer.Start();
}

bool COREScheduler::auton() {
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
