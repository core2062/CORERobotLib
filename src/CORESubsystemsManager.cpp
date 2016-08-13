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
