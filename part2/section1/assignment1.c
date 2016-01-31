#include "assignment1.h"
#include <stdio.h>

#include <stdint.h>

void init_shared_variable(SharedVariable* sv) {
	// You can initialize the shared variable if needed.
	sv->bProgramExit = NO;
        sv->state = STAY;
        sv->temp = NO;
        sv->shock = NO;
        sv->track = NO;
}

void init_sensors(SharedVariable* sv) {
    pinMode(PIN_ALED, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);    
    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_YELLOW, OUTPUT);
    
    pinMode(PIN_BUTTON, INPUT);
    pinMode(PIN_SHOCK, INPUT);
    pinMode(PIN_TEMP, INPUT);
    pinMode(PIN_TRACK, INPUT);

    
    softPwmCreate(PIN_BLUE,0,0xff);
    softPwmCreate(PIN_RED,0,0xff);
    softPwmCreate(PIN_GREEN,0,0xff);
}

void body_button(SharedVariable* sv) {
    int button = READ(PIN_BUTTON);
    if (button == 0){
        if (sv->state == DRIVING){
            init_shared_variable(sv);
        }else{
            sv->state = DRIVING;
        }
    }
}

void body_twocolor(SharedVariable* sv) {
    if(sv->state == DRIVING){
        ON(PIN_YELLOW);
    }else{
        OFF(PIN_YELLOW);
    }
}

void body_temp(SharedVariable* sv) {    
    int temp = READ(PIN_TEMP);
    /* printf("TEMP: %d\n",temp);     */
    if(temp){
        sv->temp = YES;
    }else{
        sv->temp = NO;
    }    
}

void body_track(SharedVariable* sv) {
    int track = READ(PIN_TRACK);
    /* printf("TRACK: %d\n",track); */
    if(track == 0){
        sv->track = YES;
    }    
}

void body_shock(SharedVariable* sv) {
    int shock = READ(PIN_SHOCK);
    /* printf("SHOCK: %d\n",shock); */
    if(shock == 0){
        sv->shock = YES;
    }
}

void body_rgbcolor(SharedVariable* sv) {
    if(sv->state == STAY){
        TURN_BLUE();
        return;
    }
    if(sv->track || sv->shock){
        if(sv->track)
            TURN_MAGENTA();
        if(sv->shock)
            TURN_RED();
        return;
    }    
    TURN_GREEN();    
}

void body_aled(SharedVariable* sv) {
    if(sv->temp == YES){
        ON(PIN_ALED);
    }else{
        OFF(PIN_ALED);
    }
}

void body_buzzer(SharedVariable* sv) {
    if(sv->temp == YES){
        ON(PIN_BUZZER);        
    }else{
        OFF(PIN_BUZZER);
    }
}
