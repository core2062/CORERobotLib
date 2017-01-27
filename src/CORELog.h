#pragma once

#include <vector>
#include <queue>
#include <string>
#include <memory>
#include <ctime>
#include <fstream>
#include "COREHardware/CORETimer.h"
#include "CORERobot.h"

#include "WPILib.h"

using namespace std;
namespace CORE {
    enum robotMode {
        DISABLED,
        TELEOP,
        AUTON
    };
class CORELog : public CORETask {
public:
	CORELog(string fileName);
//	void logData(string message);
	void logInfo(string message);
	void logWarning(string message);
	void logError(string message);
	void updateLog();
    string getName();
    void robotInitTask() override;
    void autonInitTask() override;
    void teleopInitTask() override;
    void disabledTask() override;
private:
    void writeLastDuration();
    string getRobotMode();
    ofstream m_file;
    string m_fileName;
    robotMode m_robotMode;
	CORETimer m_matchTimer;
};
}
 /*
  * #include <iostream>
#include <fstream>
int main( int argc, char* argv[] )
{
      ofstream myfile;
      myfile.open ("example.csv");
      myfile << "This is the first cell in the first column.\n";
      myfile << "a,b,c,\n";
      myfile << "c,s,v,\n";
      myfile << "1,2,3.456\n";
      myfile << "semi;colon";
      myfile.close();
      return 0;


}

  time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    cout << (now->tm_year + 1900) << '-'
         << (now->tm_mon + 1) << '-'
         <<  now->tm_mday
         << endl;
  */