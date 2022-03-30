# chapter 28 in python

# sudo apt-get install python3-pip
# python3 -m pip install pyserial
# sudo apt-get install python3-matplotlib

import matplotlib.pyplot as plt 
from statistics import mean 
def read_plot_matrix():
    n_str = ser.read_until(b'\n');  # get the number of data points to receive
    n_int = int(n_str) # turn it into an int
    print('Data length = ' + str(n_int))
    ref = []
    data = []
    data_received = 0
    while data_received < n_int:
        dat_str = ser.read_until(b'\n');  # get the data as a string, ints seperated by spaces
        dat_f = list(map(float,dat_str.split())) # now the data is a list
        ref.append(dat_f[0])
        data.append(dat_f[1])
        data_received = data_received + 1
    meanzip = zip(ref,data)
    meanlist = []
    for i,j in meanzip:
        meanlist.append(abs(i-j))
    score = mean(meanlist)
    t = range(len(ref)) # index array
    plt.plot(t,ref,'r*-',t,data,'b*-')
    plt.title('Score = ' + str(score))
    plt.ylabel('value')
    plt.xlabel('index')
    plt.show()

from genref import genRef

import serial
ser = serial.Serial('/dev/ttyUSB0',230400,rtscts=1)
print('Opening port: ')
print(ser.name)

has_quit = False
# menu loop
while not has_quit:
    print('PIC32 MOTOR DRIVER INTERFACE')
    # display the menu options; this list will grow
    print('\tc: get encoder counts \te: reset encoder \td: read encoder angle \tq: Quit') # '\t' is a tab
    # read the user's choice
    selection = input('\nENTER COMMAND: ')
    selection_endline = selection+'\n'
     
    # send the command to the PIC32
    ser.write(selection_endline.encode()); # .encode() turns the string into a char array
    
    # take the appropriate action
    # there is no switch() in python, using if elif instead
    if (selection == 'c'):
        print('Requesting encoder counts: ')
        n_str = ser.read_until(b'\n'); 
        n_int = int(n_str)
        print('Encoder counts = '+str(n_int)+'\n')
    elif (selection == 'r'):
        print('Requesting mode: ')
        n_str = ser.read_until(b'\n'); 
        n_int = int(n_str)
        print('Mode = '+str(n_int)+'\n')
    elif (selection == 'm'):
        ref = genRef('cubic')
        #print(len(ref))
        t = range(len(ref))
        plt.plot(t,ref,'r*-')
        plt.ylabel('ange in degrees')
        plt.xlabel('index')
        plt.show()
        # send 
        ser.write((str(len(ref))+'\n').encode())
        for i in ref:
            ser.write((str(i)+'\n').encode())
    elif (selection == 'o'):
        read_plot_matrix()
    elif (selection == 'q'):
        print('Exiting client')
        has_quit = True; # exit client
        # be sure to close the port
        ser.close()
    else:
        print('Invalid Selection ' + selection_endline)







import serial
ser = serial.Serial('/dev/ttyUSB0',230400,rtscts=1)
print('Opening port: ')
print(ser.name)

has_quit = False
# menu loop
while not has_quit:
    print('PIC32 MOTOR DRIVER INTERFACE')
    # display the menu options; this list will grow
    print('\ta: Read current sensor (ADC counts) \tb: Read current sensor (mA)') # '\t' is a tab
    print('\tc: Read encoder (counts)            \td: Read encoder (deg)')
    print('\te: Reset encoder                    \tf: Set PWM (-100 to 100)')
    print('\tg: Set Current Gains                \th: Get Current Gains')
    print('\ti: Set position gains               \tj: Get position gains')
    print('\tk: Test current control             \tl: Go to angle(deg)')
    print('\tm: Load step trajectory             \tn: Load cubic trajectory')
    print('\tq: Quit client                      \tr: Get mode')


    # read the user's choice
    selection = input('\nENTER COMMAND: ')
    selection_endline = selection+'\n'
     
    # send the command to the PIC32
    ser.write(selection_endline.encode()) # .encode() turns the string into a char array
    
    # take the appropriate action
    # there is no switch() in python, using if elif instead
    if (selection == 'a'):

        count = ser.read_until(b'\n')  # get the incremented number back
        print('The motor current is: ' + count.decode() + "ADC counts") # print it to the screen

    elif(selection == 'b'):
        count = ser.read_until(b'\n')  # get the incremented number back
        print('The motor current is: ' + count.decode() + "mA") # print it to the screen

    elif(selection == 'c'):
        count = ser.read_until(b'\n')  # get the incremented number back
        print('The motor angle is: ' + count.decode() + 'counts' '\n') # print it to the screen

    elif(selection == 'd'):
        count = ser.read_until(b'\n')  # get the incremented number back
        print('The motor angle is: ' + count.decode() + 'degrees' '\n') # print it to the screen
    
    elif(selection == 'e'):
        print('motor angle is zero')
    
    elif(selection == 'f'):
        print('what PWM value would you like [-100 to 100]?')
        duty_cycle = input('\nEnter Command: ')
        p = duty_cycle+ '\n'
        ser.write(p.encode())
    
    elif(selection == 'g'):
        curr_kp = input("\nEnter kp\n")
        curr_ki = input("\nEnter ki\n")

        
        p_kp = curr_kp+ '\n'
        p_ki = curr_ki+ '\n'

        ser.write(p_kp.encode())
        ser.write(p_ki.encode())
    
    elif(selection == 'h'):
        current_kp = ser.read_until(b'\n')
        current_ki = ser.read_until(b'\n')

        
        print(float(current_kp))
        print(float(current_ki))

    elif(selection == 'i'):
        
        pos_kp = input("\nEnter kp\n")
        pos_ki = input("\nEnter ki\n")
        pos_kd = input("\nEnter kd\n")

        p2_kp = pos_kp+ '\n'
        p2_ki = pos_ki+ '\n'
        p2_kd = pos_kd+ '\n'

        ser.write(p2_kp.encode())
        ser.write(p2_ki.encode())
        ser.write(p2_kd.encode())

    
    elif(selection == 'j'):
        pos_kp = ser.read_until(b'\n')
        pos_ki = ser.read_until(b'\n')
        pos_kd = ser.read_until(b'\n')


        print("The position kp is set to: " + pos_kp.decode())
        print("The position ki is set to: " + pos_ki.decode())
        print("The position kd is set to: " + pos_kd.decode())
    
    elif(selection == 'k'):
        read_plot_matrix()        

    elif(selection == 'l'):
        angle = input("\nEnter angle")
        p_ang = angle+ '\n'
        ser.write(p_ang.encode())

    
    elif(selection == 'p'):
        print("motor unpowered")


    elif (selection == 'r'):
        set_mode = ser.read_until(b'\n')
        print("current mode is: " + set_mode.decode() + "\n")

    elif (selection == 'q'):
        print('Exiting client')
        has_quit = True # exit client
        # be sure to close the port
        ser.close()


    else:
        print('Invalid Selection ' + selection_endline)



