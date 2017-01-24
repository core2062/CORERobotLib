#pragma once

class CORETask {
private:
    bool m_disabled;
public:
    CORETask();
    virtual void robotInitTask() {}
    virtual void teleopInitTask() {}
    virtual void preTeleopTask() {}
    virtual void postTeleopTask() {}
    virtual void teleopEndTask() {}
    virtual void disableTasks(bool disable = true);
    virtual bool isDisabled();
    virtual ~CORETask(){}
};