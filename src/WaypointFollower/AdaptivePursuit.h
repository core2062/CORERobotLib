#pragma once

#include "TankKinematics.h"
#include "Path.h"
#include "CORELogging/CORELog.h"

class AdaptivePursuit{
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

	bool checkEvent(std::string event);

	struct Circle{
		Translation2d center;
		double radius;
		bool turnRight;
		Circle(Translation2d cent, double rad, bool turn_right);
	};

	std::pair<bool, Circle> joinPath(Position2d pos, Translation2d lookahead);
};
