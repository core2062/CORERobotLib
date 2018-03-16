#pragma once

#include "Translation2d.h"

class Position2d{
public:
	struct Delta{
		double dx, dy, dtheta;

		Delta(double x, double y, double t);
	};

protected:
	Translation2d m_translation;
	Rotation2d m_rotation;

public:
	Position2d();
	Position2d(Translation2d tran, Rotation2d rot);
	Position2d(const Position2d& other);

	static Position2d fromTranslation(Translation2d tran);
	static Position2d fromRotation(Rotation2d rot);
	static Position2d fromVelocity(Delta delta);

	Translation2d getTranslation();
	void setTranslation(Translation2d tran);
	Rotation2d getRotation();
	void setRotation(Rotation2d rot);

	Position2d transformBy(Position2d other);

	Position2d inverse();

	Position2d interpolate(Position2d other, double x);


};
