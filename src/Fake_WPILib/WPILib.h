#pragma once

#include <string>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif

class CANTalon {
public:
    CANTalon(int _) {}
    inline float Set(float _) {}
};

class Victor {
public:
    Victor(int _) {}
    inline void Set(float _) {}
};

class Jaguar {
public:
    Jaguar(int _) {}
    inline void Set(float _) {}
};

class Joystick {
public:
    inline Joystick(int num) {}
    inline double GetRawAxis(int num) { return -1; }
    inline bool GetRawButton(int num) { return false; }
    inline std::string GetJoystickName() { return "NULL"; }
};

class SampleRobot {

};

inline void Wait(double _) {
    usleep(_*1000000);
}

inline bool IsOperatorControl() {
    return true;
}

inline bool IsEnabled() {
    return true;
}

inline bool IsAutonomous() {
    return true;
}

inline bool IsDisabled() {
    return false;
}