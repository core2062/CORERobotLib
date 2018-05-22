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
        virtual int GetPort() = 0;
        virtual void Set(double value) = 0;
        virtual ~ICOREMotor() = default;
    };

    class ICOREEncoder {
    public:
        virtual void Update() = 0;
        virtual int GetPortA() = 0;
        virtual int GetPortB() = 0;
        virtual void Reset() = 0;
        virtual bool IsBoundToTalonSRX() = 0;
        virtual int GetTalonSRXID() = 0;
        virtual ~ICOREEncoder() = default;
    };

	class COREHardwareManager : public CORETask {
	public:
		static void UpdateMotors();
		static void UpdateEncoders();
		static void ZeroMotors();
		static void ZeroEncoders();
		static void AddMotor(ICOREMotor* motor);
		static void AddEncoder(ICOREEncoder* encoder);
		static void CleanUp();
        void PreLoopTask() override;
    private:
		static vector<ICOREMotor*> m_motors;
		static vector<ICOREEncoder*> m_encoders;
	};
}
