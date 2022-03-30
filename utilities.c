#include "utilities.h"
#include <xc.h>

volatile enum mode_set mode;

int pwm = 0;

volatile enum mode_set get_mode(void){
    return mode;
}

void set_mode(enum mode_set m){
    mode = m;
}

void set_pwm(int a){
    pwm = a;
}


int get_pwm(){
    return pwm;
}