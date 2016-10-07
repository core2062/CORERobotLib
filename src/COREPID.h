#ifndef SRC_CORELIBRARY_COREPID_H_
#define SRC_CORELIBRARY_COREPID_H_
#include <vector>
#include <memory>

#include "COREHardware/CORETimer.h"
#include "CORESubsystemsManager.h"

namespace CORE {
class COREAHRS;

enum PIDType {
	POS,
	VEL,
	POSVEL
};

class COREPID : public CORETask {
public:
	COREPID(PIDType PIDControllerType, double pProfile1Value, double iProfile1Value, double dProfile1Value, double fProfile1Value = 1, double pProfile2Value = 0, double iProfile2Value = 0, double dProfile2Value = 0, double fProfile2Value = 0, int integralAccuracy = 1);
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
	PIDType ControllerType;
	enum inputDeviceType {
		NoInput,
		CANTalonInput,
		AHRSInput
	};
	struct PIDProfile {
		double P, I, D, F, proportional, integral, derivative, output, lastOutput;
		std::vector<double> mistake;
	}PID1, PID2;
	double setPosition = 0;
	double setVelocity = 0;
	double actualPosition = 0;
	double actualVelocity = 0;
	int defaultProfile = 1;
	CORETimer timer;
	std::shared_ptr<COREAHRS> inputGyro;
	inputDeviceType inputDevice;
	PIDProfile *getProfile(int profile);
};
}
#endif
