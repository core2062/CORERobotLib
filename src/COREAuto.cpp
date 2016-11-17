#include "COREAuto.h"

using namespace CORE;

COREAuto::COREAuto(CORERobot * robot):
        m_robot(robot)
{

}

void COREAuto::addParallel(std::shared_ptr<COREAutoAction> autoAction) {

}

void COREAuto::addSequential(std::shared_ptr<COREAutoAction> autoAction) {
    m_sequentialActions.push(autoAction);
}

void COREAuto::runAuto() {
	actionStatus lastSeqActionStatus = END;
    while (!m_sequentialActions.empty() && !m_parallelActions.empty() /*&& IsAutonomous() && !IsDisabled()*/) {
		if(lastSeqActionStatus == END) {
            m_sequentialActions.front()->actionInit();
		}
        lastSeqActionStatus = m_sequentialActions.front()->action();

        m_robot->waitLoopTime();
	}
}
