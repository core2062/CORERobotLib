#pragma once

#include "../COREPID.h"
#include <vector>
#include <memory>

#include "../CORESubsystemsManager.h"

#include "WPILib.h"

namespace CORE {
	enum controlMode {
		VOLTAGE,
		PERCENTAGE,
		CURRENT,
        POS_PID,
        VEL_PID,
		POSVELPID
	};

	enum controllerType {
		CANTALON,
		JAGUAR,
		VICTOR
	};

class COREMotor : public COREPID {
public:
	std::shared_ptr<CANTalon> CANTalonController;
	std::shared_ptr<Jaguar> JaguarController;
	std::shared_ptr<Victor> VictorController;
	//std::shared_ptr<COREEncoder>;
	COREMotor(int port, controllerType controller = CANTALON, controlMode controlMethod = VOLTAGE, double pProfile1Value = 0, double iProfile1Value = 0, double dProfile1Value = 0, double pProfile2Value = 0, double iProfile2Value = 0, double dProfile2Value = 0, int integralAccuracy = 1);
	void Set(double motorSetValue);
	double Get();
	void setReversed(bool reverse = true);
	bool getReversed();
	void setControlMode(controlMode controlMethod);
	controlMode getControlMode();
	int getPort();
	controllerType getControllerType();
	void setDeadband(double range);
	void setDeadband(double min, double max);
	void addSlave(std::shared_ptr<COREMotor> slaveMotor);
	void motorSafety(bool disableMotorSafety = true);
	void postTeleopTask();
private:
    double m_motorValue = 0;
    double m_lastMotorValue = 0;
    bool m_reversed = false;
    double m_trapSum = 0;
    double m_deadBandMin = 0;
    double m_deadBandMax = 0;
    bool m_motorUpdated = false;
    controlMode m_motorControlMode;
    controllerType m_motorControllerType;
    CORETimer *m_trapSumTimer;
    double m_lastSum = 0;
    int m_motorPort;
    int m_motorSafetyCounter = 0;
    bool m_motorSafetyDisabled = false;
    std::vector<COREMotor *> m_slaveMotors;
    shared_ptr<COREMotor> m_instance;
};
}