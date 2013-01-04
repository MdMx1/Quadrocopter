#include "Definitions.h"
#include "LowPassFilter.h"
#include "RVector3D.h"
#include "TimerCount.h"
#include "Motor.h"
#include "MotorController.h"
#include "MySerial.h"
#include "PID.h"
#include "VoltageSensor.h"
#include "MPU6050DMP.h"

#ifndef QUADROCOPTER_H
#define QUADROCOPTER_H

class Quadrocopter
{
private:
    MotorController* MController;
    TimerCount DeltaT;
    MySerial* MSerial;
    VoltageSensor* VSensor;
    MPU6050DMP* MPU;

    // pins configuration
    int DefaultMotorPins[4] = {3, 9, 10, 11};
    int DefaultAccelPins[3] = {A0, A1, A2};
    int DefaultVSensorPin = A3;

    //reaction type (different types of processing sensors' data)
    enum reactionType_ {ReactionNone, ReactionAngularVelocity, ReactionAcceleration, ReactionAngle};
    reactionType_ reactionType = ReactionNone;

    // torque corrections
    RVector3D torqueAutomaticCorrection;

    RVector3D angleManualCorrection;

    double DefaultVSensorMaxVoltage = 5 / 1.02 * 2.77; //maximal voltage (before voltage divider)

    const double g = 9.80665; // gravitational acceleration

    //physical quantities
    RVector3D angle; // angle between Earth's coordinate and ours (filtered)
    RVector3D accelDataRaw; //data from accelerometer (g - a)
    LowPassFilter<RVector3D> accelData; // filtered data from accelerometer
    RVector3D angularVelocity; // filtered angular velocity from gyroscope
    RVector3D angularAcceleration; //discrete derivative of angular velocity
    double voltage; //accumulators voltage

    RVector3D angularVelocityPrev = RVector3D(); // for angular acceleration

    //corrections
    RVector3D accelerometerXi;
    static const double angleMaxCorrection = MPI / 4;
    static const double angularVelocityMaxCorrection = MPI / 4;

    PID pidAngle, pidAngularVelocity;
    RVector3D getAngleCorrection(RVector3D angle, double dt);
    RVector3D getAccelerationCorrection(RVector3D angle, RVector3D accelData0); // totally doesnt work
    RVector3D getAngularVelocityCorrection(RVector3D angularVelocity, double dt);

    double dt, dtMax;

    static const unsigned int serialReadN = 24; // bytes to read

public:
    Quadrocopter();

    void reset();

    void processSensorsData();
    void processCorrection();
    void processMotors();

    void processSerialCommand();
    void processSerialInterrupt();

    void iteration();

    RVector3D getTorques();
};

#endif // QUADROCOPTER_H
