#ifndef SRC_CORELIBRARY_CORELOG_H_
#define SRC_CORELIBRARY_CORELOG_H_
#include <vector>
#include <queue>
#include <string>
#include <memory>
#include "COREHardware/CORETimer.h"
#include "CORERobot.h"

#ifndef __arm__
#else
#include "WPILib.h"
#endif

using namespace std;
namespace CORE {
class CORELog : public CORETask {
public:
	CORELog();
	void logData(string message);
	void logInfo(string message);
	void logWarning(string message);
	void logError(string message);
	void updateLog();
	void postTeleopTask();

};
}
#endif
