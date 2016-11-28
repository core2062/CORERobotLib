#include "COREScheduler.h"

using namespace CORE;
using namespace std;

CORESubsystem::CORESubsystem() {
    shared_ptr<CORESubsystem> pointer(this);
	COREScheduler::addSubsystem(pointer);
}

CORETask::CORETask():
		m_disabled(false)
{
	shared_ptr<CORETask> pointer(this);
	COREScheduler::addTask(pointer);
}

void CORETask::disableTasks(bool disable) {
	m_disabled = disable;
}

bool CORETask::isDisabled()
{
	return m_disabled;
}

vector<shared_ptr<CORESubsystem>> COREScheduler::m_subsystems;
vector<shared_ptr<COREAuto>> COREScheduler::m_autons;
vector<shared_ptr<CORETask>> COREScheduler::m_tasks;

void COREScheduler::addSubsystem(shared_ptr<CORESubsystem> subsystem) {
	m_subsystems.push_back(subsystem);
	cout << "Subsystem Added" << endl;
	//TODO: Log -> SUBSYSTEMNAME added
}

void COREScheduler::addAuton(shared_ptr<COREAuto> auton) {

}

void COREScheduler::addTask(shared_ptr<CORETask> task) {
	m_tasks.push_back(task);
}

void COREScheduler::robotInit() {
	for(auto subsystem : m_subsystems) {
		subsystem->robotInit();
	}
    for(auto auton : m_autons) {
        auton->putToDashboard(m_autonChooser);
    }
    SmartDashboard::PutData("Autonomous Selection", m_autonChooser);
	//for(auto task : m_tasks) {
	//	task->robotInitTask();
	//}
	//TODO: Log -> RobotInit Complete
}

void COREScheduler::autonInit() {
    shared_ptr<COREAuto> pointer((COREAuto*) m_autonChooser->GetSelected());
    m_selectedAuto = pointer;
    m_selectedAuto->autonInit();
}

void COREScheduler::auton() {
    m_selectedAuto->auton();
}

void COREScheduler::teleopInit() {
	for(auto subsystem : m_subsystems) {
		subsystem->teleopInit();
	}
	for(auto task : m_tasks) {
		if(!task->isDisabled())
			task->teleopInitTask();
	}
	//TODO: Log -> TeleopInit Complete
}

void COREScheduler::teleop() {
	for(auto task : m_tasks) {
		if(!task->isDisabled())
			task->preTeleopTask();
	}
	for(auto subsystem : m_subsystems) {
		subsystem->teleop();
	}
	for(auto task : m_tasks) {
		if(!task->isDisabled())
			task->postTeleopTask();
	}
}

void COREScheduler::teleopEnd() {
	for(auto task : m_tasks) {
		if(!task->isDisabled())
			task->teleopEndTask();
	}
	for(auto subsystem : m_subsystems) {
		subsystem->teleopEnd();
	}
	//TODO: Log -> TeleopEnd Complete
}

void COREScheduler::test() {
	for(auto subsystem : m_subsystems) {
		subsystem->test();
	}
	//TODO: Do something
}