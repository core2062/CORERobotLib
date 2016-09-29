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
	static void addSubsystem(std::shared_ptr<CORESubsystem> subsystem);
    static void addTask(std::shared_ptr<CORETask> task);
	static void robotInit();
	static void teleopInit();
	static void teleop();
	static void teleopEnd();
	static void test();
};
}
#endif
