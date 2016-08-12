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
		VELPID,
		POSPID
	};

	enum controllerType {
		CANTALON,
		JAGUAR,
		VICTOR
	};

class COREMotor : public CORETask {
public:
	std::shared_ptr<CANTalon> CANTalonController;
	std::shared_ptr<Jaguar> JaguarController;
	std::shared_ptr<Victor> VictorController;
	std::shared_ptr<COREPID> PIDController;
	COREMotor(int port, controllerType controller = CANTALON, controlMode controlMethod = VOLTAGE, double pProfile1Value = 0, double iProfile1Value = 0, double dProfile1Value = 0, double pProfile2Value = 0, double iProfile2Value = 0, double dProfile2Value = 0, int integralAccuracy = 1);
	void Set(double speed);
	double Get();
	double getEncoderValue();
	void setControlMode(controlMode controlMethod);
	controlMode getControlMode();
	void addSlave(COREMotor *slaveMotor);
	void postTeleopTask();
private:
	double motorSpeed = 0;
	double lastTrapizodalSum = 0;
	controlMode motorControlMode;
	std::vector<COREMotor*> slaveMotors;
};

}
#endif
