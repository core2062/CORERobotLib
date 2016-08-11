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
CORETask::CORETask() {
	std::shared_ptr<CORETask> pointer(this);
	CORESubsystemsManager::addTask(pointer);
}

vector<std::shared_ptr<CORESubsystem>> CORESubsystemsManager::subsystems;
vector<std::shared_ptr<CORETask>> CORESubsystemsManager::tasks;
