// Hello World to sweep a servo through its full range

#include "mbed.h"
#include "Servo.h"

Servo myservo(p26);
DigitalOut gate(p21);
DigitalOut flap(p22);
Serial pc(USBTX, USBRX);

//generating different frequencies using digital out. Since mbed multiplexes 1 PWM, we need to explicitly generate new frequencies.
void open_gate(){
        float t=3;
        float dcyc=1;
        int c=0;
      //  float i=t*1000/(dcyc+20);
        while(c<=(t*1000/(dcyc+20))){
         //1Khz
         gate = 1;
         wait_ms(dcyc);
         gate = 0;
         wait_ms(20);
         c+=1;

    }
    }
void close_gate(){
        float t=3;
        float dcyc=2;
        int c=0;
        float i=t*1000/(dcyc+20);
        while(c<=i){
         //1Khz
         gate = 1;
         wait_ms(dcyc);
         gate = 0;
         wait_ms(20);
        c+=1;
    }
    }

void open_flap(){
    float t=2;
    float dcyc=1;
    int c=0;
   // float i=t*1000/(dcyc+20);
    while(c<=(t*1000/(dcyc+20))){

     flap = 1;
     wait_ms(dcyc);
     flap = 0;
     wait_ms(20);
     c+=1;
        }
    }
void close_flap(){
    float t=2;
    float dcyc=2;
    int c=0;
    float i=t*1000/(dcyc+20);
    while(c<=i){
     flap = 1;
     wait_ms(dcyc);
     flap = 0;
     wait_ms(20);
    c+=1;
        }
    }
int main() {

while (true){
            char input;
            input=pc.getc();
            if (input=='q'){
                open_gate();
                }
            else if (input == 'w'){
                close_gate();
                }
            else if (input=='e'){
                open_flap();
                }
            else if (input=='r'){
                close_flap();
                }

       }
}

/*while(1){
    wait(4);
    myservo = 1.0;

    wait(4);

    myservo = -1;
    }*/
//
   // while(1){
//         //1Khz
//         s1 = 0;
//         wait_ms(1);
//         s1 = 0;
//         wait_ms(20);
//
//    }

    //for(float p=0; p<1.0; p += 0.1) {
//        myservo = p;
//        wait(0.2);
//    }
