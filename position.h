#ifndef POSITION__H__
#define POSITION__H__

#include "NU32.h"
#include <xc.h>
#include <sys/attribs.h>
#include "ina219.h"
#include "position.h"
#include "utilities.h"
#include "encoder.h"




void set_position_gains(float kp, float ki, float kds);

float get_position_kp();

float get_position_ki();

float get_position_kd();

void TRACK_print();

void set_desired_len(int l);

void set_desired_pos(int index, float pos);

void set_pose_ang(float ang);



void Position_ISR_Startup();

// void __ISR(_TIMER_4_VECTOR, IPL4SOFT) TIMER4ISR(void);



#endif