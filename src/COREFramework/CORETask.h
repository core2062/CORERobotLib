#pragma once

#include <memory>

namespace CORE {
    class CORETask {
    private:
        bool m_disabled;
    public:
        CORETask();
        virtual ~CORETask() {}
        virtual void RobotInitTask() {}
        virtual void DisabledTask() {}
        virtual void AutonInitTask() {}
        virtual void AutonEndTask() {}
        virtual void TeleopInitTask() {}
        virtual void PreLoopTask() {}
        virtual void PostLoopTask() {}
        virtual void TeleopEndTask() {}
        virtual void TestInitTask() {}
        virtual void TestTask() {}
        virtual void DisableTasks(bool disable = true);
        virtual bool IsDisabled();
	};
}
