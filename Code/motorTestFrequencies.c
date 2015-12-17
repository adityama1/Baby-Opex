#include "mbed.h"
#include "Motor.h"
Serial pc(USBTX, USBRX);
//DigitalOut out1(p18);
//DigitalOut out2(p19);
PwmOut dcMotor(p23);
int t_period = 0;
float t_freq = 0;
Ticker tick;
InterruptIn in(p8);
Timer t1;

//Used a simple Motor lib
Motor m(p23, p18, p19); // pwm, fwd, rev

//To get the frequency read
void flip() {

    t_period = t1.read_us();
    t_freq = (1/(float)t_period)*1000000;
    t1.reset();
}
int main() {
        in.mode(PullDown);
    in.rise(&flip);
    t1.start();
    m.speed(-0.5);

    //printing out the frequency on the terminal to test whether the emitted frequencies are correct
    while (1){//!(t_freq>=950 && t_freq<=1050)) {
        pc.printf("%f Hz\r", t_freq);
        wait_ms(100);
    }
     m.speed(0.0);
    //char input;
    //while (true){
       // m.speed(0.5);
       //  wait(4);
       //  m.speed(-0.5);
       //  }

    /* input=pc.getc();
        if (input=='f'){
            m.speed(0.5);
            }
        else if (input=='b'){
            m.speed(-0.5);
            }
        else{
            m.speed(0);
            }
        }*/
   /* for (float s= -1.0; s < 1.0 ; s += 0.01) {
       m.speed(s);
      // m1.speed(s);
       wait(0.5);
    }*/
}
