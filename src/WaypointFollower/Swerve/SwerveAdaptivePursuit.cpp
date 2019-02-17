#include "SwerveAdaptivePursuit.h"

using namespace std;
using namespace CORE;

SwerveAdaptivePursuit::SwerveAdaptivePursuit(double fixedLookahead, double maxAccel, double rotationkP, double nominalDt, SwervePath path, bool reversed,
                                 double pathCompletionTolerance, bool gradualStop) :
    m_lastCommand(0, 0, 0) {
    m_fixedLookahead = fixedLookahead;
    m_maxAccel = maxAccel;
    m_rotationkP = rotationkP;
    m_path = path;
    m_dt = nominalDt;
    m_reversed = reversed;
    m_pathCompletionTolerance = pathCompletionTolerance;
    m_lastTime = 0.0;
    m_gradualStop = gradualStop;
}

bool SwerveAdaptivePursuit::IsDone() {
    double remainingLength = m_path.GetRemainingLength();
    return (remainingLength <= m_pathCompletionTolerance);
}

SwervePosition2d::SwerveDelta SwerveAdaptivePursuit::Update(SwervePosition2d robotPos, double now) {
    SwervePosition2d pos = robotPos;
    if (m_reversed) {
        pos = SwervePosition2d(robotPos.GetTranslation(), robotPos.GetRotation().RotateBy(SwerveRotation2d::FromRadians(PI)));
    }

    double distanceFromPath = m_path.Update(pos.GetTranslation());
    if (IsDone()) {
        return {0, 0, 0};
    }

    SwervePathSegment::SwerveSample lookaheadPoint = m_path.GetLookaheadPoint(pos.GetTranslation(),
                                                                  distanceFromPath + m_fixedLookahead);

    double speed = lookaheadPoint.speed;
    if (m_reversed) {
        speed *= -1;
    }

    double dt = now - m_lastTime;
    if (!m_hasRun) {
        m_lastCommand = SwervePosition2d::SwerveDelta(0, 0, 0);
        dt = m_dt;
        m_hasRun = true;
    }
    double lastSpeed = hypot(m_lastCommand.dx, m_lastCommand.dy) * 100;
    double accel = (speed - lastSpeed) / dt;
    if (accel < -m_maxAccel) {
        speed = lastSpeed - m_maxAccel * dt;
    } else if (accel > m_maxAccel) {
        speed = lastSpeed + m_maxAccel * dt;
    }

    double remainingDistance = m_path.GetRemainingLength() - 6;
    remainingDistance = remainingDistance < 0 ? 0 : remainingDistance;
    double maxAllowedSpeed = sqrt(2 * m_maxAccel * remainingDistance);
    if (fabs(speed) > maxAllowedSpeed) {
        speed = maxAllowedSpeed * (speed / fabs(speed));
    }

    //CORELog::logInfo("Speed: " + to_string(speed) + " Max Speed: " + to_string(maxAllowedSpeed) + " Dist: " + to_string(remainingDistance));
//    double minSpeed = 4.0;
//    if (fabs(speed) < minSpeed) {
//        speed = minSpeed * (speed / fabs(speed));
//    }

    SwervePosition2d::SwerveDelta rv(0, 0, 0);

//    if (circle.first) {
//        rv = Position2d::Delta(speed, 0, ((circle.second.turnRight) ? -1 : 1) * fabs(speed) / circle.second.radius);
//        CORELog::logInfo("Circle X: " + to_string(circle.second.center.Getx()));
//        CORELog::logInfo("Circle Y: " + to_string(circle.second.center.GetY()));
//        CORELog::logInfo("Circle Radians: " + to_string(circle.second.center.GetRadians()));
//        CORELog::logInfo("Circle Radius: " + to_string(circle.second.radius));

//        Attempt 1:
//        double y = -1 * ((circle.second.turnRight) ? -1 : 1) * (fabs(speed) * circle.second.radius) * cos(PI + circle.second.center.GetRadians());
//        double x = (fabs(speed) * circle.second.radius) * sin(PI + circle.second.center.GetRadians());
//        Attempt 2:
//        double y = sin(circle.second.center.GetRadians() + PI/2) * fabs(speed);
//        double x = cos(circle.second. center.GetRadians() + PI/2) * fabs(speed);

    SwerveTranslation2d move = robotPos.GetTranslation().Inverse().TranslateBy(lookaheadPoint.translation);
    double x = move.GetCos() * speed * 0.01;
    double y = move.GetSin() * speed * 0.01;

    /*double angDelta = robotPos.getRotation().inverse().rotateBy(m_path.getNextRotation()).getRadians();
    //double angSpeed = (angDelta) / dt;
    double angSpeed = 100;

    double lastAngSpeed = m_lastCommand.dtheta * 100;
    double angAccel = (angSpeed - lastAngSpeed) / dt;
    if (angAccel < -m_maxAngAccel) {
        angSpeed = lastAngSpeed - m_maxAngAccel * dt;
    } else if (angAccel > m_maxAngAccel) {
        angSpeed = lastAngSpeed + m_mm_startaxAngAccel * dt;
    }

    double maxAllowedAngSpeed = sqrt(2 * m_maxAngAccel * angDelta);
    if (abs(angSpeed) > maxAllowedAngSpeed) {
        angSpeed = maxAllowedAngSpeed * (angSpeed / fabs(angSpeed));
    }*/

    SwerveRotation2d setAngle = m_path.GetClosestPoint(pos.GetTranslation()).GetRotation();

    double remainingAng = COREVector::FromRadians(robotPos.GetRotation().GetRadians())
            .ShortestRotationTo(COREVector::FromRadians(setAngle.GetRadians()));

    /*double angSpeed = remainingAng < 0 ? -100 : 100;
    double lastAngSpeed = m_lastCommand.dtheta * 100;
    double angAccel = (angSpeed - lastAngSpeed) / dt;

    if (angAccel < -m_maxAngAccel) {
        angSpeed = lastAngSpeed - m_maxAngAccel * dt;
    } else if (angAccel > m_maxAngAccel) {
        angSpeed = lastAngSpeed + m_maxAngAccel * dt;
    }

    if (m_gradualStop) {
        double maxAllowedSpeed = sqrt(2 * m_maxAngAccel * remainingAng);
        if (fabs(angSpeed) > maxAllowedSpeed) {
            angSpeed = maxAllowedSpeed * (angSpeed / fabs(angSpeed));
        }
    }*/

    double angSpeed = remainingAng * m_rotationkP;

    rv = SwervePosition2d::SwerveDelta(x, y, angSpeed);
    m_lastTime = now;
    m_lastCommand = rv;
    return rv;
}

