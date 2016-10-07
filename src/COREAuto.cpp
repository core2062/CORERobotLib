#include "COREAuto.h"

using namespace CORE;

COREAuto::COREAuto(CORERobot * robot):
	robot(robot)
{

}

void COREAuto::addParallel(std::shared_ptr<COREAutoAction> autoAction) {

}

void COREAuto::addSequential(std::shared_ptr<COREAutoAction> autoAction) {
	sequentialActions.push(autoAction);
}

void COREAuto::runAuto() {
//	actionStatus lastSeqActionStatus = END;
//	while(!sequentialActions.empty() && !parallelActions.empty() && IsAutonomous() && !IsDisabled()) {
//		if(lastSeqActionStatus == END) {
//			sequentialActions.front()->actionInit();
//		}
//		lastSeqActionStatus = sequentialActions.front()->action();
//
//		robot->waitLoopTime();
//	}
}
