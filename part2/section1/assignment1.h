#include <wiringPi.h>
#include <softPwm.h>

#ifndef _ASSIGNMENT_BODY_
#define _ASSIGNMENT_BODY_

// A. Pin number definitions (DO NOT MODIFY)
// Important! USE the pin numbers and WIRE.
// We use 8 sensors.
//
#define NO 0
#define YES 1

#define ON(pin) {                              \
        digitalWrite(pin,1);                    \
    }

#define OFF(pin) {                              \
        digitalWrite(pin,0);                    \
}

#define WRITE(pin, x){\
digitalWrite(pin,x);                    \
}

#define READ(pin) digitalRead(pin)

// 1. Button
#define PIN_BUTTON 0 //11
 
// 2. 2-Color LED
#define PIN_YELLOW 1 //12

// 3. Temperature
#define PIN_TEMP 4 //16

// 4. Tracking Sensor
#define PIN_TRACK 5 //18

// 5. Shock Sensor
#define PIN_SHOCK 6 //22

// 6. RGB(3-Color) LED
#define PIN_RED 7 //7
#define PIN_GREEN 8 //3
#define PIN_BLUE 9 //5

// 7. Auto-flash Red
#define PIN_ALED 12 //19

// 8. Buzzer
#define PIN_BUZZER 13 //21

#define DRIVING 10
#define STAY 9

#define TURN_BLUE() {                       \
        softPwmWrite(PIN_RED, 0);           \
        softPwmWrite(PIN_GREEN, 0);         \
        softPwmWrite(PIN_BLUE, 0xff);         \
    }

#define TURN_MAGENTA() {                           \
        softPwmWrite(PIN_RED, 0x76);             \
        softPwmWrite(PIN_GREEN, 0x00);           \
        softPwmWrite(PIN_BLUE, 0xee);             \
    }

#define TURN_RED() {                          \
        softPwmWrite(PIN_RED, 0xff);             \
        softPwmWrite(PIN_GREEN, 0);            \
        softPwmWrite(PIN_BLUE, 0);            \
    }

#define TURN_GREEN() {                            \
        softPwmWrite(PIN_RED, 0x0);            \
        softPwmWrite(PIN_GREEN, 0xff);             \
        softPwmWrite(PIN_BLUE, 0);              \
    }

// B. Shared structure
// All thread fuctions get a shared variable of the structure
// as the function parameter.
// If needed, you can add anything in this structure.
typedef struct shared_variable {
	int bProgramExit; // Once it is set to 1, the program will be terminated.
    int state;
    int temp;
    int shock;
    int track;
} SharedVariable;




// C. Functions
// You need to implement following functions.
// Do not change any function name here.
void init_shared_variable(SharedVariable* sv);
void init_sensors(SharedVariable* sv);

void body_button(SharedVariable* sv);
void body_twocolor(SharedVariable* sv);
void body_temp(SharedVariable* sv);
void body_track(SharedVariable* sv);
void body_shock(SharedVariable* sv);
void body_rgbcolor(SharedVariable* sv);
void body_aled(SharedVariable* sv);
void body_buzzer(SharedVariable* sv);



#endif
