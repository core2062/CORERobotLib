#ifndef SRC_CORELIBRARY_COREHARDWARE_COREJOYSTICK_H_
#define SRC_CORELIBRARY_COREHARDWARE_COREJOYSTICK_H_
#include <vector>
#include "../CORESubsystemsManager.h"


#ifndef SIMULATION
#include "WPILib.h"
#endif

namespace CORE {
	enum JoystickAxis {
		LEFTSTICKX,
		LEFTSTICKY,
		RIGHTSTICKX,
		RIGHTSTICKY
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
		RELESASED,
		NORMAL
	};
class COREJoystick : public CORETask {
public:
	COREJoystick(int joystickNumber)
	{

	}
	void registerAxis(JoystickAxis axis);
	void registerButton(JoystickButton button);
	double getAxis(JoystickAxis axis);
	bool getButton(JoystickButton button);
	ButtonState getButtonState(JoystickButton button);
	void preTeleopTask();
};

}
#endif