bool SwerveAdaptivePursuit::CheckEvent(string event) {
    return m_path.EventPassed(event);
}

SwerveAdaptivePursuit::Circle::Circle(SwerveTranslation2d cent, double rad, bool turn_right) {
    center = cent;
    radius = rad;
    turnRight = turn_right;
}

pair<bool, SwerveAdaptivePursuit::Circle> SwerveAdaptivePursuit::JoinPath(SwervePosition2d pos, SwerveTranslation2d lookahead) {
    double x1 = pos.GetTranslation().GetX();
    double y1 = pos.GetTranslation().GetY();
    double x2 = lookahead.GetX();
    double y2 = lookahead.GetY();

    SwerveTranslation2d posToLookahead = pos.GetTranslation().Inverse().TranslateBy(lookahead);

    double crossProduct = lookahead.GetX() * pos.GetRotation().GetSin()
                          - posToLookahead.GetY() * pos.GetRotation().GetCos();
    if (abs(crossProduct) < kE) {
        return {false, Circle(SwerveTranslation2d(), 0, 0)};
    }

    double dx = x1 - x2;
    double dy = y1 - y2;
    double my = ((crossProduct > 0) ? -1 : 1) * pos.GetRotation().GetCos();
    double mx = ((crossProduct > 0) ? 1 : -1) * pos.GetRotation().GetSin();

    double crossTerm = mx * dx + my * dy;

    if (abs(crossTerm) < kE) {
        return {false, Circle(SwerveTranslation2d(), 0, false)};
    }

    return {true, Circle(SwerveTranslation2d((mx * (x1 * x1 - x2 * x2 - dy * dy) + 2 * my * x1 * dy) / (2 * crossTerm),
                                       (-my * (-y1 * y1 + y2 * y2 + dx * dx) + 2 * mx * y1 * dx) / (2 * crossTerm)),
                         .5 * abs((dx * dx + dy * dy) / crossTerm), (crossProduct > 0))};
}
