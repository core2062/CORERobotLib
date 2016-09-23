#ifndef SRC_CORELIBRARY_COREHARDWARE_COREJOYSTICK_H_
#define SRC_CORELIBRARY_COREHARDWARE_COREJOYSTICK_H_
#include <vector>
#include <map>
#include "../CORESubsystemsManager.h"


#ifndef __arm__
class Joystick {
public:
	inline Joystick(int num) {}
	inline double GetRawAxis(int num) { return -1; }
	inline bool GetRawButton(int num) { return false; }
};
#else
#include "WPILib.h"
#endif

using namespace std;
using namespace CORE;

namespace CORE {
enum JoystickAxis {
	LEFTSTICKX = 0,
	LEFTSTICKY = 1,
	RIGHTSTICKX = 2,
	RIGHTSTICKY = 3,
	RIGHTTRIGGERAXIS,
	LEFTTRIGGERAXIS
};
enum JoystickButton {
	DPADN,
	DPADNE,
	DPADE,
	DPADSE,
	DPADS,
	DPADSW,
	DPADW,
	DPADNW,
	LEFTSTICK,
	RIGHTSTICK,
	BACKBUTTON,
	LOGOBUTTON,
	STARTBUTTON,
	YBUTTON,
	BBUTTON,
	ABUTTON = 2,
	XBUTTON = 1,
	LEFTBUTTON,
	RIGHTBUTTON,
	LEFTTRIGGER,
	RIGHTTRIGGER
};
enum ButtonState {
	PRESSED,
	ACTIVE,
	RELEASED,
	NORMAL
};
class COREJoystick : public CORETask {
public:
	COREJoystick(unsigned int joystickNumber);
	void registerAxis(JoystickAxis axis);
	void registerButton(JoystickButton button);
	double getAxis(JoystickAxis axis);
	bool getButton(JoystickButton button);
	ButtonState getButtonState(JoystickButton button);
	int getPort();
	void preTeleopTask();
private:
	Joystick joystick;
	int joystickPort;
	map<JoystickButton, ButtonState> lastButtonCache, buttonCache;
	map<JoystickAxis, double> axisCache;
};

}
#endif
