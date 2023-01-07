// #pragma once

// #include "SwerveTranslation2d.h"

// class SwervePosition2d{
// public:
// 	struct SwerveDelta{
// 		double dx, dy, dtheta;

// 		SwerveDelta(double x, double y, double t);
// 	};

// protected:
// 	SwerveTranslation2d m_translation;
// 	SwerveRotation2d m_rotation;

// public:
// 	SwervePosition2d();
// 	SwervePosition2d(SwerveTranslation2d tran, SwerveRotation2d rot);
// 	SwervePosition2d(const SwervePosition2d& other);

// 	static SwervePosition2d FromTranslation(SwerveTranslation2d tran);
// 	static SwervePosition2d FromRotation(SwerveRotation2d rot);
// 	static SwervePosition2d FromVelocity(SwerveDelta delta);

// 	SwerveTranslation2d GetTranslation();
// 	void SetTranslation(SwerveTranslation2d tran);
// 	SwerveRotation2d GetRotation();
// 	void SetRotation(SwerveRotation2d rot);

// 	SwervePosition2d TransformBy(SwervePosition2d other);

// 	SwervePosition2d Inverse();

// 	SwervePosition2d Interpolate(SwervePosition2d other, double x);


// };
