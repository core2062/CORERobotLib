#include "CORESubsystemsManager.h"
#include <iostream>

using namespace CORE;

CORESubsystem::CORESubsystem() {
    std::shared_ptr<CORESubsystem> pointer(this);
	CORESubsystemsManager::addSubsystem(pointer);
}
CORETask::CORETask():
	disabled(false)
{
	std::shared_ptr<CORETask> pointer(this);
	CORESubsystemsManager::addTask(pointer);
}

void CORETask::disableTasks(bool disable) {
	disabled = disable;
}

bool CORETask::isDisabled()
{
	return disabled;
}

vector<std::shared_ptr<CORESubsystem>> CORESubsystemsManager::subsystems;
vector<std::shared_ptr<CORETask>> CORESubsystemsManager::tasks;

void CORESubsystemsManager::addSubsystem(std::shared_ptr<CORESubsystem> subsystem) {
	subsystems.push_back(subsystem);
	cout << "Subsystem Added" << endl;
	//TODO: Log -> SUBSYSTEMNAME added
}
void CORESubsystemsManager::addTask(std::shared_ptr<CORETask> task) {
	tasks.push_back(task);
}
void CORESubsystemsManager::robotInit() {
	for(auto subsystem : subsystems) {
		subsystem->robotInit();
	}
	//for(auto task : tasks) {
	//	task->robotInitTask();
	//}
	//TODO: Log -> RobotInit Complete
}
void CORESubsystemsManager::teleopInit() {
	for(auto subsystem : subsystems) {
		subsystem->teleopInit();
	}
	for(auto task : tasks) {
		if(!task->isDisabled())
			task->teleopInitTask();
	}
	//TODO: Log -> TeleopInit Complete
}
void CORESubsystemsManager::teleop() {
	for(auto task : tasks) {
		if(!task->isDisabled())
			task->preTeleopTask();
	}
	for(auto subsystem : subsystems) {
		subsystem->teleop();
	}
	for(auto task : tasks) {
		if(!task->isDisabled())
			task->postTeleopTask();
	}
}
void CORESubsystemsManager::teleopEnd() {
	for(auto task : tasks) {
		if(!task->isDisabled())
			task->teleopEndTask();
	}
	for(auto subsystem : subsystems) {
		subsystem->teleopEnd();
	}
	//TODO: Log -> TeleopEnd Complete
}
void CORESubsystemsManager::test() {
	for(auto subsystem : subsystems) {
		subsystem->test();
	}
	//TODO: Do something
}
