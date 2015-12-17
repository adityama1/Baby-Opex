#include "mbed.h"
#include "MRF24J40.h"

#include <string>

// RF tranceiver to link with handheld.
MRF24J40 mrf(p11, p12, p13, p14, p21);

// LEDs you can treat these as variables (led2 = 1 will turn led2 on!)
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

// Timer
Timer timer;

// Serial port for showing RX data.
Serial pc(USBTX, USBRX);

// Used for sending and receiving
char txBuffer[128];
char rxBuffer[128];
int rxLen;

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


//***************** You can start coding here *****************//
int main (void)
{
    uint8_t channel = 2;
    char input;
    int slots[4]={0,0,0,0};
    int i;


    //Set the Channel. 0 is default, 15 is max
    mrf.SetChannel(channel);

    while(1){
        i=0;
        while(1){
            pc.printf("Enter P to park or R to rerieve \n");
            input=pc.getc();
            if (input=='P'||input=='R'){
                break;
                }
        }

        if (input=='p'){
            while(i<4){
                if (slots[i]==0){
                    break;
                    }
                i++;
                }
            if (i==4){
                pc.printf("Garage full\n");
                }
            else {
                if (i==0){
                    strcpy(txBuffer, "Park_1");
                    }
                else if (i==1){
                    strcpy(txBuffer, "Park_2");
                    }
                else if (i==2){
                    strcpy(txBuffer, "Park_3");
                    }
                else{
                    strcpy(txBuffer, "Park_4");
                    }
                }
                rf_send(txBuffer, strlen(txBuffer)+1);
                pc.printf("Sent: %s\r\n", txBuffer);
            }
        else if (input=='R'){
            while(1){
                pc.printf("Enter Slot Number");
                input=pc.getc();
                if (input=='1'){
                    strcpy(txBuffer, "Retrieve_1");
                    break;
                    }
                else if (input=='2'){
                    strcpy(txBuffer, "Retrieve_2");
                    break;
                    }
                else if (input=='3'){
                    strcpy(txBuffer, "Retrieve_3");
                    break;
                    }
                else if(input=='4'){
                    strcpy(txBuffer, "Retrieve_4");
                    break;
                    }
                }
                rf_send(txBuffer, strlen(txBuffer)+1);
                pc.printf("Sent: %s\r\n", txBuffer);
                }
            while(1){
                rxLen = rf_receive(rxBuffer, 128);
                if (rxLen>0){
                    pc.printf("Received: %s\r\n", rxBuffer);
                    //strcpy(rxBuffer,"");
                    if(!strcmp("Done", rxBuffer)) {
                        break;
                        }
                    else if(!strcmp("Push_1", rxBuffer)) {
                        //recieve car
                        }
                    else if(!strcmp("Push_2", rxBuffer)) {
                        //recieve car
                        }
                    else if(!strcmp("Push_3", rxBuffer)) {
                        //recieve car
                        }
                    else if(!strcmp("Push_4", rxBuffer)) {
                        //recieve car
                        }
                    else if(!strcmp("Pull_1", rxBuffer)) {
                        //deliver car
                        }
                    else if(!strcmp("Pull_2", rxBuffer)) {
                        //deliver car
                        }
                    else if(!strcmp("Pull_3", rxBuffer)) {
                        //deliver car
                        }
                    else if(!strcmp("Pull_4", rxBuffer)) {
                        //deliver car
                        }
                    else if(!strcmp("Close_Flap", rxBuffer)) {
                        //close flap
                        strcpy(txBuffer, "Flap_Closed");
                        rf_send(txBuffer, strlen(txBuffer)+1);
                        pc.printf("Sent: %s\r\n", txBuffer);
                        }
                    else if(!strcmp("Open_Flap", rxBuffer)) {
                        //open flap
                        strcpy(txBuffer, "Flap_Opened");
                        rf_send(txBuffer, strlen(txBuffer)+1);
                        pc.printf("Sent: %s\r\n", txBuffer);
                        }
                    else if(!strcmp("Open_Gate_1", rxBuffer)) {
                        //close flap
                        strcpy(txBuffer, "Gate_1_Opened");
                        rf_send(txBuffer, strlen(txBuffer)+1);
                        pc.printf("Sent: %s\r\n", txBuffer);
                        }
                    else if(!strcmp("Open_Gate_2", rxBuffer)) {
                        //close flap
                        strcpy(txBuffer, "Gate_2_Opened");
                        rf_send(txBuffer, strlen(txBuffer)+1);
                        pc.printf("Sent: %s\r\n", txBuffer);
                        }
                    else if(!strcmp("Close_Gate_1", rxBuffer)) {
                        //close flap
                        strcpy(txBuffer, "Gate_1_Closed");
                        rf_send(txBuffer, strlen(txBuffer)+1);
                        pc.printf("Sent: %s\r\n", txBuffer);
                        }
                    else if(!strcmp("Close_Gate_2", rxBuffer)) {
                        //close flap
                        strcpy(txBuffer, "Gate_2_Closed");
                        rf_send(txBuffer, strlen(txBuffer)+1);
                        pc.printf("Sent: %s\r\n", txBuffer);
                        }
                    }
                }
            }
        }
