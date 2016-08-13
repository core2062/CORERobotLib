#ifndef SRC_CORELIBRARY_CORESUBSYSTEMSMANAGER_H_
#define SRC_CORELIBRARY_CORESUBSYSTEMSMANAGER_H_

#include <vector>
#include <string>
#include <memory>
#include <iostream>

using namespace std;
namespace CORE {

class CORESubsystem {
public:
	std::string name;
	CORESubsystem();
	virtual void robotInit() = 0;
	virtual void teleopInit() = 0;
	virtual void teleop() = 0;
	virtual void teleopEnd() {
		//TODO: Log -> Teleop end not implemented for: NAME
	}
	virtual ~CORESubsystem(){}
};

class CORETask {
private:
	bool disabled;
public:
	CORETask();
	virtual void robotInitTask() {}
	virtual void teleopInitTask() {}
	virtual void preTeleopTask() {}
	virtual void postTeleopTask() {}
	virtual void teleopEndTask() {}
	virtual void disableTasks(bool disable = true);
	virtual bool isDisabled();
	virtual ~CORETask(){}
};

class CORESubsystemsManager {
private:
	static vector<std::shared_ptr<CORESubsystem>> subsystems;
	static vector<std::shared_ptr<CORETask>> tasks;
public:
	static void addSubsystem(std::shared_ptr<CORESubsystem> subsystem) {
		subsystems.push_back(subsystem);
        cout << "Subsystem Added" << endl;
		//TODO: Log -> SUBSYSTEMNAME added
	}
    static void addTask(std::shared_ptr<CORETask> task) {
		tasks.push_back(task);
	}
	static void robotInit() {
		for(auto subsystem : subsystems) {
			subsystem->robotInit();
		}
		//for(auto task : tasks) {
		//	task->robotInitTask();
		//}
		//TODO: Log -> RobotInit Complete
	}
	static void teleopInit() {
		for(auto subsystem : subsystems) {
			subsystem->teleopInit();
		}
		for(auto task : tasks) {
			if(!task->isDisabled())
				task->teleopInitTask();
		}
		//TODO: Log -> TeleopInit Complete
	}
	static void teleop() {
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
	static void teleopEnd() {
		for(auto task : tasks) {
			if(!task->isDisabled())
				task->teleopEndTask();
		}
		for(auto subsystem : subsystems) {
			subsystem->teleopEnd();
		}
		//TODO: Log -> TeleopEnd Complete
	}
};
}
#endif
