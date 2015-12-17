#include "mbed.h"
#include "MRF24J40.h"

#include <string>

// RF tranceiver to link with handheld.
MRF24J40 mrf(p11, p12, p13, p14, p21);
InterruptIn in(p8);
// LEDs you can treat these as variables (led2 = 1 will turn led2 on!)
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

// Timer
Timer timer;
Timer t1;
// Serial port for showing RX data.
Serial pc(USBTX, USBRX);

// Used for sending and receiving
char txBuffer[128];
char rxBuffer[128];
int rxLen;
int t_period = 0;
float t_freq = 0;

//***************** Do not change these methods (please) *****************//

/**
* Receive data from the MRF24J40.
*
* @param data A pointer to a char array to hold the data
* @param maxLength The max amount of data to read.
*/
int rf_receive(char *data, uint8_t maxLength)
{
    uint8_t len = mrf.Receive((uint8_t *)data, maxLength);
    uint8_t header[8]= {1, 8, 0, 0xA1, 0xB2, 0xC3, 0xD4, 0x00};

    if(len > 10) {
        //Remove the header and footer of the message
        for(uint8_t i = 0; i < len-2; i++) {
            if(i<8) {
                //Make sure our header is valid first
                if(data[i] != header[i])
                    return 0;
            } else {
                data[i-8] = data[i];
            }
        }

        //pc.printf("Received: %s length:%d\r\n", data, ((int)len)-10);
    }
    return ((int)len)-10;
}

/**
* Send data to another MRF24J40.
*
* @param data The string to send
* @param maxLength The length of the data to send.
*                  If you are sending a null-terminated string you can pass strlen(data)+1
*/
void rf_send(char *data, uint8_t len)
{
    //We need to prepend the message with a valid ZigBee header
    uint8_t header[8]= {1, 8, 0, 0xA1, 0xB2, 0xC3, 0xD4, 0x00};
    uint8_t *send_buf = (uint8_t *) malloc( sizeof(uint8_t) * (len+8) );

    for(uint8_t i = 0; i < len+8; i++) {
        //prepend the 8-byte header
        send_buf[i] = (i<8) ? header[i] : data[i-8];
    }
    //pc.printf("Sent: %s\r\n", send_buf+8);

    mrf.Send(send_buf, len+8);
    free(send_buf);
}
void move_forward(){
    //fill here
    }
void stop(){
    //fill here
    }
void move_hv_transition_point_1(){
    move_forward();
    while (!(t_freq>=50 && t_freq<=150));
    stop();
    }
void close_gate1(){
    strcpy(txBuffer, "Close_Gate_1");
    rf_send(txBuffer, strlen(txBuffer)+1);
    pc.printf("Sent: %s\r\n", txBuffer);
    while(true){
        rxLen = rf_receive(rxBuffer, 128);
        if (rxLen>0){
            pc.printf("Received: %s\r\n", rxBuffer);
            if(!strcmp("Gate_1_Closed", rxBuffer)) {
                break;
                }
            }
        }
    }
void close_gate2(){
    strcpy(txBuffer, "Close_Gate_2");
    rf_send(txBuffer, strlen(txBuffer)+1);
    pc.printf("Sent: %s\r\n", txBuffer);
    while(true){
        rxLen = rf_receive(rxBuffer, 128);
        if (rxLen>0){
            pc.printf("Received: %s\r\n", rxBuffer);
            if(!strcmp("Gate_2_Closed", rxBuffer)) {
                break;
                }
            }
        }
    }
void open_gate1(){
    strcpy(txBuffer, "Open_Gate_1");
    rf_send(txBuffer, strlen(txBuffer)+1);
    pc.printf("Sent: %s\r\n", txBuffer);
    while(true){
        rxLen = rf_receive(rxBuffer, 128);
        if (rxLen>0){
            pc.printf("Received: %s\r\n", rxBuffer);
            if(!strcmp("Gate_1_Opened", rxBuffer)) {
                break;
                }
            }
        }
    }
void open_gate2(){
    strcpy(txBuffer, "Open_Gate_2");
    rf_send(txBuffer, strlen(txBuffer)+1);
    pc.printf("Sent: %s\r\n", txBuffer);
    while(true){
        rxLen = rf_receive(rxBuffer, 128);
        if (rxLen>0){
            pc.printf("Received: %s\r\n", rxBuffer);
            if(!strcmp("Gate_2_Opened", rxBuffer)) {
                break;
                }
            }
        }
    }
void open_flap(){
    strcpy(txBuffer, "Open_Flap");
    rf_send(txBuffer, strlen(txBuffer)+1);
    pc.printf("Sent: %s\r\n", txBuffer);
    while(true){
        rxLen = rf_receive(rxBuffer, 128);
        if (rxLen>0){
            pc.printf("Received: %s\r\n", rxBuffer);
            if(!strcmp("Flap_Opened", rxBuffer)) {
                break;
                }
            }
        }
    }
