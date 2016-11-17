#pragma once

#include <vector>
#include <map>
#include <queue>
#include <string>
#include <memory>
#include "COREHardware/CORETimer.h"
#include "CORERobot.h"

#include "WPILib.h"

using namespace std;
namespace CORE {

enum actionStatus {
	CONTINUE,
	BACKGROUND,
	END
};

class COREAutoAction {
public:
	COREAutoAction();
	virtual void actionInit() {}
	virtual void actionEnd() {}
	virtual actionStatus action() = 0;
	virtual ~COREAutoAction(){}
};

class COREAuto {
public:
	COREAuto(CORERobot * robot);
	void addSequential(std::shared_ptr<COREAutoAction> autoAction);
	void addParallel(std::shared_ptr<COREAutoAction> autoAction);
	void runAuto();

private:
    queue<std::shared_ptr<COREAutoAction>> m_sequentialActions;
    queue<std::shared_ptr<COREAutoAction>> m_parallelActions;
    CORERobot *m_robot;
};
}