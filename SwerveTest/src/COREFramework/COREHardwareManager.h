#pragma once

#include <map>
#include <vector>
#include <memory>

#include "COREUtilities/CORETimer.h"
#include "COREFramework/CORETask.h"
#include "CORELogging/CORELog.h"

using namespace std;

namespace CORE {
    class ICOREMotor {
    public:
        virtual void Update() = 0;
        virtual int getPort() = 0;
        virtual void Set(double value) = 0;
        virtual ~ICOREMotor() = default;
    };

    class ICOREEncoder {
    public:
        virtual void Update() = 0;
        virtual int getPortA() = 0;
        virtual int getPortB() = 0;
        virtual void Reset() = 0;
        virtual bool isBoundToCANTalon() = 0;
        virtual int getCANTalonID() = 0;
        virtual ~ICOREEncoder() = default;
    };

	class COREHardwareManager : public CORETask {
	public:
		static void updateMotors();
		static void updateEncoders();
		static void zeroMotors();
		static void zeroEncoders();
		static void addMotor(ICOREMotor* motor);
		static void addEncoder(ICOREEncoder* encoder);
		static void cleanUp();
        void preLoopTask() override;
    private:
		static vector<ICOREMotor*> m_motors;
		static vector<ICOREEncoder*> m_encoders;
	};
}
