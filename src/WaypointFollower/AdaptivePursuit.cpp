#include "AdaptivePursuit.h"

using namespace std;
using namespace CORE;

AdaptivePursuit::AdaptivePursuit(double fixedLookahead, double maxAccel, double nominalDt, Path path, bool reversed,
                                 double pathCompletionTolerance, bool gradualStop) :
    m_lastCommand(0, 0, 0) {
    m_fixedLookahead = fixedLookahead;
    m_maxAccel = maxAccel;
    m_path = path;
    m_dt = nominalDt;
    m_reversed = reversed;
    m_pathCompletionTolerance = pathCompletionTolerance;
    m_lastTime = 0.0;
    m_gradualStop = gradualStop;
}

bool AdaptivePursuit::isDone() {
    double remainingLength = m_path.getRemainingLength();
    return (remainingLength <= m_pathCompletionTolerance);
}

Position2d::Delta AdaptivePursuit::update(Position2d robotPos, double now) {
//    Position2d pos = robotPos;
//    if (m_reversed) {
//        pos = Position2d(robotPos.getTranslation(), robotPos.getRotation().rotateBy(Rotation2d::fromRadians(PI)));
//    }

    double distanceFromPath = m_path.update(robotPos.getTranslation());
    if (isDone()) {
        return {0, 0, 0};
    }

    PathSegment::Sample lookaheadPoint = m_path.getLookaheadPoint(robotPos.getTranslation(),
                                                                  distanceFromPath + m_fixedLookahead);
//    pair<bool, Circle> circle = joinPath(pos, lookaheadPoint.translation);

    double speed = lookaheadPoint.speed;
    if (m_reversed) {
        speed *= -1;
    }

    double dt = now - m_lastTime;
    if (!m_hasRun) {
        m_lastCommand = Position2d::Delta(0, 0, 0);
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

    double remainingDistance = m_path.getRemainingLength();
    CORELog::logInfo("Look Ahead Pos - X: " + to_string(lookaheadPoint.translation.getX()) + " Y: " + to_string(lookaheadPoint.translation.getY()));
    if (m_gradualStop) {
        double maxAllowedSpeed = sqrt(2 * m_maxAccel * remainingDistance);
        if (fabs(speed) > maxAllowedSpeed) {
            speed = maxAllowedSpeed * (speed / fabs(speed));
        }
    }
    double minSpeed = 4.0;
    if (fabs(speed) < minSpeed) {
        speed = minSpeed * (speed / fabs(speed));
    }

    Position2d::Delta rv(0, 0, 0);
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
        double y = lookaheadPoint.translation.getSin() * speed;
        double x = lookaheadPoint.translation.getCos() * speed;
        rv = Position2d::Delta(-x, y, 0);
//    } else {
//        rv = Position2d::Delta(speed, 0, 0);
//    }
    m_lastTime = now;
    m_lastCommand = rv;
    return rv;
}

bool AdaptivePursuit::checkEvent(string event) {
    return m_path.eventPassed(event);
}

AdaptivePursuit::Circle::Circle(Translation2d cent, double rad, bool turn_right) {
    center = cent;
    radius = rad;
    turnRight = turn_right;
}

pair<bool, AdaptivePursuit::Circle> AdaptivePursuit::joinPath(Position2d pos, Translation2d lookahead) {
    double x1 = pos.getTranslation().getX();
    double y1 = pos.getTranslation().getY();
    double x2 = lookahead.getX();
    double y2 = lookahead.getY();

    Translation2d posToLookahead = pos.getTranslation().inverse().translateBy(lookahead);

    double crossProduct = lookahead.getX() * pos.getRotation().getSin()
                          - posToLookahead.getY() * pos.getRotation().getCos();
    if (abs(crossProduct) < kE) {
        return {false, Circle(Translation2d(), 0, 0)};
    }

    double dx = x1 - x2;
    double dy = y1 - y2;
    double my = ((crossProduct > 0) ? -1 : 1) * pos.getRotation().getCos();
    double mx = ((crossProduct > 0) ? 1 : -1) * pos.getRotation().getSin();

    double crossTerm = mx * dx + my * dy;

    if (abs(crossTerm) < kE) {
        return {false, Circle(Translation2d(), 0, false)};
    }

    return {true, Circle(Translation2d((mx * (x1 * x1 - x2 * x2 - dy * dy) + 2 * my * x1 * dy) / (2 * crossTerm),
                                       (-my * (-y1 * y1 + y2 * y2 + dx * dx) + 2 * mx * y1 * dx) / (2 * crossTerm)),
                         .5 * abs((dx * dx + dy * dy) / crossTerm), (crossProduct > 0))};
}
