#include "TankAdaptivePursuit.h"

TankAdaptivePursuit::TankAdaptivePursuit(double fixedLookahead, double maxAccel,
		double nominalDt, TankPath * path, bool reversed,
		double pathCompletionTolerance, bool gradualStop):
		m_lastCommand(0,0,0){
	m_fixedLookahead = fixedLookahead;
	m_maxAccel = maxAccel;
	m_path = path;
	m_dt = nominalDt;
	m_reversed = reversed;
	m_pathCompletionTolerance = pathCompletionTolerance;
	m_lastTime = 0.0;
	m_gradualStop = gradualStop;
	cout<<m_path<<"TankAdaptivePursuit"<<endl;

}

bool TankAdaptivePursuit::IsDone(TankPath * m_path) {
	cout<<"TankAdaptivePursuit::IsDone"<<endl;
	double remainingLength = m_path->GetRemainingLength();
	return (remainingLength <= m_pathCompletionTolerance);
}

TankPosition2d::TankDelta TankAdaptivePursuit::Update(TankPosition2d robotPos, double now) {
	cout<<"TankPosition2d::TankDelta"<<endl;
	TankPosition2d pos = robotPos;
	if (m_reversed){
		pos = TankPosition2d(robotPos.GetTranslation(),
				robotPos.GetRotation().RotateBy(TankRotation2d::FromRadians(PI)));
	
	}

	double distanceFromPath = m_path->Update(robotPos.GetTranslation());
	if(IsDone(m_path)){
		return TankPosition2d::TankDelta(0,0,0);
	}

//	std::cout << "Dist From Path" << distanceFromPath << std::endl;

	TankPathSegment::TankSample lookaheadPoint = m_path->GetLookaheadPoint(robotPos.GetTranslation(),
			distanceFromPath + m_fixedLookahead);
	std::pair<bool, Circle> circle = JoinPath(pos, lookaheadPoint.translation);
//	std::cout << "Look X: " << lookaheadPoint.translation.getX() << "  Look Y: " << lookaheadPoint.translation.getY() << std::endl;


	double speed = lookaheadPoint.speed;
	if(m_reversed){
//		std::cout << "Reversed: " << speed << std::endl;
		speed *= -1;
	}

	double dt = now - m_lastTime;
	if (!m_hasRun){
		m_lastCommand = TankPosition2d::TankDelta(0,0,0);
		dt = m_dt;
		m_hasRun = true;
	}
	double accel = (speed - m_lastCommand.dx) / dt;
	if(accel < -m_maxAccel){
		speed = m_lastCommand.dx - m_maxAccel * dt;
	} else if (accel > m_maxAccel){
		speed = m_lastCommand.dx + m_maxAccel * dt;
	}

	double remainingDistance = m_path->GetRemainingLength();
//	std::cout << "Remain: " << remainingDistance << std::endl;
	if(m_gradualStop){
		double maxAllowedSpeed = sqrt(2 * m_maxAccel * remainingDistance);
		if (fabs(speed) > maxAllowedSpeed){
			speed = maxAllowedSpeed * (speed / fabs(speed));
		}
	}
	double minSpeed = 20.0;
	if (fabs(speed) < minSpeed){
		speed = minSpeed * (speed / fabs(speed));
	}

	TankPosition2d::TankDelta rv(0,0,0);
	if(circle.first){
		rv = TankPosition2d::TankDelta(speed, 0,
				((circle.second.turnRight) ? -1 : 1) * fabs(speed) / circle.second.radius);
	} else {
		rv = TankPosition2d::TankDelta(speed, 0, 0);
	}
	m_lastTime = now;
	m_lastCommand = rv;
	return rv;

cout<<"TankAdaptivePursuit Position Updated"<<endl;
}

bool TankAdaptivePursuit::CheckEvent(std::string event) {
	return m_path->EventPassed(event);
}

TankAdaptivePursuit::Circle::Circle(TankTranslation2d cent, double rad,
		bool turn_right) {
	center = cent;
	radius = rad;
	turnRight = turn_right;
}

std::pair<bool, TankAdaptivePursuit::Circle> TankAdaptivePursuit::JoinPath(TankPosition2d pos,
		TankTranslation2d lookahead) {
	cout<<"TankAdaptivePursuit::Circle"<<endl;
	double x1 = pos.GetTranslation().GetX();
	double y1 = pos.GetTranslation().GetY();
	double x2 = lookahead.GetX();
	double y2 = lookahead.GetY();

	TankTranslation2d posToLookahead = pos.GetTranslation().Inverse().TranslateBy(lookahead);
	double crossProduct = posToLookahead.GetX() * pos.GetRotation().GetSin()
			- posToLookahead.GetY() * pos.GetRotation().GetCos();
	if(abs(crossProduct) < kE){
		return {false, Circle(TankTranslation2d(),0,0)};
	}

	double dx = x1 - x2;
	double dy = y1 - y2;
	double my = ((crossProduct > 0) ? -1 : 1) * pos.GetRotation().GetCos();
	double mx = ((crossProduct > 0) ? 1 : -1) * pos.GetRotation().GetSin();

	double crossTerm = mx * dx + my * dy;

	if(abs(crossTerm) < kE){
		return {false, Circle(TankTranslation2d(),0,0)};
	}

	return {true, Circle(
			TankTranslation2d((mx * (x1 * x1 - x2 * x2 - dy * dy) + 2 * my * x1 * dy) / (2 * crossTerm),
					(-my * (-y1 * y1 + y2 * y2 + dy * dy) + 2 * mx * y1 * dx) / (2 * crossTerm)),
					.5 * abs((dx * dx + dy * dy) / crossTerm), (crossProduct > 0))};
}
