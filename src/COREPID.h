#ifndef SRC_CORELIBRARY_COREPID_H_
#define SRC_CORELIBRARY_COREPID_H_
#include <vector>
#include <memory>

#include "COREHardware/CORETimer.h"

namespace CORE {
using namespace CORE::COREHardware;
class COREAHRS;
class COREPID {
private:
	enum inputDeviceType {
		NoInput,
		CANTalonInput,
		AHRSInput
	};
	enum outputDeviceType {
		NoOutput,
		CANTalonOutput
	};
	struct PIDProfile {
		double P,I,D,porportional,integral,derivative,output;
		std::vector<double> mistake;
	}PID1, PID2;
	double setPointValue = 0, actualPosition = 0;
	CORETimer timer;
	std::shared_ptr<COREAHRS> inputGyro;
	inputDeviceType inputDevice;
	outputDeviceType outputDevice;
	PIDProfile *getProfile(int profile);
public:
	enum PIDType {
		Pos,
		Vel
	};
	COREPID(PIDType PIDControllerType, double pProfile1Value, double iProfile1Value, double dProfile1Value, double pProfile2Value = 0, double iProfile2Value = 0, double dProfile2Value = 0, int integralAccuracy = 1);
	double calculate(int profile = 1);
	double calculate(double newSetpoint, int profile = 1);
	void setPoint(double setPoint);
	void setActualPosition(double actualPosition);
	void bindInputDevice(COREAHRS * gyro);
	double getSetPoint();
	double getP(int profile = 1);
	double getI(int profile = 1);
	double getD(int profile = 1);
	void setProfile(int profile);
	void setP(double value, int profile = 1);
	void setI(double value, int profile = 1);
	void setD(double value, int profile = 1);
	double getOutput(int profile = 1);
	double getPorportional(int profile = 1);
	double getIntegral(int profile = 1);
	double getDerivative(int profile = 1);
private:
	PIDType ControllerType;
};
}
#endif
