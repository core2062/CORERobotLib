#ifndef SRC_CORELIBRARY_COREAUTO_H_
#define SRC_CORELIBRARY_COREAUTO_H_
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <memory>
#include "COREHardware/CORETimer.h"
#include "CORERobot.h"

#ifndef __arm__
bool IsAutonomous() {return true;}
bool IsDisabled() {return false;}
#else
#include "WPILib.h"
#endif

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
private:
	queue<std::shared_ptr<COREAutoAction>> sequentialActions;
	queue<std::shared_ptr<COREAutoAction>> parallelActions;
	CORERobot * robot;
public:
	COREAuto(CORERobot * robot);
	void addSequential(std::shared_ptr<COREAutoAction> autoAction);
	void addParallel(std::shared_ptr<COREAutoAction> autoAction);
	void runAuto();

};
}
#endif
