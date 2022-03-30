#ifndef CURRENT__H__
#define CURRENT__H__

#include "NU32.h"
#include <xc.h>
#include <sys/attribs.h>
#include "ina219.h"
#include "position.h"
#include "utilities.h"





float get_current_kp();

float get_current_ki();


void set_desired_torque(float t);

void set_current_gains(float set_kp, float set_ki);

void Current_ISR_Startup();

void ITEST_print();



#endif