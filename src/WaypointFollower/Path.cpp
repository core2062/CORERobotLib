#include "Path.h"

Waypoint::Waypoint(Translation2d pos, double spd, std::string setFlag) {
	position = pos;
	speed = spd;
	flag = setFlag;
}

Path::Path(){

}

Path::Path(std::vector<Waypoint> waypoints) {
	m_waypoints = waypoints;
	for (unsigned int i = 0; i < m_waypoints.size() - 1; ++i){
		m_segments.push_back(
				PathSegment(m_waypoints[i].position, m_waypoints[i+1].position, m_waypoints[i].speed));
	}

	if(m_waypoints.size() > 0){
		if(m_waypoints[0].flag != ""){
			m_flags.push_back(m_waypoints[0].flag);
		}
		m_waypoints.erase(m_waypoints.begin());
	}
}

double Path::update(Translation2d pos) {
	double rv = 0.0;
	for(unsigned int i = 0; i < m_segments.size() - 1; ++i){
		PathSegment segment = m_segments[i];
		PathSegment::ClosestPointReport closestPointReport = segment.getClosestPoint(pos);
		if (closestPointReport.index >= .99){
			m_segments.erase(m_segments.begin() + i);
			if(m_waypoints.size() > 0){
				if(m_waypoints[0].flag != ""){
					m_flags.push_back(m_waypoints[0].flag);
				}
				m_waypoints.erase(m_waypoints.begin());
			}
			return update(pos);
		} else {
			if(closestPointReport.index > 0.0){
				segment.updateStart(closestPointReport.closestPoint);
			}

			rv = closestPointReport.distance;

			if(m_segments.size() > i + 1){
				PathSegment next = m_segments[i+1];
				PathSegment::ClosestPointReport nextClosestPointReport = next.getClosestPoint(pos);
				if(nextClosestPointReport.index > 0
						&& nextClosestPointReport.index < .99
						&& nextClosestPointReport.distance < rv){
					next.updateStart(nextClosestPointReport.closestPoint);
					rv = nextClosestPointReport.distance;
					m_segments.erase(m_segments.begin() + i);
					if(m_waypoints.size() > 0){
						if(m_waypoints[0].flag != ""){
							m_flags.push_back(m_waypoints[0].flag);
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

bool Path::isFlagSet(std::string flag) {
	return (find(m_flags.begin(), m_flags.end(), flag) != m_flags.end());
}

double Path::getRemainingLength() {
	double length = 0.0;
	for (auto i: m_segments){
		length += i.getLength();
	}
	return length;
}

PathSegment::Sample Path::getLookaheadPoint(Translation2d pos,
		double lookahead) {
	if(m_segments.size() == 0){
		return PathSegment::Sample(Translation2d(), 0);
	}

	Translation2d posInverse = pos.inverse();
	if(posInverse.translateBy(m_segments[0].getStart()).norm() >= lookahead){
		return PathSegment::Sample(m_segments[0].getStart(), m_segments[0].getSpeed());
	}
	for (unsigned int i = 0; i < m_segments.size(); ++i){
		PathSegment segment = m_segments[i];
		double distance = posInverse.translateBy(segment.getEnd()).norm();
		if(distance >= lookahead){
			std::pair<bool, Translation2d> intersectionPoint = getFirstCircleSegmentIntersection(segment,
					pos, lookahead);
			if(intersectionPoint.first){
				return PathSegment::Sample(intersectionPoint.second, segment.getSpeed());
			} else {
				std::cout << "Error? Bad things happened" << std::endl;
			}
		}
	}

	PathSegment lastSegment = m_segments[m_segments.size() - 1];
	PathSegment newLastSegment = PathSegment(lastSegment.getStart(), lastSegment.interpolate(10000),
			lastSegment.getSpeed());
	std::pair<bool, Translation2d> intersectionPoint = getFirstCircleSegmentIntersection(newLastSegment, pos,
			lookahead);
	if(intersectionPoint.first){
		return PathSegment::Sample(intersectionPoint.second, lastSegment.getSpeed());
	} else {
		std::cout << "Error? REALLY Bad things happened" << std::endl;
		return PathSegment::Sample(lastSegment.getEnd(), lastSegment.getSpeed());
	}
}

std::pair<bool, Translation2d> Path::getFirstCircleSegmentIntersection(
		PathSegment segment, Translation2d center, double radius) {
	double x1 = segment.getStart().getX() - center.getX();
	double y1 = segment.getStart().getY() - center.getY();
	double x2 = segment.getEnd().getX() - center.getX();
	double y2 = segment.getEnd().getY() - center.getY();
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
			(det * dy + ((dy < 0) ? -1 : 1) * dx * sqrtDiscriminant) / drSquared + center.getX(),
			(-det * dx + abs(dy) * sqrtDiscriminant) / drSquared + center.getY());
	Translation2d negSolution = Translation2d(
			(det * dy + ((dy < 0) ? -1 : 1) * dx * sqrtDiscriminant) / drSquared + center.getX(),
			(-det * dx - abs(dy) * sqrtDiscriminant) / drSquared + center.getY());

	double posDot = segment.dotProduct(posSolution);
	double negDot = segment.dotProduct(negSolution);
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
