#include "current.h"
#include <stdio.h>

#define EINTMAX 1000



volatile float kp = 0.01;
volatile float ki = 0.05;
volatile float required_current[100];
volatile float actual_current[100];
volatile float required_torque = 0;

float get_current_kp(){
    return kp;
}

float get_current_ki(){
    return ki;
}

void set_desired_torque(float t){
    required_torque = t;
}

void set_current_gains(float set_kp, float set_ki){
    kp = set_kp;
    ki = set_ki;
}

void Current_ISR_Startup(){
    
    T3CONbits.TCKPS = 1;
    PR3 = 1999;
    TMR3 = 0;
    OC1CONbits.OCM = 0b110;
    OC1CONbits.OCTSEL = 1;
    OC1RS = 0;
    OC1R = 0;
    T3CONbits.ON = 1;
    OC1CONbits.ON = 1;

    TRISDbits.TRISD1 = 0;
    LATDbits.LATD1 = 0;

    T2CONbits.TCKPS = 2;
    PR2 = 3999;
    TMR2 = 0;
    T2CONbits.ON = 1;
    IPC2bits.T2IP = 6;
    IPC2bits.T2IS = 0;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;

}

void __ISR(_TIMER_2_VECTOR, IPL6SRS) Timer2ISR(void)
{
    IFS0bits.T2IF = 0;

    static int counter = 0;
    static float desired_current = 200.0;
    static float eint = 0;
    
    switch(get_mode()){
        case IDLE:
        {
            OC1RS = 0;
            LATDbits.LATD1 = 0;
            break;
        }


        case PWM:
        {
            int p = get_pwm();
            if(p > 0){
                OC1RS = p*1999/100;
                LATDbits.LATD1 = 1;

            }

            else {
                p = -p;
                OC1RS = p*1999/100;
                LATDbits.LATD1 = 0;

            }
            break;
        }


        // case ITEST:
        // {
        //     if(counter == 25){
        //         current_ma = -200.0;
        //     }

        //     if(counter == 50){
        //         current_ma = 200.0;
        //     }

        //     if(counter == 75){
        //         current_ma = -200.0;
        //     }

        //     float current = INA219_read_current();
        //     float e = current_ma - current;
        //     eint = eint + e;
        //     if(eint > EINTMAX){
        //         eint = EINTMAX;
        //     }
            
        //     if(eint < -EINTMAX){
        //         eint = -EINTMAX;
        //     }

        //     float u = kp*e + ki*eint;

        //     if(u > 100.0){
        //         u = -100.0;
        //     }

        //     if(u < -100.0){
        //         u = -100.0;
        //     }

        //     if(u > 0){
        //         OC1RS = u*1999/100;
        //         LATDbits.LATD1 = 1;
        //     }
        //     else{
        //         u = -u;
        //         OC1RS = u*1999/100;
        //         LATDbits.LATD1 = 0;
        //     }

        //     required_current[counter] = current_ma;
        //     actual_current[counter] = current;

        //     counter++;
        //     if(counter == 100){
        //         counter = 0;
        //         current_ma = 200.0;
        //         eint = 0;
        //         set_mode(IDLE);
        //         ITEST_print();
        //     }
        //     break;
        // }

         case ITEST:
        {
            if (counter == 25)
            {
                desired_current = -200.0;
            }
            if(counter == 50)
            {
                desired_current = 200.0;
            }
            if(counter == 75)
            {
                desired_current = -200.0;
            }

            float current = INA219_read_current();
            float e = desired_current - current;
            eint = eint + e;
            if (eint > EINTMAX)
            {
                eint = EINTMAX;
            }

            if (eint < -EINTMAX)
            {
                eint = -EINTMAX;
            }
            float u = kp*e + ki*eint;

            if(u > 100.0)
            {
                u = 100.0;
            }
            if(u < -100.0)
            {
                u = -100.0;
            }
            if(u > 0)
            {
                OC1RS = 1999*u/100;
                LATDbits.LATD1 = 1;
            }
            else {
                u = -u;
                OC1RS = 1999*u/100;
                LATDbits.LATD1 = 0;
            }
            required_current[counter] = desired_current;
            actual_current[counter] = current;

            counter++;
            if(counter == 100)
            {
                counter = 0;
                desired_current = 200.0;
                eint = 0;
                set_mode(IDLE);
            }
            break;
        }




        case TRACK:
        {
            float current = INA219_read_current();
            float e = required_torque - current;
            eint = eint + e;
            if(eint > EINTMAX){
                eint = EINTMAX;
            }

            if(eint < -EINTMAX){
                eint = -EINTMAX;
            }

            float u = (kp*e) + (ki*eint);

            if(u > 100.0){
                u = 100.0;
            }

            if(u < -100.0){
                u = -100.0;
            }

            if(u > 0){
                OC1RS = u*1999/100;
                LATDbits.LATD1 = 1;
            }
            else{
                u = -u;
                OC1RS = u*1999/100;
                LATDbits.LATD1 = 0;

            }
            break;


        }
        default:
        {
            break;
        }




        




    }

}
void ITEST_print(){
    char message[100];
    NU32_WriteUART3("100\r\n");
    int i = 0;
    for(i = 0; i < 100; i++){
        sprintf(message, "%f %f\r\n", required_current[i], actual_current[i]);
        NU32_WriteUART3(message);
    }
}

