#pragma once

#include <memory>

namespace CORE {
    class CORETask {
    private:
        bool m_disabled;
    public:
        CORETask();
        virtual void robotInitTask() {}
        virtual void autonInitTask() {}
        virtual void teleopInitTask() {}
        virtual void preLoopTask() {}
        virtual void postLoopTask() {}
        virtual void teleopEndTask() {}
        virtual void disabledTask() {}
        virtual void disableTasks(bool disable = true);
        virtual bool isDisabled();
        virtual ~CORETask() {}
    };
}
