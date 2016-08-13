#ifndef SRC_CORELIBRARY_COREROBOT_H_
#define SRC_CORELIBRARY_COREROBOT_H_

#include "CORESubsystemsManager.h"
#include "COREAuto.h"
#include "COREHardware\CORETimer.h"

using namespace CORE;

class CORERobot : public CORESubsystem {
private:
	bool loopStarted;
	CORETimer loopTimer;
	const double TARGETLOOPTIME = 0.1;
public:
	CORERobot();
	void waitLoopTime();
};

#endif