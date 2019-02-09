#include "Path.h"

Waypoint::Waypoint(Translation2d pos, double spd, std::string completeEvent) {
	position = pos;
	speed = spd;
	event = completeEvent;
}

Path::Path(std::vector<Waypoint> waypoints, bool flipY, bool flipX) {
	m_waypoints = waypoints;
	for (unsigned int i = 0; i < m_waypoints.size() - 1; ++i){
		if (flipX && flipY) {
			m_segments.push_back(
					PathSegment(m_waypoints[i].position.Inverse(), m_waypoints[i+1].position.Inverse(), m_waypoints[i].speed));
		} else if(flipX) {
			std::cout << "Flipped X" << std::endl;
			m_segments.push_back(
					PathSegment(m_waypoints[i].position.FlipX(), m_waypoints[i+1].position.FlipX(), m_waypoints[i].speed));
		} else if(flipY) {
			std::cout << "Flipped Y" << std::endl;
			m_segments.push_back(
					PathSegment(m_waypoints[i].position.FlipY(), m_waypoints[i+1].position.FlipY(), m_waypoints[i].speed));
		} else {
			m_segments.push_back(
					PathSegment(m_waypoints[i].position, m_waypoints[i+1].position, m_waypoints[i].speed));
		}
	}

	if(m_waypoints.size() > 0){
		if(m_waypoints[0].event != ""){
			m_events.push_back(m_waypoints[0].event);
		}
		m_waypoints.erase(m_waypoints.begin());
	}
}

double Path::Update(Translation2d pos) {
	double rv = 0.0;
	for(unsigned int i = 0; i < m_segments.size(); i++){
		PathSegment::ClosestPointReport closestPointReport = m_segments[i].GetClosestPoint(pos);
		if (closestPointReport.index >= .99){
			m_segments.erase(m_segments.begin() + i);
			if(m_waypoints.size() > 0){
				if(m_waypoints[0].event != ""){
					m_events.push_back(m_waypoints[0].event);
				}
				m_waypoints.erase(m_waypoints.begin());
			}
			return Update(pos);
		} else {
			if(closestPointReport.index > 0.0){
				m_segments[i].UpdateStart(closestPointReport.closestPoint);
			}

			rv = closestPointReport.distance;

			if(m_segments.size() > i + 1){
				PathSegment::ClosestPointReport nextClosestPointReport = m_segments[i+1].GetClosestPoint(pos);
				if(nextClosestPointReport.index > 0
						&& nextClosestPointReport.index < .99
						&& nextClosestPointReport.distance < rv){
					m_segments[i+1].UpdateStart(nextClosestPointReport.closestPoint);
					rv = nextClosestPointReport.distance;
					m_segments.erase(m_segments.begin() + i);
					if(m_waypoints.size() > 0){
						if(m_waypoints[0].event != ""){
							m_events.push_back(m_waypoints[0].event);
						}
						m_waypoints.erase(m_waypoints.begin());
					}
				}
			}
			break;
		}
	}
	return rv;
}

bool Path::EventPassed(std::string event) {
	return (find(m_events.begin(), m_events.end(), event) != m_events.end());
}

double Path::GetRemainingLength() {
	double length = 0.0;
	for (auto i: m_segments){
		length += i.GetLength();
	}
	return length;
}

PathSegment::Sample Path::GetLookaheadPoint(Translation2d pos,
		double lookahead) {
	if(m_segments.size() == 0){
		return PathSegment::Sample(Translation2d(), 0);
	}

	Translation2d posInverse = pos.Inverse();
	if(posInverse.TranslateBy(m_segments[0].GetStart()).Norm() >= lookahead){
		return PathSegment::Sample(m_segments[0].GetStart(), m_segments[0].GetSpeed());
	}
	for (unsigned int i = 0; i < m_segments.size(); ++i){
		PathSegment segment = m_segments[i];
		double distance = posInverse.TranslateBy(segment.GetEnd()).Norm();
		if(distance >= lookahead){
			std::pair<bool, Translation2d> intersectionPoint = GetFirstCircleSegmentIntersection(segment,
					pos, lookahead);
			if(intersectionPoint.first){
				return PathSegment::Sample(intersectionPoint.second, segment.GetSpeed());
			} else {
				std::cout << "Error? Bad things happened" << std::endl;
			}
		}
	}

	PathSegment lastSegment = m_segments[m_segments.size() - 1];
	PathSegment newLastSegment = PathSegment(lastSegment.GetStart(), lastSegment.Interpolate(10000),
			lastSegment.GetSpeed());
	std::pair<bool, Translation2d> intersectionPoint = GetFirstCircleSegmentIntersection(newLastSegment, pos,
			lookahead);
	if(intersectionPoint.first){
		return PathSegment::Sample(intersectionPoint.second, lastSegment.GetSpeed());
	} else {
		std::cout << "Error? REALLY Bad things happened" << std::endl;
		return PathSegment::Sample(lastSegment.GetEnd(), lastSegment.GetSpeed());
	}
}

std::pair<bool, Translation2d> Path::GetFirstCircleSegmentIntersection(
		PathSegment segment, Translation2d center, double radius) {
	double x1 = segment.GetStart().GetX() - center.GetX();
	double y1 = segment.GetStart().GetY() - center.GetY();
	double x2 = segment.GetEnd().GetX() - center.GetX();
	double y2 = segment.GetEnd().GetY() - center.GetY();
	double dx = x2 - x1;
	double dy = y2 - y1;
	double drSquared = dx * dx + dy * dy;
	double det = x1 * y2 - x2 * y1;

	double discriminant = drSquared *radius * radius - det * det;
	if (discriminant < 0){
		return {false, Translation2d()};
	}

	double sqrtDiscriminant = sqrt(discriminant);
	Translation2d posSolution = Translation2d(
			(det * dy + ((dy < 0) ? -1 : 1) * dx * sqrtDiscriminant) / drSquared + center.GetX(),
			(-det * dx + abs(dy) * sqrtDiscriminant) / drSquared + center.GetY());
	Translation2d negSolution = Translation2d(
			(det * dy - ((dy < 0) ? -1 : 1) * dx * sqrtDiscriminant) / drSquared + center.GetX(),
			(-det * dx - abs(dy) * sqrtDiscriminant) / drSquared + center.GetY());

	double posDot = segment.DotProduct(posSolution);
	double negDot = segment.DotProduct(negSolution);
	if (posDot < 0 && negDot >= 0){
		return {true, negSolution};
	} else if (posDot >= 0 && negDot < 0){
		return {true, posSolution};
	} else {
		if (abs(posDot) <= abs(negDot)){
			return {true, posSolution};
		} else {
			return {true, negSolution};
		}
	}
}