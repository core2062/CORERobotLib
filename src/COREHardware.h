#ifndef SRC_CORELIBRARY_COREHARDWARE_H_
#define SRC_CORELIBRARY_COREHARDWARE_H_

#include <map>

#include "COREHardware/CORETimer.h"
#include "COREHardware/COREMotor.h"
#include "COREHardware/COREJoystick.h"

using namespace CORE;
using namespace std;

namespace CORE {
class COREHardware {
private:
	static std::map<int, std::shared_ptr<COREMotor>> motorMap;
	static std::map<int, std::shared_ptr<COREJoystick>> joystickMap;
public:
	static void addMotor(std::shared_ptr<COREMotor> motor) {
		if(motorMap.count(motor->getPort()) == 1) {
			//TODO: Error: motor in port # already registered!
			cout << "Motor in port: " << motor->getPort() << " already added!" << endl;
		}
		else {
			motorMap[motor->getPort()] = motor;
			cout << "Motor port: " << motor->getPort() << " added" << endl;
		}
	}

	static void addJoystick(std::shared_ptr<COREJoystick> joystick) {
		if(joystickMap.count(joystick->getPort()) == 1) {
			//TODO: Error: joystick in port # already registered!
		}
		else {
			joystickMap[joystick->getPort()] = joystick;
		}
	}

	static std::shared_ptr<COREMotor> motor(int port) {
		if(motorMap.count(port) == 1) {
			//TODO: Error: motor in port # not found!
			return motorMap[port];
		}
		else {
			return motorMap[port];
		}
	}

	static std::shared_ptr<COREJoystick> joystick(int port) {
		if(joystickMap.count(port) == 1) {
			//TODO: Error: joystick in port # not found!
			return joystickMap[port];
		}
		else {
			return joystickMap[port];
		}
	}

	static void disableAllMotors() {
		for(auto motor : motorMap) {
			motor.second->Set(0);
		}
	}
};
}
#endif
