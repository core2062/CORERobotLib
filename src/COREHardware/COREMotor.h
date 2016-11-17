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
		POSPID,
		VELPID,
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
	double motorValue = 0;
	double lastMotorValue = 0;
	bool reversed = false;
	double trapSum = 0;
	double deadBandMin = 0;
	double deadBandMax = 0;
	bool motorUpdated = false;
	controlMode motorControlMode;
	controllerType motorControllerType;
	CORETimer * trapSumTimer;
	double lastSum = 0;
	int motorPort;
	int motorSafetyCounter = 0;
	bool motorSafetyDisabled = false;
	std::vector<COREMotor*> slaveMotors;
	shared_ptr<COREMotor> instance;
};
}