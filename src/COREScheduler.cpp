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
shared_ptr<SendableChooser<COREAuto*>> COREScheduler::m_autonChooser;
shared_ptr<COREAuto> COREScheduler::m_selectedAuto;

void COREScheduler::addSubsystem(shared_ptr<CORESubsystem> subsystem) {
	m_subsystems.push_back(subsystem);
	cout << "Subsystem Added" << endl;
	//TODO: Log -> SUBSYSTEMNAME added
}

void COREScheduler::addAuton(shared_ptr<COREAuto> auton) {
    m_autons.push_back(auton);
    cout << "Auton Added" << endl;
    //TODO: Log -> AUTONNAME added
}

void COREScheduler::addTask(shared_ptr<CORETask> task) {
	m_tasks.push_back(task);
}

void COREScheduler::robotInit() {
	for(auto subsystem : m_subsystems) {
		subsystem->robotInit();
	}
	for(auto task : m_tasks) {
		task->robotInitTask();
	}
    for(auto auton : m_autons) {
        auton->putToDashboard(m_autonChooser);
    }
    //TODO: Log -> RobotInit Complete
}

void COREScheduler::autonInit() {
	if(!m_autons.empty()) {
#ifdef __arm__
		auto m_selectedAuto = m_autonChooser->GetSelected();
#else
		m_selectedAuto = m_autons[0];
		//TODO: Simulated auto switcher
#endif
		m_selectedAuto->autonInit();
	}
	else {
		cout << "No Autons Added!" << endl;
		m_selectedAuto = nullptr;
	}
}

bool COREScheduler::auton() {
	if(m_selectedAuto != nullptr) {
		m_selectedAuto->auton();
		return m_selectedAuto->complete();
	}
	else {
		cout << "No Auton Selected!" << endl;
		return true;
	}
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
