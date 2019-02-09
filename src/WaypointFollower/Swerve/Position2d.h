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

	static Position2d FromTranslation(Translation2d tran);
	static Position2d FromRotation(Rotation2d rot);
	static Position2d FromVelocity(Delta delta);

	Translation2d GetTranslation();
	void SetTranslation(Translation2d tran);
	Rotation2d GetRotation();
	void SetRotation(Rotation2d rot);

	Position2d TransformBy(Position2d other);

	Position2d Inverse();

	Position2d Interpolate(Position2d other, double x);


};
