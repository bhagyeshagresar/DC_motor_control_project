#include "position.h"
#include "current.h"
#include <stdio.h>


#define EINTMAX 1000
volatile float pos_kp = 100;
volatile float pos_ki = 0;
volatile float pos_kd = 10;

volatile float des_pos[2000];
volatile float actual_pos[2000];
volatile float desired_length = 0;
volatile float desired_angle = 0;

void set_position_gains(float kp, float ki, float kd){
    pos_kp = kp;
    pos_ki = ki;
    pos_kd = kd;
}

float get_position_kp(){
    return pos_kp;
}

float get_position_ki(){
    return pos_ki;
}

float get_position_kd(){
    return pos_kd;
}

void set_pose_ang(float ang){
    desired_angle = ang;
}



void set_desired_pos(int index, float pos){
    if (index < 2000){
        des_pos[index] = pos;

    }
}


void TRACK_print(){
    char message[100];
    sprintf(message, "%d\r\n", desired_length);
    NU32_WriteUART3(message);
    int i = 0;
    for(i = 0; i < desired_length; i++){
        sprintf(message, "%f %f\r\n", des_pos[i], actual_pos[i]);
        NU32_WriteUART3(message);
    }


}


void set_desired_len(int l){
    desired_length = l;
}

void Position_ISR_Startup(){
    T4CONbits.TCKPS = 3;
    PR4 = 49999;
    TMR4 = 0;
    T4CONbits.ON = 1;
    IPC4bits.T4IP = 4;
    IPC4bits.T4IS = 0;
    IFS0bits.T4IF = 0;
    IEC0bits.T4IE = 1;

}

void __ISR(_TIMER_4_VECTOR, IPL4SOFT) TIMER4ISR(void){
    IFS0bits.T4IF = 0;
    static int counter = 0;
    static float eint = 0;
    static float eprev = 0;

    switch(get_mode()){
        case TRACK:
        {
            WriteUART2("a");
            while(!get_encoder_flag()){

            }
            set_encoder_flag(0);
            float pos = get_encoder_count()*360.0/(4*334.0);
            actual_pos[counter] = pos;
            float e = des_pos[counter] - pos;
            eint = eint + e;
            if(eint > EINTMAX){
                eint = EINTMAX;
            }

            if(eint < -EINTMAX){
                eint = -EINTMAX;
            }
            
            float edot = e - eprev;
            eprev = e;
            float u = (pos_kp*e) + (pos_ki*eint) + (pos_kd*edot);

            set_desired_torque(u);
            counter++;
            if(counter == desired_length){
                counter = 0;
                eint = 0;
                eprev = 0;
                set_mode(IDLE);


            }
            break;

        }
    }

}