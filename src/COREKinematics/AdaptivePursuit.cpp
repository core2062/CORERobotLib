#include "AdaptivePursuit.h"

using namespace std;

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
    Position2d pos = robotPos;
    if (m_reversed) {
        pos = Position2d(robotPos.getTranslation(),
                         robotPos.getRotation().RotateBy(COREVector::FromRadians(PI)));
    }

    double distanceFromPath = m_path.update(robotPos.getTranslation());
    if (isDone()) {
        return {0, 0, 0};
    }

    PathSegment::Sample lookaheadPoint = m_path.getLookaheadPoint(robotPos.getTranslation(),
                                                                  distanceFromPath + m_fixedLookahead);
    pair<bool, Circle> circle = joinPath(pos, lookaheadPoint.translation);

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
    double accel = (speed - m_lastCommand.dx) / dt;
    if (accel < -m_maxAccel) {
        speed = m_lastCommand.dx - m_maxAccel * dt;
    } else if (accel > m_maxAccel) {
        speed = m_lastCommand.dx + m_maxAccel * dt;
    }

    double remainingDistance = m_path.getRemainingLength();
    if (m_gradualStop) {
        double maxAllowedSpeed = sqrt(2 * m_maxAccel * remainingDistance);
        if (fabs(speed) > maxAllowedSpeed) {
            speed = maxAllowedSpeed * (speed / fabs(speed));
        }
    }
    double minSpeed = 20.0;
    if (fabs(speed) < minSpeed) {
        speed = minSpeed * (speed / fabs(speed));
    }

    Position2d::Delta rv(0, 0, 0);
    if (circle.first) {
        rv = Position2d::Delta(speed, 0, ((circle.second.turnRight) ? -1 : 1) * fabs(speed) / circle.second.radius);
    } else {
        rv = Position2d::Delta(speed, 0, 0);
    }
    m_lastTime = now;
    m_lastCommand = rv;
    return rv;
}

bool AdaptivePursuit::checkEvent(string event) {
    return m_path.eventPassed(event);
}

AdaptivePursuit::Circle::Circle(COREVector cent, double rad, bool turn_right) {
    center = cent;
    radius = rad;
    turnRight = turn_right;
}

pair<bool, AdaptivePursuit::Circle> AdaptivePursuit::joinPath(Position2d pos, COREVector lookahead) {
    double x1 = pos.getTranslation().GetX();
    double y1 = pos.getTranslation().GetY();
    double x2 = lookahead.GetX();
    double y2 = lookahead.GetY();

    COREVector posToLookahead = pos.getTranslation().MagnitudeInverse().TranslateBy(lookahead);

    double crossProduct = lookahead.GetX() * pos.getRotation().GetSin()
                          - posToLookahead.GetY() * pos.getRotation().GetCos();
    if (abs(crossProduct) < kE) {
        return {false, Circle(COREVector(), 0, 0)};
    }

    double dx = x1 - x2;
    double dy = y1 - y2;
    double my = ((crossProduct > 0) ? -1 : 1) * pos.getRotation().GetCos();
    double mx = ((crossProduct > 0) ? 1 : -1) * pos.getRotation().GetSin();

    double crossTerm = mx * dx + my * dy;

    if (abs(crossTerm) < kE) {
        return {false, Circle(COREVector(), 0, 0)};
    }

    return {true, Circle(COREVector((mx * (x1 * x1 - x2 * x2 - dy * dy) + 2 * my * x1 * dy) / (2 * crossTerm),
                                       (-my * (-y1 * y1 + y2 * y2 + dy * dy) + 2 * mx * y1 * dx) / (2 * crossTerm)),
                         .5 * abs((dx * dx + dy * dy) / crossTerm), (crossProduct > 0))};
}
