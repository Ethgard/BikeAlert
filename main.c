#include <pic16f1782.h>
#include <stdio.h>
#include "BluetoothModule.h"
#include <xc.h>

char readings[] = {0,0,0,0,0,0,0,0};
int counter = 0;
int maxCountingToAlert = 3;
int sumOfMovs = 0;
int accelTresh = 12; 
int measuresToDiscard = 10;
unsigned int lastValue = 99;


int calculateTresh() {
    int result=0;
    for(int i=0; i<8;i++){
        result *=2;
        result += readings[i];
        
    }
    return result;
}

void makeDelay() {
    for(int i=0; i<100;i++){}
}

int checkADXL(char y_offset) {
    
    SSPCON1bits.SSPEN=0;
    TRISCbits.TRISC3=0;    //rc3 is output pin
    TRISCbits.TRISC4=1;    //rc4 is input pin
    TRISCbits.TRISC5=0;    //rc5 is output pin
    
    TRISBbits.TRISB1 =0;
    PORTBbits.RB1 =1; // chip select on 1
    PORTCbits.RC3 =1; //clock on high


    
    //--------write to pwr_ctl-----
    //address
    PORTBbits.RB1 =0; //chip select 0
    
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =0;
    PORTCbits.RC3 =1; 
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =0;
    PORTCbits.RC3 =1;
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =1;

    PORTCbits.RC3 =1;  
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =0;

    PORTCbits.RC3 =1;
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =1;

    PORTCbits.RC3 =1;  
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =1;

    PORTCbits.RC3 =1;
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =0;
    PORTCbits.RC3 =1;  
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =1;

    PORTCbits.RC3 =1;
    
    //data
    
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =0;
    PORTCbits.RC3 =1; 
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =0;
    PORTCbits.RC3 =1;
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =0;

    PORTCbits.RC3 =1;  
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =0;

    PORTCbits.RC3 =1;
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =1;

    PORTCbits.RC3 =1;  
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =0;

    PORTCbits.RC3 =1;
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =0;
    PORTCbits.RC3 =1;  
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =0;

    PORTCbits.RC3 =1;

    PORTBbits.RB1 =1; //chip select 1

    //-----------------------------
    makeDelay();
    
    //--------write to dat_format--
    PORTBbits.RB1 =0; //chip select 0
    
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =0;
    PORTCbits.RC3 =1; 
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =0;
    PORTCbits.RC3 =1;
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =1;

    PORTCbits.RC3 =1;  
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =1;

    PORTCbits.RC3 =1;
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =0;

    PORTCbits.RC3 =1;  
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =0;

    PORTCbits.RC3 =1;
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =0;
    PORTCbits.RC3 =1;  
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =1;

    PORTCbits.RC3 =1;
    
    //data
    
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =0;
    PORTCbits.RC3 =1; 
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =1; //spi bit - 1 means 3-wire
    PORTCbits.RC3 =1;
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =0;

    PORTCbits.RC3 =1;  
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =0;

    PORTCbits.RC3 =1;
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =1;

    PORTCbits.RC3 =1;  
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =1;

    PORTCbits.RC3 =1;
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =0;
    PORTCbits.RC3 =1;  
    PORTCbits.RC3 =0;
                PORTCbits.RC5 =0;

    PORTCbits.RC3 =1;

    PORTBbits.RB1 =1; //chip select 1
    
    //-----------------------------
    
    makeDelay();
    
//    while(1) {
//        makeDelay();
            TRISCbits.TRISC5=0;    //rc5 is output pin

        PORTBbits.RB1 =1;
        PORTCbits.RC5 =0;
        PORTCbits.RC3 =1;
        
                    PORTBbits.RB1 =0;

            PORTCbits.RC3 =0;
                        PORTCbits.RC5 =1;
            PORTCbits.RC3 =1; 
            PORTCbits.RC3 =0;
                        PORTCbits.RC5 =0;
            PORTCbits.RC3 =1;
            PORTCbits.RC3 =0;
                        PORTCbits.RC5 =1;

            PORTCbits.RC3 =1;  
            PORTCbits.RC3 =0;
                        PORTCbits.RC5 =1;

            PORTCbits.RC3 =1;
            PORTCbits.RC3 =0;
                        PORTCbits.RC5 =0;

            PORTCbits.RC3 =1;  
            PORTCbits.RC3 =0;
                        PORTCbits.RC5 =0 + y_offset;

            PORTCbits.RC3 =1;
            PORTCbits.RC3 =0;
                        PORTCbits.RC5 =1 - y_offset;
            PORTCbits.RC3 =1;  
            PORTCbits.RC3 =0;
                        PORTCbits.RC5 =1;

            PORTCbits.RC3 =1;
                TRISCbits.TRISC5=1;    //rc5 is input pin

            PORTCbits.RC3 =0;
                        readings[0] = PORTCbits.RC5;
            PORTCbits.RC3 =1;  
            PORTCbits.RC3 =0;
                        readings[1] = PORTCbits.RC5;

            PORTCbits.RC3 =1;
            PORTCbits.RC3 =0;
                        readings[2] = PORTCbits.RC5;

            PORTCbits.RC3 =1;
            PORTCbits.RC3 =0;
                        readings[3] = PORTCbits.RC5;

            PORTCbits.RC3 =1;  
            PORTCbits.RC3 =0;
                        readings[4] = PORTCbits.RC5;

            PORTCbits.RC3 =1;
            PORTCbits.RC3 =0;
                        readings[5] = PORTCbits.RC5;

            PORTCbits.RC3 =1;  
            PORTCbits.RC3 =0;
                        readings[6] = PORTCbits.RC5;

            PORTCbits.RC3 =1;
            PORTCbits.RC3 =0;
                        readings[7] = PORTCbits.RC5;

            PORTCbits.RC3 =1;  
                    PORTBbits.RB1 =1;
                    
    int result = calculateTresh();
    return result;
     
}

