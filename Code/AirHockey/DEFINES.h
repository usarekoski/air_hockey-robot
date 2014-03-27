//ALL defines are here, written in ALL CAPITAL to avoid changing anything else.
//Fuctions and variables areNamedLikeThis for clarity and to distinguish them from these

// Dimensions etc.:
#define ARMLENGHT 180 // length of each four connecting arms
#define MOTORSPAN 300 // dimension between motors (center-to-center)
#define PII 3.141592653 // Pi

#define A_MAX 0.020 // m^2/s
#define V_MAX 0.050 // m/s

#define DPS 1.8 // degrees per step

#define MOVEUPDATE 1000// delay between move updates in microseconds

// Stepper drive:
#define LATCHPIN GPIO_FAST_IO3 // to motor driver shift register
#define STEPPERENABLEPIN 4 // High to enable steppers, low to turn them off. Good for standby