void close_flap(){
    strcpy(txBuffer, "Close_Flap");
    rf_send(txBuffer, strlen(txBuffer)+1);
    pc.printf("Sent: %s\r\n", txBuffer);
    while(true){
        rxLen = rf_receive(rxBuffer, 128);
        if (rxLen>0){
            pc.printf("Received: %s\r\n", rxBuffer);
            if(!strcmp("Flap_Closed", rxBuffer)) {
                break;
                }
            }
        }
    }
void move_upto_first_level(){
    move_forward();
    while (!(t_freq>=200 && t_freq<=300));
    stop();
}
void move_upto_second_level(){
    move_forward();
    while (!(t_freq>=450 && t_freq<=550));
    stop();
}
void push(int floor){
    //activate conveyor to push
    if (floor==1){
        strcpy(txBuffer, "Push_1");
        }
    else if (floor==2){
        strcpy(txBuffer, "Push_2");
        }
    else if (floor==3){
        strcpy(txBuffer, "Push_3");
        }
    else if (floor==4){
        strcpy(txBuffer, "Push_4");
        }
    rf_send(txBuffer, strlen(txBuffer)+1);
    pc.printf("Sent: %s\r\n", txBuffer);
    //stop conveyor after waiting sufficient time
    }
void pull(int floor){
    //activate conveyor to pull
    if (floor==1){
        strcpy(txBuffer, "Pull_1");
        }
    else if (floor==2){
        strcpy(txBuffer, "Pull_2");
        }
    else if (floor==3){
        strcpy(txBuffer, "Pull_3");
        }
    else if (floor==4){
        strcpy(txBuffer, "Pull_4");
        }
    rf_send(txBuffer, strlen(txBuffer)+1);
    pc.printf("Sent: %s\r\n", txBuffer);
    //stop conveyor after waiting sufficient time
    }
void move_hv_transition_point_2(){
    move_forward();
    while (!(t_freq>=650 && t_freq<=750));
    stop();
}
void move_downto_second_level(){
    move_forward();
    while (!(t_freq>=950 && t_freq<=1050));
    stop();
    }
void move_downto_first_level(){
    move_forward();
    while (!(t_freq>=1200 && t_freq<=1300));
    stop();
    }
void move_to_startpoint(){
    move_forward();
    while (!(t_freq>=1450 && t_freq<=1550));
    stop();
    }

//***************** You can start coding here *****************//
void flip() {

    t_period = t1.read_us();
    t_freq = (1/(float)t_period)*1000000;
    t1.reset();
}
int main (void)
{
    in.mode(PullDown);
    in.rise(&flip);
    char instruction[128];
    uint8_t channel = 2;

    //Set the Channel. 0 is default, 15 is max
    mrf.SetChannel(channel);

    //Start the timer
    timer.start();

    while(true) {
        while(true){
            rxLen = rf_receive(rxBuffer, 128);
            if (rxLen>0){
                pc.printf("Received: %s\r\n", rxBuffer);
                if((!strcmp("Park_1", rxBuffer))||(!strcmp("Park_2", rxBuffer))||(!strcmp("Park_3", rxBuffer))||(!strcmp("Park_4", rxBuffer))||(!strcmp("Retrieve_1", rxBuffer))||(!strcmp("Retrieve_2", rxBuffer))||(!strcmp("Retrieve_3", rxBuffer))||(!strcmp("Retrieve_4", rxBuffer))) {
                    pc.printf("Instruction Received: %s\r\n", rxBuffer);
                    strcpy(instruction,rxBuffer);
                    break;
                    }
                }
            }
        move_hv_transition_point_1();
        close_gate1();
        if ((!strcmp("Park_1",instruction))||(!strcmp("Retrieve_1",instruction))){
            move_upto_first_level();
            if (!strcmp("Park_1",instruction)){
                push(1);
                }
            else{
                pull(1);
                }
            }
        else if ((!strcmp("Park_2",instruction))||(!strcmp("Retrieve_2",instruction))){
            move_upto_second_level();
            if (!strcmp("Park_2",instruction)){
                push(2);
                }
            else{
                pull(2);
                }
            }
        move_hv_transition_point_2();
        open_gate1();
        open_flap();
        close_gate2();
        if ((!strcmp("Park_3",instruction))||(!strcmp("Retrieve_3",instruction))){
            move_downto_second_level();
            if (!strcmp("Park_3",instruction)){
                push(3);
                }
            else{
                pull(3);
                }
            }
        else if ((!strcmp("Park_4",instruction))||(!strcmp("Retrieve_4",instruction))){
            move_downto_first_level();
            if (!strcmp("Park_4",instruction)){
                push(4);
                }
            else{
                pull(4);
                }
            }
        move_to_startpoint();
        close_flap();
        open_gate2();



    }



    }
    }
