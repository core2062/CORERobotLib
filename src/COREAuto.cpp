#include "COREAuto.h"

using namespace CORE;

COREAuto::COREAuto(CORERobot * robot):
	robot(robot)
{

}

void COREAuto::addParallel(std::shared_ptr<COREAutoAction> autoAction) {
	sequentialActions.push(autoAction);
}

void COREAuto::addSequential(std::shared_ptr<COREAutoAction> autoAction) {
	parallelActions.push(autoAction);
}

void COREAuto::runAuto() {
	while(!sequentialActions.empty() && !parallelActions.empty()) {// && IsAutonomous() && !IsDisabled()

		robot->waitLoopTime();
	}
}
