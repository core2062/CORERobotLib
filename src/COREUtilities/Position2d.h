#pragma once

#include "COREVector.h"

class Position2d{
public:
	struct Delta{
		double dx, dy, dtheta;

		Delta(double x, double y, double t);
	};

protected:
	COREVector m_translation;
	COREVector m_rotation;

public:
	Position2d();
	Position2d(COREVector tran, COREVector rot);
	Position2d(const Position2d& other);

	static Position2d fromTranslation(COREVector tran);
	static Position2d fromRotation(COREVector rot);
	static Position2d fromVelocity(Delta delta);

	COREVector getTranslation();
	void setTranslation(COREVector tran);
	COREVector getRotation();
	void setRotation(COREVector rot);

	Position2d transformBy(Position2d other);

	Position2d inverse();

	Position2d interpolate(Position2d other, double x);


};
