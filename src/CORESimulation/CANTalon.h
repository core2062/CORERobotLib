#pragma once

#include "gmock/gmock.h"

class CANTalon {
public:
enum ControlMode {
    kVoltage,
    kPercentVbus,
    kCurrent,
    kFollower
};

enum FeedbackDevice {
    QuadEncoder = 0,
    AnalogPot = 2,
    AnalogEncoder = 3,
    EncRising = 4,
    EncFalling = 5,
    CtreMagEncoder_Relative = 6,  //!< Cross The Road Electronics Magnetic
    //! Encoder in Absolute/PulseWidth Mode
            CtreMagEncoder_Absolute = 7,  //!< Cross The Road Electronics Magnetic
    //! Encoder in Relative/Quadrature Mode
            PulseWidth = 8,
};

enum FeedbackDeviceStatus {
    FeedbackStatusUnknown = 0,  //!< Sensor status could not be determined.  Not
    //! all sensors can do this.
            FeedbackStatusPresent = 1,  //!< Sensor is present and working okay.
    FeedbackStatusNotPresent =
    2,  //!< Sensor is not present, not plugged in, not powered, etc...
};
enum StatusFrameRate {
    StatusFrameRateGeneral = 0,
    StatusFrameRateFeedback = 1,
    StatusFrameRateQuadEncoder = 2,
    StatusFrameRateAnalogTempVbat = 3,
    StatusFrameRatePulseWidthMeas = 4,
};

enum SetValueMotionProfile {
    SetValueMotionProfileDisable = 0,
    SetValueMotionProfileEnable = 1,
    SetValueMotionProfileHold = 2,
};

enum TalonControlMode {
    kThrottleMode = 0,
    kFollowerMode = 5,
    kVoltageMode = 4,
    kPositionMode = 1,
    kSpeedMode = 2,
    kCurrentMode = 3,
    kMotionProfileMode = 6,
    kMotionMagicMode = 7,
    kDisabled = 15
};

    explicit CANTalon(int deviceNumber) {};

    explicit CANTalon(int deviceNumber, int controlPeriodMs) {};
    ~CANTalon() = default;

    MOCK_METHOD1(PIDWrite, void(double
            output));

    MOCK_METHOD0(PIDGet, double());

    MOCK_METHOD1(SetExpiration, void(double
            timeout));

    MOCK_CONST_METHOD0(GetExpiration, double());

    MOCK_CONST_METHOD0(IsAlive, bool());

    MOCK_METHOD0(StopMotor, void());

    MOCK_METHOD1(SetSafetyEnabled, void(bool
            enabled));

    MOCK_CONST_METHOD0(IsSafetyEnabled, bool());

    MOCK_CONST_METHOD1(GetDescription, void(std::ostringstream
            &desc));

    MOCK_CONST_METHOD0(Get, double());

    MOCK_METHOD1(Set, void(double
            value));

    MOCK_METHOD0(Reset, void());

    MOCK_METHOD1(SetSetpoint, void(double
            value));

    MOCK_METHOD0(Disable, void());

    MOCK_METHOD0(EnableControl, void());

    MOCK_METHOD0(Enable, void());

    MOCK_METHOD1(SetP, void(double
            p));

    MOCK_METHOD1(SetI, void(double
            i));

    MOCK_METHOD1(SetD, void(double
            d));

    MOCK_METHOD3(SetPID, void(double
            p, double
            i, double
            d));

    MOCK_METHOD4(SetPID, void(double
            p, double
            i, double
            d, double
            f));

    MOCK_CONST_METHOD0(GetP, double());

    MOCK_CONST_METHOD0(GetI, double());

    MOCK_CONST_METHOD0(GetD, double());

    MOCK_CONST_METHOD0(GetF, double());

    MOCK_CONST_METHOD0(GetBusVoltage, double());

    MOCK_CONST_METHOD0(GetOutputVoltage, double());

    MOCK_CONST_METHOD0(GetOutputCurrent, double());

    MOCK_CONST_METHOD0(GetTemperature, double());

    MOCK_CONST_METHOD0(GetPosition, double());

    MOCK_CONST_METHOD0(GetSpeed, double());

    MOCK_CONST_METHOD0(GetClosedLoopError, int());
    MOCK_METHOD1(SetAllowableClosedLoopErr, void(uint32_t
            allowableCloseLoopError));

    MOCK_CONST_METHOD0(GetAnalogIn, int());

    MOCK_METHOD1(SetAnalogPosition, void(int
            newPosition));

    MOCK_CONST_METHOD0(GetAnalogInRaw, int());

    MOCK_CONST_METHOD0(GetAnalogInVel, int());

    MOCK_CONST_METHOD0(GetEncPosition, int());

    MOCK_METHOD1(SetEncPosition, void(int));

    MOCK_CONST_METHOD0(GetEncVel, int());

    MOCK_CONST_METHOD0(GetPulseWidthPosition, int());

    MOCK_METHOD1(SetPulseWidthPosition, void(int
            newpos));

    MOCK_CONST_METHOD0(GetPulseWidthVelocity, int());

    MOCK_CONST_METHOD0(GetPulseWidthRiseToFallUs, int());

    MOCK_CONST_METHOD0(GetPulseWidthRiseToRiseUs, int());

    MOCK_CONST_METHOD0(GetForwardLimitOK, bool());

    MOCK_CONST_METHOD0(GetReverseLimitOK, bool());

    MOCK_CONST_METHOD0(GetFaults, uint16_t());

    MOCK_METHOD1(SetVoltageRampRate, void(double
            rampRate));

    MOCK_METHOD1(SetVoltageCompensationRampRate, void(double
            rampRate));

    MOCK_CONST_METHOD0(GetFirmwareVersion, int());
    MOCK_METHOD1(ConfigPotentiometerTurns, void(uint16_t
            turns));

    MOCK_METHOD2(ConfigSoftPositionLimits, void(double
            forwardLimitPosition, double
            reverseLimitPosition));

    MOCK_METHOD0(DisableSoftPositionLimits, void());

    MOCK_METHOD1(ConfigForwardLimit, void(double
            forwardLimitPosition));

    MOCK_METHOD1(ConfigReverseLimit, void(double
            reverseLimitPosition));

    MOCK_METHOD1(ConfigMaxOutputVoltage, void(double
            voltage));

    MOCK_METHOD1(ConfigFaultTime, void(double
            faultTime));
    MOCK_METHOD1(SetControlMode, void(ControlMode
            mode));

    MOCK_METHOD1(SetTalonControlMode, void(TalonControlMode mode));
    MOCK_CONST_METHOD0(GetTalonControlMode, TalonControlMode(void));

    MOCK_CONST_METHOD0(GetControlMode, ControlMode());

    MOCK_CONST_METHOD0(IsEnabled, bool());

    MOCK_CONST_METHOD0(GetSetpoint, double());

    MOCK_METHOD1(SetInverted, void(bool
            isInverted));

    MOCK_CONST_METHOD0(GetInverted, bool());
    MOCK_METHOD2(SetStatusFrameRateMs, void(StatusFrameRate, int));
};