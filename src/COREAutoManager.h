#include <vector>
#include <string>
#define quote(x)
using namespace std;
namespace CORE {

class CORESubsystem {
public:
	std::string name;
	CORESubsystem();
	virtual void robotInit() = 0;
	virtual void teleopInit() = 0;
	virtual void teleop() = 0;
	virtual void teleopEnd(){
		//TODO: Log -> Teleop end not implemented for: NAME
	}
};

class CORESubsystemsManager {
private:
	static vector<CORESubsystem*> subsystems;
public:
	static void addSubsystem(CORESubsystem* subsystem) {
		subsystems.push_back(subsystem);
		//TODO: Log -> SUBSYSTEMNAME added
	}
	static void robotInit() {
		for(auto subsystem : subsystems) {
			subsystem->teleopInit();
		}
		//TODO: Log -> RobotInit Complete
	}
	static void teleopInit() {
		for(CORESubsystem* subsystem : subsystems) {
			subsystem->teleopInit();
		}
		//TODO: Log -> TeleopInit Complete
	}
	static void teleop() {
		for(CORESubsystem* subsystem : subsystems) {
			subsystem->teleop();
		}
	}
	void teleopEnd() {
		for(CORESubsystem* subsystem : subsystems) {
			subsystem->teleopEnd();
		}
		//TODO: Log -> TeleopEnd Complete
	}
};
}
