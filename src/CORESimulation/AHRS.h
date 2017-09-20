#pragma once

#include "gmock/gmock.h"

class AHRS {
public:
    enum BoardAxis {
        kBoardAxisX = 0,
        kBoardAxisY = 1,
        kBoardAxisZ = 2,
    };

    struct BoardYawAxis
    {
        /* Identifies one of the board axes */
        BoardAxis board_axis;
        /* true if axis is pointing up (with respect to gravity); false if pointing down. */
        bool up;
    };

    enum SerialDataType {
        /**
         * (default):  6 and 9-axis processed data
         */
                kProcessedData = 0,
        /**
         * unprocessed data from each individual sensor
         */
                kRawData = 1
    };

    struct SPI {
        enum Port {
            kOnboardCS0, kOnboardCS1, kOnboardCS2, kOnboardCS3, kMXP
        };
    };
    struct SerialPort {
        enum Port { kOnboard = 0, kMXP = 1, kUSB = 2, kUSB1 = 2, kUSB2 = 3 };
    };

    AHRS(SPI::Port spi_port_id) {}
    AHRS(SerialPort::Port serial_port_id) {}

    AHRS(SPI::Port spi_port_id, uint8_t update_rate_hz) {}
    AHRS(SPI::Port spi_port_id, uint32_t spi_bitrate, int update_rate_hz) {}

    AHRS(SerialPort::Port serial_port_id, AHRS::SerialDataType data_type, int update_rate_hz) {}

    float GetPitch();
    float GetRoll();
    float GetYaw();
    float GetCompassHeading();
    void ZeroYaw();
    bool IsCalibrating();
    bool IsConnected();
    double GetByteCount();
    double GetUpdateCount();
    long GetLastSensorTimestamp();
    float GetWorldLinearAccelX();
    float GetWorldLinearAccelY();
    float GetWorldLinearAccelZ();
    bool IsMoving();
    bool IsRotating();
    float GetBarometricPressure();
    float GetAltitude();
    bool IsAltitudeValid();
    float GetFusedHeading();
    bool IsMagneticDisturbance();
    bool IsMagnetometerCalibrated();
    float GetQuaternionW();
    float GetQuaternionX();
    float GetQuaternionY();
    float GetQuaternionZ();
    void ResetDisplacement();
    void UpdateDisplacement(float accel_x_g, float accel_y_g, int update_rate_hz, bool is_moving);
    float GetVelocityX();
    float GetVelocityY();
    float GetVelocityZ();
    float GetDisplacementX();
    float GetDisplacementY();
    float GetDisplacementZ();
    double GetAngle();
    double GetRate();
    void SetAngleAdjustment(double angle);
    double GetAngleAdjustment();
    void Reset();
    float GetRawGyroX();
    float GetRawGyroY();
    float GetRawGyroZ();
    float GetRawAccelX();
    float GetRawAccelY();
    float GetRawAccelZ();
    float GetRawMagX();
    float GetRawMagY();
    float GetRawMagZ();
    float GetPressure();
    float GetTempC();
    AHRS::BoardYawAxis GetBoardYawAxis();
    std::string GetFirmwareVersion();
    int GetActualUpdateRate();
    int GetRequestedUpdateRate();
};