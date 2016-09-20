#ifndef SRC_CORELIBRARY_COREHARDWARE_COREENCODER_H_
#define SRC_CORELIBRARY_COREHARDWARE_COREENCODER_H_

namespace CORE {
enum distanceUnit {
	FEET,
	INCHES,
	CENTIMETERS,
	METERS
};
class COREEncoder {
public:
	COREEncoder(int port, bool reversed = false);
	void setReversed(bool reversed = true);
	bool getReversed();
	double Get();
	double getDistance(distanceUnit unit = FEET);
	void setDistance(distanceUnit unit = FEET);
	void Set();
	void Reset();
private:
	double ticksToDistanceConversion[4];
};

}

#endif
