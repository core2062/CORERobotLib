#pragma once

#include "TankKinematics.h"
#include "TankPath.h"

class TankAdaptivePursuit{
private:
	double m_fixedLookahead;
	TankPath * m_path;
	TankPosition2d::TankDelta m_lastCommand;
	double m_lastTime;
	double m_maxAccel;
	double m_dt;
	bool m_reversed;
	double m_pathCompletionTolerance;
	bool m_hasRun = false;
	bool m_gradualStop = true;

public:
	TankAdaptivePursuit(double fixedLookahead, double maxAccel, double nominalDt, TankPath * path,
			bool reversed, double pathCompletionTolerance, bool gradualStop = true);

	bool IsDone(TankPath * m_path);

	TankPosition2d::TankDelta Update(TankPosition2d robotPos, double now);

	bool CheckEvent(std::string event);

	struct Circle{
		TankTranslation2d center;
		double radius;
		bool turnRight;
		Circle(TankTranslation2d cent, double rad, bool turn_right);
	};

	std::pair<bool, Circle> JoinPath(TankPosition2d pos, TankTranslation2d lookahead);
};
