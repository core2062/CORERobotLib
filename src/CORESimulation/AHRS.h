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
    MOCK_METHOD0(GetPitch,
        float());
    MOCK_METHOD0(GetRoll,
        float());
    MOCK_METHOD0(GetYaw,
        float());
    MOCK_METHOD0(GetCompassHeading,
        float());
    MOCK_METHOD0(ZeroYaw,
        void());
    MOCK_METHOD0(IsCalibrating,
        bool());
    MOCK_METHOD0(IsConnected,
        bool());
    MOCK_METHOD0(GetByteCount,
        double());
    MOCK_METHOD0(GetUpdateCount,
        double());
    MOCK_METHOD0(GetLastSensorTimestamp,
        long());
    MOCK_METHOD0(GetWorldLinearAccelX,
        float());
    MOCK_METHOD0(GetWorldLinearAccelY,
        float());
    MOCK_METHOD0(GetWorldLinearAccelZ,
        float());
    MOCK_METHOD0(IsMoving,
        bool());
    MOCK_METHOD0(IsRotating,
        bool());
    MOCK_METHOD0(GetBarometricPressure,
        float());
    MOCK_METHOD0(GetAltitude,
        float());
    MOCK_METHOD0(IsAltitudeValid,
        bool());
    MOCK_METHOD0(GetFusedHeading,
        float());
    MOCK_METHOD0(IsMagneticDisturbance,
        bool());
    MOCK_METHOD0(IsMagnetometerCalibrated,
        bool());
    MOCK_METHOD0(GetQuaternionW,
        float());
    MOCK_METHOD0(GetQuaternionX,
        float());
    MOCK_METHOD0(GetQuaternionY,
        float());
    MOCK_METHOD0(GetQuaternionZ,
        float());
    MOCK_METHOD0(ResetDisplacement,
        void());
    MOCK_METHOD4(UpdateDisplacement,
        void(float accel_x_g, float accel_y_g, int update_rate_hz, bool is_moving));
    MOCK_METHOD0(GetVelocityX,
        float());
    MOCK_METHOD0(GetVelocityY,
        float());
    MOCK_METHOD0(GetVelocityZ,
        float());
    MOCK_METHOD0(GetDisplacementX,
        float());
    MOCK_METHOD0(GetDisplacementY,
        float());
    MOCK_METHOD0(GetDisplacementZ,
        float());
    MOCK_METHOD0(GetAngle,
        double());
    MOCK_METHOD0(GetRate,
        double());
    MOCK_METHOD1(SetAngleAdjustment,
        void(double angle));
    MOCK_METHOD0(GetAngleAdjustment,
        double());
    MOCK_METHOD0(Reset,
        void());
    MOCK_METHOD0(GetRawGyroX,
        float());
    MOCK_METHOD0(GetRawGyroY,
        float());
    MOCK_METHOD0(GetRawGyroZ,
        float());
    MOCK_METHOD0(GetRawAccelX,
        float());
    MOCK_METHOD0(GetRawAccelY,
        float());
    MOCK_METHOD0(GetRawAccelZ,
        float());
    MOCK_METHOD0(GetRawMagX,
        float());
    MOCK_METHOD0(GetRawMagY,
        float());
    MOCK_METHOD0(GetRawMagZ,
        float());
    MOCK_METHOD0(GetPressure,
        float());
    MOCK_METHOD0(GetTempC,
        float());
    MOCK_METHOD0(GetBoardYawAxis,
        AHRS::BoardYawAxis());
    MOCK_METHOD0(GetFirmwareVersion,
        std::string());
    MOCK_METHOD0(GetActualUpdateRate,
        int());
    MOCK_METHOD0(GetRequestedUpdateRate,
        int());
};
