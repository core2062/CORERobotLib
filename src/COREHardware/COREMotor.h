#ifndef SRC_CORELIBRARY_COREHARDWARE_COREMOTOR_H_
#define SRC_CORELIBRARY_COREHARDWARE_COREMOTOR_H_
#include "../COREPID.h"
#include <vector>
#include <memory>

#include "../CORESubsystemsManager.h"

#ifndef SIMULATION1
//#include "WPILib.h"
#endif

namespace CORE {
class CANTalon{};
class Jaguar{};
class Victor{};
using namespace CORE;

	enum controlMode {
		Voltage,
		Percentage,
		Current,
		VelPID,
		PosPID
	};

template <class T>
class COREMotor : private CORETask {
public:
	std::shared_ptr<T> motor;
	std::shared_ptr<COREPID> PIDController;
	COREMotor(int port, controlMode controlMethod = controlMode::Voltage, double pProfile1Value = 0, double iProfile1Value = 0, double dProfile1Value = 0, double pProfile2Value = 0, double iProfile2Value = 0, double dProfile2Value = 0, int integralAccuracy = 1):
        motorControlMode(controlMethod)
    {
        if(!(pProfile1Value == 0 && iProfile1Value == 0 && dProfile1Value == 0 && pProfile2Value == 0 && iProfile2Value == 0 && dProfile2Value == 0) || controlMethod == VelPID || controlMethod == PosPID) {
            COREPID::PIDType PIDControllerType = motorControlMode == VelPID ? COREPID::PIDType::Vel : COREPID::PIDType::Pos;
            std::shared_ptr<COREPID> pointer(new COREPID(PIDControllerType, pProfile1Value, iProfile1Value, dProfile1Value, pProfile2Value, iProfile2Value, dProfile2Value, integralAccuracy));
            PIDController = pointer;
        }
    #ifdef NSIMULATION
        motor = new T(port);
    #else

    #endif
    }

	void Set(double speed) {
        if(motorControlMode <= Current) {
            motorSpeed = speed;
        }
        else {
        }
    }

	double Get() {
        return motorSpeed;
    }

	double getEncoderValue();
	void setControlMode(controlMode controlMethod);
	controlMode getControlMode();
	void addSlave(COREMotor *slaveMotor);
	//void postTeleopTask();
private:
	double motorSpeed = 0;
	double lastTrapizodalSum = 0;
	controlMode motorControlMode;
	std::vector<COREMotor*> slaveMotors;
};

}
}
#endif
