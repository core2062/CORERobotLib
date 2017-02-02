#pragma once

#include <string>
#include <memory>
#include <iostream>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif

#warning Using Fake WPILib

using namespace std;

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

template <class T>
class SendableChooser {
public:
    virtual ~SendableChooser() = default;
    inline void AddObject(const std::string &name, void *object) {}
    inline void AddDefault(const std::string &name, void *object) {}
    inline void *GetSelected() {}
};

class SmartDashboard {
public:
    static inline void init() {}
    //static inline void PutData(string key, SendableChooser *data) {}
    //static inline void PutData(NamedSendable *value) {}
    static inline void PutBoolean(string keyName, bool value) {}
    static inline bool GetBoolean(string keyName, bool defaultValue) {}
    static inline void PutNumber(string keyName, double value) {}
    static inline double GetNumber(string keyName, double defaultValue) {}
    static inline void PutString(string keyName, string value) {}
    static inline std::string GetString(string keyName, string defaultValue) {}
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

class Encoder {
public:
    inline Encoder(int aChannel, int bChannel, bool reverseDirection = false) {}
    inline int32_t Get() {}
    inline void Reset() {}
};

class DigitalInput {
public:
    inline DigitalInput(uint32_t channel) {}
    inline bool Get() {return false;}
};

class DriverStation {
public:
    enum Alliance {
        kRed,
        kBlue,
        kInvalid
    };
    static inline DriverStation &GetInstance() {
        static DriverStation instance;
        return instance;
    }
    inline bool IsFMSAttached() { return false; }
    inline Alliance GetAlliance() { return Alliance::kInvalid; }
    inline int GetLocation() { return 0; }
};

class Preferences {
public:
    static inline Preferences &GetInstance() {
        static Preferences instance;
        return instance;
    }
    inline void PutDouble(string, string) {}
    inline void PutString(string, string) {}
    inline void PutBoolean(string, string) {}
    inline void PutInt(string, string) {}
    inline double GetDouble(string) { return 0; }
    inline string GetString(string) { return "null"; }
    inline bool GetBoolean(string) { return false; }
    inline int GetInt(string) { return 0; }
};