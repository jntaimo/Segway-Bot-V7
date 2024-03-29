#define WHEEL_SEPARATION 0.4 // Wheel separation in meters
#define WHEEL_RADIUS 0.06 // Wheel radius in meters

#define MIN_KP 0.0
#define MAX_KP 20.0

#define MIN_KI 0.0
#define MAX_KI 200.0

#define MIN_KD 0.0
#define MAX_KD 3.0

#define MIN_TRIM -0.01 //degrees
#define MAX_TRIM 0.01 //degrees

#define MAX_CONTROLLER_TILT 10//degrees
#define MAX_CONTROLLER_YAW 50//degrees/sec
#define CONTROLLER_DEADBAND 0.03 //minimum joystick value to register as a command

#define MAX_TILT 20 //degrees
#define CONTROLLER_TAU 0.3 //seconds

#define PRINT_DELAY 30 // Delay between printing to serial in milliseconds

struct Gains {
    double kp;
    double ki;
    double kd;
    double trim;
};

bool updatePIDParams(Gains &gains);
bool safeAngle(float angle, float trim, float maxSafeAngle);