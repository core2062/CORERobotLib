#pragma once

#include <memory>

namespace CORE {
    class CORETask {
    private:
        bool m_disabled;
    public:
        CORETask();
        virtual ~CORETask() {}
        virtual void robotInitTask() {}
        virtual void disabledTask() {}
        virtual void autonInitTask() {}
        virtual void autonEndTask() {}
        virtual void teleopInitTask() {}
        virtual void preLoopTask() {}
        virtual void postLoopTask() {}
        virtual void teleopEndTask() {}
        virtual void testInitTask() {}
        virtual void testTask() {}
        virtual void disableTasks(bool disable = true);
        virtual bool isDisabled();
	};
}
