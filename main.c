#include "NU32.h"
#include "encoder.h"
#include <stdio.h>
#include "ina219.h"
#include "utilities.h"
#include "current.h"
#include "position.h"

#define BUF_SIZE 200

int main(void){
    char buffer[BUF_SIZE];
    NU32_Startup();
    set_mode(IDLE);
    NU32_LED1 = 1;
    NU32_LED2 = 1;
    printf("turn both LEDs off");
    __builtin_disable_interrupts();
    UART2_Startup();
    INA219_Startup();
    Position_ISR_Startup();
    Current_ISR_Startup();
    __builtin_enable_interrupts();

    while(1){
        NU32_ReadUART3(buffer, BUF_SIZE);
        NU32_LED1 = 1;
        switch(buffer[0]){

            case 'a':
            {
                char message[100];
                float current_ma = INA219_read_current();
                sprintf(message, "current is %f\r\n", (current_ma*3));
                NU32_WriteUART3(message);
                break;

            }
        
            
            case 'b':
            {
                char message[100];
                float current_ma = INA219_read_current();
                sprintf(message, "current is %f\r\n", current_ma);
                NU32_WriteUART3(message);
                break;

            }


            
            
            case 'c':
            {
                WriteUART2("a");
                while(!get_encoder_flag()){

                }
                set_encoder_flag(0);
                char message[100];
                int count = get_encoder_count();
                sprintf(message,"encoder angle: %d\r\n", count);
                NU32_WriteUART3(message);
                break;

            }

            case 'd':
            {
                WriteUART2("a");
                while(!get_encoder_flag()){

                }
                set_encoder_flag(0);
                char message[100];
                int count = get_encoder_count();
                double degrees = count*(360.0)/(4*334.0);
                sprintf(message, "Encoder angle in degrees %f\r\n",degrees);
                NU32_WriteUART3(message);
                break;
            }

            
            case 'e':
            {
                
                WriteUART2("b");
                break;
            }

            case 'f':
            {
                //set pwm
                char message[100];
                NU32_ReadUART3(message, 100);
                int c;
                sscanf(message, "%d", &c);
                set_pwm(c);
                set_mode(PWM);
                break;
            }

            case 'g':
            {
                //set current gains
                char message[100];
                float current_kp, current_ki;

                NU32_ReadUART3(message, 100);
                sscanf(message, "%f", &current_kp);

                NU32_ReadUART3(message, 100);
                sscanf(message, "%f", &current_ki);

                set_current_gains(current_kp, current_ki);
                break;
            }

            case 'h':
            {
                //get current gains
                char message[100];
                sprintf(message, "%f\r\n", get_current_kp());
                NU32_WriteUART3(message);

                sprintf(message, "%f\r\n", get_current_ki());
                NU32_WriteUART3(message);


                break;
            }


            case 'i':
            {
                //set position gains
               char message[100];
               float pos_kp, pos_ki, pos_kd;
               
               NU32_ReadUART3(message, 100);
               sscanf(message, "%f", &pos_kp);

               NU32_ReadUART3(message, 100);
               sscanf(message, "%f", &pos_ki);

               NU32_ReadUART3(message, 100);
               sscanf(message, "%f", &pos_kd);

               set_position_gains(pos_kp, pos_ki, pos_kd);
               break;
            }

            case 'j':
            {
                //get position gains
                char message[100];
                sprintf(message, "%f\r\n", get_position_kp());
                NU32_WriteUART3(message);

                sprintf(message, "%f\r\n", get_position_ki());
                NU32_WriteUART3(message);

                sprintf(message, "%f\r\n", get_position_kd());
                NU32_WriteUART3(message);


                break;
            }

            case 'k':
            {
                set_mode(ITEST);
                while(get_mode() == ITEST){

                }
                ITEST_print();
                break;
            }

            case 'l':
            {
                char message[100];
                NU32_ReadUART3(message, 100);
                float ang;
                sscanf(message, "%f", &ang);
                set_pose_ang(ang);
                set_mode(HOLD);
                break;

            }

            case 'm':
            {
                char message[100];
                NU32_ReadUART3(message, 100);
                int length = 0;
                sscanf(message, "%d", &length);
                set_desired_len(length);
                int i = 0;
                float pos = 0;
                for(i = 0; i < length; i++){
                    NU32_ReadUART3(message, 100);
                    sscanf(message, "%f", &pos);
                    set_desired_pos(i, pos);
                }
                break;

            }

            case 'o':
            {
                set_mode(TRACK);
                while(get_mode() == TRACK){

                }
                TRACK_print();
                break;
            }


            
            case 'p':
            {
                set_pwm(0);
                set_mode(IDLE);
                break;
            }



            case 'r':
            {
                char message[100];
                sprintf(message, "Get mode %d\r\n", get_mode());
                NU32_WriteUART3(message);
                break;
            }


            case 'q':
            {
                set_mode(IDLE);
                break;
            }

           
            

            default:
            {
                NU32_LED2 = 0; // turn on LED2 to indicate an error
                break;
            }

        }

    }

    return 0;
}