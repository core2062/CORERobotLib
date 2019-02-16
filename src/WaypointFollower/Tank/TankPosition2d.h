#pragma once

#include "TankTranslation2d.h"

class TankPosition2d{
public:
	struct TankDelta{
		double dx, dy, dtheta;

		TankDelta(double x, double y, double t);
	};

protected:
	TankTranslation2d m_translation;
	TankRotation2d m_rotation;

public:
	TankPosition2d();
	TankPosition2d(TankTranslation2d tran, TankRotation2d rot);
	TankPosition2d(const TankPosition2d& other);

	static TankPosition2d FromTranslation(TankTranslation2d tran);
	static TankPosition2d FromRotation(TankRotation2d rot);
	static TankPosition2d FromVelocity(TankDelta delta);

	TankTranslation2d GetTranslation();
	void SetTranslation(TankTranslation2d tran);
	TankRotation2d GetRotation();
	void SetRotation(TankRotation2d rot);

	TankPosition2d TransformBy(TankPosition2d other);

	TankPosition2d Inverse();

	TankPosition2d Interpolate(TankPosition2d other, double x);


};
