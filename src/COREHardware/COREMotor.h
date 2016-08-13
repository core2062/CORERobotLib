#ifndef SRC_CORELIBRARY_COREHARDWARE_COREMOTOR_H_
#define SRC_CORELIBRARY_COREHARDWARE_COREMOTOR_H_
#include "../COREPID.h"
#include <vector>
#include <memory>

#include "../CORESubsystemsManager.h"

class CANTalon {};
class Jaguar {};
class Victor {};

#ifndef SIMULATION1
//#include "WPILib.h"
#endif

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
	void setControlMode(controlMode controlMethod);
	controlMode getControlMode();
	void setDeadband(double range);
	void setDeadband(double min, double max);
	void addSlave(COREMotor *slaveMotor);
	void postTeleopTask();
private:
	double motorValue = 0;
	double lastMotorValue = 0;
	double trapSum = 0;
	double deadBandMin = 0;
	double deadBandMax = 0;
	bool motorUpdated = false;
	controlMode motorControlMode;
	controllerType motorControllerType;
	CORETimer * trapSumTimer;
	double lastSum;
	std::vector<COREMotor*> slaveMotors;
};

}
#endif
