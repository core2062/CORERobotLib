#pragma once

#include "TankKinematics.h"
#include "Path.h"

using namespace std;

class AdaptivePursuit {
private:
    double m_fixedLookahead;
    Path m_path;
    Position2d::Delta m_lastCommand;
    double m_lastTime;
    double m_maxAccel;
    double m_dt;
    bool m_reversed;
    double m_pathCompletionTolerance;
    bool m_hasRun = false;
    bool m_gradualStop = true;

public:
    AdaptivePursuit(double fixedLookahead, double maxAccel, double nominalDt, Path path,
                    bool reversed, double pathCompletionTolerance, bool gradualStop = true);
    bool isDone();
    Position2d::Delta update(Position2d robotPos, double now);
    bool checkEvent(string event);

    struct Circle {
        COREVector center;
        double radius;
        bool turnRight;
        Circle(COREVector cent, double rad, bool turn_right);
    };

    pair<bool, Circle> joinPath(Position2d pos, COREVector lookahead);
};
