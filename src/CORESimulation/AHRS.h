#pragma once

namespace SerialPort {
    enum Port {
        kUSB
    };
}

class AHRS {
public:
    enum SerialDataType {
        kProcessedData
    };
    AHRS(SerialPort::Port _, AHRS::SerialDataType __, int ___) {}
    inline void ZeroYaw() {}
    inline double GetYaw() {}
};