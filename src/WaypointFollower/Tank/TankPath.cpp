#include "TankPath.h"
#include "CORELogging/CORELog.h"

TankWaypoint::TankWaypoint(TankTranslation2d pos, double spd, std::string completeEvent) {
	cout<<"TankWaypoint::TankWaypoint"<<endl;
	position = pos;
	speed = spd;
	event = completeEvent;
}

/*
Path::Path(){
}
*/

TankPath::TankPath(std::vector<TankWaypoint> waypoints, bool flipY, bool flipX) {
	cout<<"TankPath::TankPath"<<endl;
	m_waypoints = waypoints;
	for (unsigned int i = 0; i < m_waypoints.size() - 1; ++i){
		if(flipX && flipY){
			m_segments.push_back(
					TankPathSegment(m_waypoints[i].position.Inverse(), m_waypoints[i+1].position.Inverse(), m_waypoints[i].speed));
		}else if(flipX){
			std::cout << "Flipped X" << std::endl;
			m_segments.push_back(
					TankPathSegment(m_waypoints[i].position.FlipX(), m_waypoints[i+1].position.FlipX(), m_waypoints[i].speed));
		}else if(flipY){
			std::cout << "Flipped Y" << std::endl;
			m_segments.push_back(
					TankPathSegment(m_waypoints[i].position.FlipY(), m_waypoints[i+1].position.FlipY(), m_waypoints[i].speed));
		}else{
			m_segments.push_back(
					TankPathSegment(m_waypoints[i].position, m_waypoints[i+1].position, m_waypoints[i].speed));
		}
		for (unsigned int i = 0; i < m_waypoints.size() - 1; ++i) {
			cout << m_waypoints[i].position.GetX() << endl;
			cout << m_waypoints[i].position.GetY() << endl;
		}
	}
	

	if(m_waypoints.size() > 0){
		if(m_waypoints[0].event != ""){
			m_events.push_back(m_waypoints[0].event);
		}
		m_waypoints.erase(m_waypoints.begin());
	}
}

double TankPath::Update(TankTranslation2d pos) {
	cout<<"TankPath::Update"<<endl;
	double rv = 0.0;
	for(unsigned int i = 0; i < m_segments.size(); i++){
//		PathSegment segment = m_segments[i];
		TankPathSegment::TankClosestPointReport closestPointReport = m_segments[i].GetClosestPoint(pos);
//		std::cout << "Index " << closestPointReport.index << std::endl;
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
				TankPathSegment::TankClosestPointReport nextClosestPointReport = m_segments[i+1].GetClosestPoint(pos);
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

bool TankPath::EventPassed(std::string event) {
	cout<<"TankPath::EventPassed"<<endl;
	return (find(m_events.begin(), m_events.end(), event) != m_events.end());
}

double TankPath::GetRemainingLength() {
	cout<<"TankPath::GetRemainingLength"<<endl;
	double length = 0.0;
	for (auto i: m_segments){
		length += i.GetLength();
	}
	return length;
}

TankPathSegment::TankSample TankPath::GetLookaheadPoint(TankTranslation2d pos,
		double lookahead) {
		cout<<"TankPath::GetLookaheadPoint"<<endl;
	if(m_segments.size() == 0){
		return TankPathSegment::TankSample(TankTranslation2d(), 0);
	}

	TankTranslation2d posInverse = pos.Inverse();
	if(posInverse.TranslateBy(m_segments[0].GetStart()).Norm() >= lookahead){
		return TankPathSegment::TankSample(m_segments[0].GetStart(), m_segments[0].GetSpeed());
	}
	for (unsigned int i = 0; i < m_segments.size(); ++i){
		TankPathSegment segment = m_segments[i];
		double distance = posInverse.TranslateBy(segment.GetEnd()).Norm();
		if(distance >= lookahead){
			std::pair<bool, TankTranslation2d> intersectionPoint = GetFirstCircleSegmentIntersection(segment,
					pos, lookahead);
			if(intersectionPoint.first){
				return TankPathSegment::TankSample(intersectionPoint.second, segment.GetSpeed());
			} else {
				std::cout << "Error. Bad things happened" << std::endl;
			}
		}
	}

	TankPathSegment lastSegment = m_segments[m_segments.size() - 1];
	TankPathSegment newLastSegment = TankPathSegment(lastSegment.GetStart(), lastSegment.Interpolate(10000),
			lastSegment.GetSpeed());
	std::pair<bool, TankTranslation2d> intersectionPoint = GetFirstCircleSegmentIntersection(newLastSegment, pos,
			lookahead);
	if(intersectionPoint.first){
		return TankPathSegment::TankSample(intersectionPoint.second, lastSegment.GetSpeed());
	} else {
		std::cout << "Error? REALLY Bad things happened" << std::endl;
		return TankPathSegment::TankSample(lastSegment.GetEnd(), lastSegment.GetSpeed());
	}
}

std::pair<bool, TankTranslation2d> TankPath::GetFirstCircleSegmentIntersection(
		TankPathSegment segment, TankTranslation2d center, double radius) {
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
		return {false, TankTranslation2d()};
	}

	double sqrtDiscriminant = sqrt(discriminant);
	TankTranslation2d posSolution = TankTranslation2d(
			(det * dy + ((dy < 0) ? -1 : 1) * dx * sqrtDiscriminant) / drSquared + center.GetX(),
			(-det * dx + abs(dy) * sqrtDiscriminant) / drSquared + center.GetY());
	TankTranslation2d negSolution = TankTranslation2d(
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

TankPath TankPath::FromFile(string fileName, bool flip) {
	cout<<"TankPath::FromFile"<<endl;
    CORELog::LogInfo("Loading File: " + fileName);
    string fileStarter = "/home/lvuser/Paths/";
    ifstream inFile(fileStarter + fileName);
    if (inFile.is_open()) {
        string text((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
        return FromText(text, flip);
    }
    cout << "Failed to open: " << fileName << endl;
    return TankPath();
}

TankPath TankPath::FromText(string text, bool flip) {
    vector<TankWaypoint> points;
    json json;
    try {
        json = json::parse(text);
    } catch (const exception& e) {
        CORELog::LogError("Error parsing json path! " + string(e.what()));
        return TankPath();
    }

    //CORELog::logInfo("Json text contents:\n" + json.dump(4));
    try {
        for (auto point : json) {
			// This may be wrong, because swerve code needs to be changed to tank code
            TankWaypoint waypoint({point["x"].get<double>(), point["y"].get<double>()}, 100);
            // if(flip) {
            //     waypoint.position = waypoint.position.FlipX();
            //     waypoint.rotation = waypoint.rotation.Inverse();
            // }
			cout<< "Waypoint loop" << point["x"].get<double>();
            if(point["name"].get<string>() != "point") {
                waypoint.event = point["name"].get<string>();
            }
            points.push_back(waypoint);
        }
    } catch (const exception& e) {
        CORELog::LogError("Error reading json path! " + string(e.what()));
        return TankPath();
    }

    if(!points.empty()){
        CORELog::LogInfo("Path with " + to_string(points.size()) + " points was loaded successfully.");
        return TankPath(points);
    } else{
        CORELog::LogError("Loaded path text was empty!");
        return TankPath();
    }
}