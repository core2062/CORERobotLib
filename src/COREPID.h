#pragma once

#include <vector>
#include <memory>

#include "COREHardware/CORETimer.h"
#include "COREScheduler.h"
#include "COREHardware/CORESensor.h"

namespace CORE {
class COREAHRS;

enum PIDType {
	POS,
	VEL,
	POS_VEL
};

class COREPID : public CORETask {
public:
	COREPID(PIDType PIDControllerType, double pProfile1Value, double iProfile1Value, double dProfile1Value, double fProfile1Value = 1, double pProfile2Value = 0, double iProfile2Value = 0, double dProfile2Value = 1, double fProfile2Value = 0, int integralAccuracy = 1);
	double calculate(int profile = -1);
	void setPos(double setPositionValue);
	void setVel(double setVelocityValue);
	double getPos();
	double getVel();
	void setActualPos(double actualPositionValue);
	void setActualVel(double actualVelocityValue);
	void bindInputDevice(COREAHRS * gyro);
	void setDefaultProfile(int profile);
	void setP(double value, int profile = -1);
	void setI(double value, int profile = -1);
	void setD(double value, int profile = -1);
	void setF(double value, int profile = -1);
	double getP(int profile = -1);
	double getI(int profile = -1);
	double getD(int profile = -1);
	double getF(int profile = -1);
	double getOutput(int profile = -1);
	double getProportional(int profile = -1);
	double getIntegral(int profile = -1);
	double getDerivative(int profile = -1);
	void setType(PIDType type);
	PIDType getType();
	void preTeleopTask();
	void postTeleopTask();
protected:
	PIDType m_controllerType;
	enum inputDeviceType {
		NO_INPUT,
		CANTALON_INPUT,
		AHRS_INPUT
	};
	struct PIDProfile {
		double P, I, D, F, proportional, integral, derivative, output, lastOutput;
		std::vector<double> mistake;
	} m_PID1, m_PID2;
	double m_setPosition = 0;
	double m_setVelocity = 0;
	double m_actualPosition = 0;
	double m_actualVelocity = 0;
	int m_defaultProfile = 1;
	CORETimer m_timer;
	std::shared_ptr<COREAHRS> m_inputGyro;
	inputDeviceType m_inputDevice;
	PIDProfile *getProfile(int profile);
};
}
