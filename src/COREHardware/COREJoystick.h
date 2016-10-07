#ifndef SRC_CORELIBRARY_COREHARDWARE_COREJOYSTICK_H_
#define SRC_CORELIBRARY_COREHARDWARE_COREJOYSTICK_H_
#include <vector>
#include <map>
#include "../CORESubsystemsManager.h"
#include "../../sim/include/WPILib.h"


#ifndef __arm__
class Joystick {
public:
	inline Joystick(int num) {}
	inline double GetRawAxis(int num) { return -1; }
	inline bool GetRawButton(int num) { return false; }
	inline string GetJoystickName() { return "NULL"; }
};
#else
#include "WPILib.h"
#endif

using namespace std;
using namespace CORE;

namespace CORE {
enum JoystickAxis {
	LEFTSTICKX,
	LEFTSTICKY,
	RIGHTSTICKX,
	RIGHTSTICKY,
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
	ABUTTON,
	XBUTTON,
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
enum JoystickType {
	F310
};
class COREJoystick : public CORETask {
public:
	COREJoystick(int joystickNumber);
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
	JoystickType expectedJoystickType;
	std::shared_ptr<COREJoystick> instance;
};

}
#endif