int needToSendAlert(int movX, int movY, int tresh) {
    counter++;
    int result = 0; 
    if(movX > accelTresh){
        sumOfMovs ++;
    } 
    if(movY > accelTresh){
        sumOfMovs ++;
    } 
    if(counter > maxCountingToAlert){ //10- how many checks to do
        if(sumOfMovs > tresh){ // maximum can be 20 (10*2)
            result = 1;
        }
        
        sumOfMovs = 0;
        counter = 0;
    }
    
    return result;
}

void initA2D() {

    TRISAbits.TRISA0 = 1; //configure A0 (=RA0) as input
    ADCON0bits.ADON=1; //1=A/D enabled

    //ANSELA
    ANSELAbits.ANSA0 = 1;
    
    //ADCON0: A/D CONTROL REGISTER 0
    ADCON0bits.ADRMD = 1; // 10 bit resulotion
    ADCON0bits.CHS4=0;  
    ADCON0bits.CHS3=0;                            
    ADCON0bits.CHS2=0; //CHS 43210 00000=AN0
    ADCON0bits.CHS1=0;
    ADCON0bits.CHS0=0;    // AN0 selected
    ADCON0bits.GO_nDONE=0; // 1=in progress

    //ADCON1: A/D CONTROL REGISTER 1
    ADCON1bits.ADFM=1;   //A/D ADRESH Format Select bit 1=right justified
    ADCON1bits.ADCS2=1; //A/D Conversion Clock Select bits 001=fosc/8
    ADCON1bits.ADCS1=0;
    ADCON1bits.ADCS0=1;
    ADCON1bits.ADNREF=0;
    ADCON1bits.ADPREF1=0; // A/D Positive Voltage Reference Configuration bits
    ADCON1bits.ADPREF0=0; // 00= AVDD
    
    //ADCON2: 
    ADCON2bits.TRIGSEL3 =0;
    ADCON2bits.TRIGSEL2 =0;
    ADCON2bits.TRIGSEL1 =0;
    ADCON2bits.TRIGSEL0 =0;
    ADCON2bits.CHSN3 =1;
    ADCON2bits.CHSN2 =1;
    ADCON2bits.CHSN1 =1;
    ADCON2bits.CHSN0 =1;

    
}

unsigned int Read_ADC_Value(void)
{
    unsigned int ADCValue =0;
//    ADCON0bits.ADON=1; //1=A/D enabled

    ADCON0bits.GO_nDONE = 1;// start conversion
//    for(int i=0; i<5000;i++){}
    while (ADCON0bits.GO_nDONE);          // wait for conversion to finish
    ADCValue = ADRESH << 8;         // get the 2 msbs of the result and rotate 8 bits to the left
//    unsigned int adrsValue = ADRESL >> 6;
    ADCValue = ADCValue + ADRESL;   // now add the low 8 bits of the result into our return variable
//    ADCON0bits.ADON=0; //1=A/D enabled

    return (ADCValue);              // return the 12bit result in a single variable
}

unsigned int calculateADC() {
    unsigned int res = Read_ADC_Value();
//    res = res/1024;
//    res = res*5;
//    res = res*7;
//    res = res/2;
//    res = res/12;
//    res = res*100;
    res = res*35;
    res = res/24;
    res = res*25;
    res = res/256;
    
    
//    res = res*875;
//    res = res/6144;
    if(res > lastValue) {
        if(res < lastValue + 3) {
            res = lastValue;
        }
        
        
    }
    lastValue = res;
    return res;
}

void main() {
    Initialize_Bluetooth();
    initA2D();
    int tresh = 2;
    unsigned int readA2D;
    int countWhenToSendBat =0;
    while(1) {
        countWhenToSendBat++;
        makeDelay();
        int movX = checkADXL(0);
        int movY = checkADXL(1);
        int sendAlert = needToSendAlert(movX , movY, tresh);
        readA2D = calculateADC();
        if (sendAlert == 1) {
            BT_load_string("alert");
        }
        if(countWhenToSendBat%10 ==0){
            char out4[8];
            sprintf(out4,"%d", readA2D);
            BT_load_string(out4);
        }
        
    }
}

