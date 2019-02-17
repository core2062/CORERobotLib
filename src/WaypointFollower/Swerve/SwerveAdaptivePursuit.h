#pragma once

#include "SwerveKinematics.h"
#include "SwervePath.h"
#include "CORELogging/CORELog.h"
#include "COREUtilities/COREVector.h"

class SwerveAdaptivePursuit{
private:
	double m_fixedLookahead;
	SwervePath m_path;
	SwervePosition2d::SwerveDelta m_lastCommand;
	double m_lastTime;
	double m_maxAccel;
	double m_rotationkP;
	double m_dt;
	bool m_reversed;
	double m_pathCompletionTolerance;
	bool m_hasRun = false;
	bool m_gradualStop = true;

public:
	SwerveAdaptivePursuit(double fixedLookahead, double maxAccel, double rotationkP, double nominalDt, SwervePath path,
			bool reversed, double pathCompletionTolerance, bool gradualStop = true);

	bool IsDone();

	SwervePosition2d::SwerveDelta Update(SwervePosition2d robotPos, double now);

	bool CheckEvent(std::string event);

	struct Circle{
		SwerveTranslation2d center;
		double radius;
		bool turnRight;
		Circle(SwerveTranslation2d cent, double rad, bool turn_right);
	};

	std::pair<bool, Circle> JoinPath(SwervePosition2d pos, SwerveTranslation2d lookahead);
};
