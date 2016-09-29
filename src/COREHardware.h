#ifndef SRC_CORELIBRARY_COREHARDWARE_H_
#define SRC_CORELIBRARY_COREHARDWARE_H_

#include <map>
#include <memory>

#include "COREHardware/CORETimer.h"
#include "COREHardware/COREMotor.h"
#include "COREHardware/COREJoystick.h"

using namespace CORE;
using namespace std;

namespace CORE {
class Robot {
private:
	static map<int, shared_ptr<COREMotor>> motorMap;
	static map<int, shared_ptr<COREJoystick>> joystickMap;
public:
	static void addMotor(shared_ptr<COREMotor> motor);
	static void addJoystick(shared_ptr<COREJoystick> joystick);
	static shared_ptr<COREMotor> motor(int port);
	static shared_ptr<COREJoystick> joystick(int port);
	static void disableAllMotors();
};
}
#endif
