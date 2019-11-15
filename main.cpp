#include "mbed.h"

#define std 0
#define fwd 1
#define right 2
#define left 3
#define rev 4
#define brk 5


DigitalOut Mot0_In1(D1); //Right Motor
DigitalOut Mot0_In2(D0); //Right Motor
DigitalOut Mot1_In1(D4); //Left Motor
DigitalOut Mot1_In2(D5); //Left Motor
DigitalOut Standby(D6);

PwmOut Mot0_speed(D2);
PwmOut Mot1_speed(D3);
//Mot0_speed.period_us(10);
//Mot1_speed.period_us(10);

AnalogIn sense_right(A0);
AnalogIn sense_left(A2);

volatile short input =0;

void input_generate();
void reverse();
void mov_forward();
void turn_left();
void turn_right();
void brake();
void standby_state();

void input_generate()
{
    float x,y=0;
    x = sense_left.read();
    y = sense_right.read();
    
    if (x<0.1270)
    x=1;
    else 
    x=0;
    
    if (y<0.1270)
    y=1;
    else 
    y=0; 
    
    input = (short)((10*x)+ y);
    
}




void mov_forward()
{
    Standby = 1;
    Mot0_speed = 0.5;
    Mot0_In1 = 1;
    Mot0_In2 = 0;
    
    Mot1_speed = 0.5;
    Mot1_In1 = 1;
    Mot1_In2 = 0;
        
}

void reverse()
{
    Standby = 1;
    Mot0_speed = 0.5;
    Mot0_In1 = 0;
    Mot0_In2 = 1;
    
    Mot1_speed = 0.5;
    Mot1_In1 = 0;
    Mot1_In2 = 1; 
}

void brake()
{

    Standby = 1;
    Mot0_speed = 0.5;
    Mot0_In1 = 1;
    Mot0_In2 = 1;
    
    Mot1_speed = 0.5;
    Mot1_In1 = 1;
    Mot1_In2 = 1; 
}

void turn_left()
{
    Standby = 1;
    Mot0_speed = 0.5;
    Mot0_In1 = 1;
    Mot0_In2 = 0;
    
    Mot1_speed = 0.5;
    Mot1_In1 = 0;
    Mot1_In2 = 1;

    
}

void turn_right()
{

    Standby = 1;
    Mot0_speed = 0.5;
    Mot0_In1 = 0;
    Mot0_In2 = 1;
    
    Mot1_speed = 0.5;
    Mot1_In1 = 1;
    Mot1_In2 = 0;

}

void standby_state()
{
    Standby = 1;
    Mot0_speed = 0.5;
    Mot0_In1 = 0;
    Mot0_In2 = 0;
    
    Mot1_speed = 0.5;
    Mot1_In1 = 0;
    Mot1_In2 = 0;
    
}



struct State
{
    void (*action) (void);
    float time;
    unsigned short next[4];
}; // For storing finite state machine



const State FSM[6] =
{{&standby_state,20,{fwd,left,right,brk}},
{&mov_forward, 20,{fwd,left,right,brk}},
{&turn_right, 50,{fwd,left,right,brk}},
{&turn_left, 50,{fwd,left,right,brk}},
{&reverse,1000,{fwd,left,right,right}},
{&brake, 10, {fwd,left,right,rev}}
}; // each array element represents each state

unsigned char s; //for storing present state

int main()
{
    s = 0;  //initial state
    
    while(true)
    {
        (FSM[s].action)(); //action  call
        wait_ms(FSM[s].time); //delay for state
        input_generate(); //get input from sensors
        s = FSM[s].next[input]; //assign next state
        }

} 



