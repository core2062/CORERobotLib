#pragma once

#warning Using Fake CANTalon

class CANTalon {
public:
    enum FeedbackDevice {
        CtreMagEncoder_Relative
    };
    CANTalon(int _) {}
    inline float Set(float _) {}
    inline int GetEncPosition() { return -1; }
    inline void SetEncPosition(int _) {}
    inline void SetFeedbackDevice(FeedbackDevice _) {}
};