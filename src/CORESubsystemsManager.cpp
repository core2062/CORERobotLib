#include "CORESubsystemsManager.h"
#include <iostream>
#define quote(x) #x

using namespace CORE;

CORESubsystem::CORESubsystem() {
	name = string(quote("Subsystem Name"));
	std::cout << name << " Yep" << std::endl;
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
