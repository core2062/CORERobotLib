#include "SwervePath.h"

SwerveWaypoint::SwerveWaypoint(SwerveTranslation2d pos, SwerveRotation2d rot, double spd, 
	string completeEvent) {
	position = pos;
	speed = spd;
	event = completeEvent;
	rotation = rot;
}

SwervePath::SwervePath() : SwervePath({SwerveWaypoint(SwerveTranslation2d(), SwerveRotation2d(), 0)}, false) {

}

SwervePath::SwervePath(vector<SwerveWaypoint> waypoints, bool flip) {
	m_waypoints = waypoints;
	for (int i = 0; i < m_waypoints.size() - 1; i++) {
        m_segments.emplace_back(m_waypoints[i].position, m_waypoints[i+1].position,
                                (flip ? m_waypoints[i].rotation.Inverse() : m_waypoints[i].rotation),
                                m_waypoints[i].speed);
	}

/*	if(m_waypoints.size() > 0){
		if(m_waypoints[0].event != ""){
			m_events.push_back(m_waypoints[0].event);
		}
		m_waypoints.erase(m_waypoints.begin());
	}*/
}

SwervePath SwervePath::FromFile(string fileName, bool flip) {
    CORELog::LogInfo("Loading File: " + fileName);
    string fileStarter = "/home/lvuser/Paths/";
    ifstream inFile(fileStarter + fileName);
    if (inFile.is_open()) {
        string text((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
        return FromText(text, flip);
    }
    cout << "Failed to open: " << fileName << endl;
    return SwervePath();
}

SwervePath SwervePath::FromText(string text, bool flip) {
    vector<SwerveWaypoint> points;
    json json;
    try {
        json = json::parse(text);
    } catch (const exception& e) {
        CORELog::LogError("Error parsing json path! " + string(e.what()));
        return SwervePath();
    }

    //CORELog::logInfo("Json text contents:\n" + json.dump(4));
    try {
        for (auto point : json) {
            SwerveWaypoint waypoint({point["x"].get<double>(), point["y"].get<double>()},
                              SwerveRotation2d::FromRadians(point["theta"].get<double>()), 100);
            if(flip) {
                waypoint.position = waypoint.position.FlipX();
                waypoint.rotation = waypoint.rotation.Inverse();
            }
            if(point["name"].get<string>() != "point") {
                waypoint.event = point["name"].get<string>();
            }
            points.push_back(waypoint);
        }
    } catch (const exception& e) {
        CORELog::LogError("Error reading json path! " + string(e.what()));
        return SwervePath();
    }

    if(!points.empty()){
        CORELog::LogInfo("Path with " + to_string(points.size()) + " points was loaded successfully.");
        return SwervePath(points);
    } else{
        CORELog::LogError("Loaded path text was empty!");
        return SwervePath();
    }
}

double SwervePath::Update(SwerveTranslation2d pos) {
	double rv = 0.0;
	for(int i = 0; i < m_segments.size(); i++) {
//		PathSegment segment = m_segments[i];
		SwervePathSegment::SwerveClosestPointReport closestPointReport = m_segments[i].GetClosestPoint(pos);
//		cout << "Index " << closestPointReport.index << endl;
		if (closestPointReport.index >= .99){
			m_segments.erase(m_segments.begin() + i);
			if(!m_waypoints.empty()){
				if(!m_waypoints[0].event.empty()){
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
				SwervePathSegment::SwerveClosestPointReport nextClosestPointReport = m_segments[i+1].GetClosestPoint(pos);
				if(nextClosestPointReport.index > 0
						&& nextClosestPointReport.index < .99
						&& nextClosestPointReport.distance < rv){
					m_segments[i+1].UpdateStart(nextClosestPointReport.closestPoint);
					rv = nextClosestPointReport.distance;
					m_segments.erase(m_segments.begin() + i);
					if(!m_waypoints.empty()){
						if(!m_waypoints[0].event.empty()){
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

bool SwervePath::EventPassed(string event) {
	return (find(m_events.begin(), m_events.end(), event) != m_events.end());
}

double SwervePath::GetRemainingLength() {
	double length = 0.0;
	for (auto i: m_segments){
		length += i.GetLength();
	}
	return length;
}

SwervePathSegment::SwerveSample SwervePath::GetLookaheadPoint(SwerveTranslation2d pos, double lookahead) {
	if(m_segments.empty()){
		return {SwerveTranslation2d(), 0};
	}

	SwerveTranslation2d posInverse = pos.Inverse();
	if(posInverse.TranslateBy(m_segments[0].GetStart()).Norm() >= lookahead){
		return {m_segments[0].GetStart(), m_segments[0].GetSpeed()};
	}
	for (auto segment : m_segments) {
        double distance = posInverse.TranslateBy(segment.GetEnd()).Norm();
		if(distance >= lookahead){
			pair<bool, SwerveTranslation2d> intersectionPoint =
                    GetFirstCircleSegmentIntersection(segment, pos, lookahead);
			if(intersectionPoint.first){
				return {intersectionPoint.second, segment.GetSpeed()};
			} else {
				cout << "Error? Bad things happened" << endl;
			}
		}
	}

	SwervePathSegment lastSegment = m_segments[m_segments.size() - 1];
	SwervePathSegment newLastSegment = SwervePathSegment(lastSegment.GetStart(), lastSegment.Interpolate(10000),
                                             lastSegment.GetAngle(), lastSegment.GetSpeed());
	pair<bool, SwerveTranslation2d> intersectionPoint = GetFirstCircleSegmentIntersection(newLastSegment, pos,
			lookahead);
	if(intersectionPoint.first){
		return {intersectionPoint.second, lastSegment.GetSpeed()};
	} else {
		cout << "Error? REALLY Bad things happened" << endl;
		return {lastSegment.GetEnd(), lastSegment.GetSpeed()};
	}
}

pair<bool, SwerveTranslation2d> SwervePath::GetFirstCircleSegmentIntersection(SwervePathSegment segment, SwerveTranslation2d center,
                                                                       double radius) {
	double x1 = segment.GetStart().GetX() - center.GetX();
	double y1 = segment.GetStart().GetY() - center.GetY();
	double x2 = segment.GetEnd().GetX() - center.GetX();
	double y2 = segment.GetEnd().GetY() - center.GetY();
	double dx = x2 - x1;
	double dy = y2 - y1;
	double drSquared = dx * dx + dy * dy;
	double det = x1 * y2 - x2 * y1;

	double discriminant = drSquared *radius * radius - det * det;
	if (discriminant < 0) {
		return {false, SwerveTranslation2d()};
	}

	double sqrtDiscriminant = sqrt(discriminant);
	SwerveTranslation2d posSolution = SwerveTranslation2d(
			(det * dy + ((dy < 0) ? -1 : 1) * dx * sqrtDiscriminant) / drSquared + center.GetX(),
			(-det * dx + abs(dy) * sqrtDiscriminant) / drSquared + center.GetY());
	SwerveTranslation2d negSolution = SwerveTranslation2d(
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

SwerveWaypoint SwervePath::GetFirstWaypoint() {
    return m_waypoints[0];
}

SwervePosition2d SwervePath::GetClosestPoint(SwerveTranslation2d pos) {
    SwervePosition2d closestPoint = SwervePosition2d(m_segments[0].GetStart(), m_segments[0].GetAngle());
    double closestDistance = hypot(pos.GetX() - closestPoint.GetTranslation().GetX(),
                            pos.GetY() - closestPoint.GetTranslation().GetY());
    for (unsigned int i = 1; i < m_segments.size(); i++){
        SwervePathSegment segment = m_segments[i];
        double distance = hypot(pos.GetX() - segment.GetStart().GetX(),
                                pos.GetY() - segment.GetStart().GetY());
        if(distance < closestDistance) {
            closestPoint = SwervePosition2d(segment.GetStart(), segment.GetAngle());
            closestDistance = distance;
        }
    }
    return closestPoint;
}
