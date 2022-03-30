#ifndef UTILITIES__H__
#define UTILITIES__H__



enum mode_set {IDLE, PWM, ITEST, HOLD, TRACK};

void set_mode(enum mode_set);
volatile enum mode_set get_mode(void);


void set_pwm(int a);
int get_pwm();


#endif