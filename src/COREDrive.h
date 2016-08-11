#ifndef SRC_CORELIBRARY_COREDRIVE_H_
#define SRC_CORELIBRARY_COREDRIVE_H_
#include <vector>

#include "COREHardware.h"

namespace CORE {
namespace COREDrive {
using namespace CORE::COREHardware;
class TankDrive {
public:
	//TankDrive(COREMotor * frontLeft, COREMotor * backLeft, COREMotor * frontRight, COREMotor * backRight);
	void SetLeftSpeed();
	void SetRightSpeed();
	void SetSpeed();

};

class MecanumDrive {
private:

public:
	MecanumDrive();
};

class SwerveDrive {
private:

public:
	SwerveDrive();
};

class AetherDrive {
private:

public:
	AetherDrive();
	
};
}
}
#endif
