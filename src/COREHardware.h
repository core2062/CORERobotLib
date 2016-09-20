#ifndef SRC_CORELIBRARY_COREHARDWARE_H_
#define SRC_CORELIBRARY_COREHARDWARE_H_

#include <map>

#include "COREHardware/CORETimer.h"
#include "COREHardware/COREMotor.h"

using namespace CORE;
using namespace std;

namespace CORE {
class COREHardware {
private:
	static std::map<int, std::shared_ptr<COREMotor>> motorMap;
public:
	static void addMotor(std::shared_ptr<COREMotor> motor) {
		if(motorMap.find(motor->getPort()) == motorMap.end()) {
			//TODO: Error: motorType in port # already registered!
		}
		else {
			motorMap[motor->getPort()] = motor;
		}
	}

	static void disableAllMotors() {
		for(auto motor : motorMap) {
			motor.second->Set(0);
		}
	}

	std::shared_ptr<COREMotor> &operator[](int motorPort) {
		if(motorMap.find(motorPort) == motorMap.end()) {
			//TODO: Error: motorType in port # not found!
		}
		return motorMap[motorPort];
	}
};
}
#endif
