#include "spi.h"

void spiNewInit() {
    OSCCONbits.SPLLEN=1; 
    OSCCONbits.SCS0=0;

    TRISAbits.TRISA0 = 1;	// AN0 = A/D Thermistor    pin2
    TRISAbits.TRISA1 = 1;	// AN1 = isense            pin3
    TRISAbits.TRISA2 = 1;	// AN2 temp                pin4
    TRISAbits.TRISA3 = 1;	// c1in1+                  pin5
    TRISAbits.TRISA4 = 1;	// c1out                   pin6
    TRISAbits.TRISA5 = 1;	// AN4 clamp               pin7
    TRISAbits.TRISA6 = 1;	// nc                      pin10
    TRISAbits.TRISA7 = 0;	// nc                      pin9
    
     APFCONbits.C2OUTSEL=0; // c2out on ra5
 APFCONbits.CCP1SEL=0;
 APFCONbits.SDOSEL=0;
 APFCONbits.SCKSEL=0;
 APFCONbits.SDISEL=0;
 APFCONbits.TXSEL=0; // 0= TX is on pin Rc6
 APFCONbits.RXSEL=0; // Rx on rc7
 APFCONbits.CCP2SEL=0; 
 
 SSPCON1bits.SSPM0=0;
 SSPCON1bits.SSPM1=1;
 SSPCON1bits.SSPM2=0;
 SSPCON1bits.SSPM3=0;   
 SSPCON1bits.CKP=1;
 SSPCON1bits.SSPEN=1; // maybe 1?
 SSPCON1bits.SSPOV=0;
 SSPCON1bits.WCOL=0;
 
     PSMC1CON  = 0x00;
 //-----TXSTA: TRANSMIT STATUS AND CONTROL REGISTER

    TXSTAbits.CSRC =1; // internal baud clock
    TXSTAbits.TX9 = 0; // 0=8 bits, 1 =9 bits
    TXSTAbits.TXEN=0;  // 1 =Tx en 
    TXSTAbits.SYNC=0;  // 0 = asynchronous mode
    TXSTAbits.SENDB=0; // break character
    TXSTAbits.BRGH=0;  // low speed
    TXSTAbits.TRMT= 1; // 1=transmit buffer empty
    TXSTAbits.TX9D=0;
    
 //---   RCSTA: RECEIVE STATUS AND CONTROL REGISTER(
    RCSTAbits.SPEN=1; // 1=serial port enable
    RCSTAbits.RX9=0;  // 0=8 bits
    RCSTAbits.SREN=0; // dont care, used for synchronous mode
    RCSTAbits.CREN=1; // 1 = en receiver
    RCSTAbits.ADDEN=0;// 0 = 8 bits, no address detection
    RCSTAbits.FERR=0; // 0 = no frame error
    RCSTAbits.OERR=0; // 0=no overrun error
    RCSTAbits.RX9D=0;
    
 //BAUDCON: BAUD RATE CONTROL REGISTER
    BAUDCONbits.ABDOVF=0; // auto baud timer did not overflow
    BAUDCONbits.ABDOVF=1; // receiver is idle
    BAUDCONbits.SCKP=0;  // 0 = Transmit non-inverted data to the TX/CK pin
    BAUDCONbits.BRG16=0;  // 0 = 8-bit Baud Rate Generator is used
    BAUDCONbits.WUE=1;    // 1 = receiver is idle until falling edge
    BAUDCONbits.ABDEN=0;  // 0 = Auto-Baud Detect mode is disabled
    
    // // SYNC = 0, BRGH = 0, BRG16 = 0
    SPBRGL=0x19; // 25 for 9600 baud
    SPBRGH=0x01;
    
     SSPSTAT = 0x80;

}


void spiInit(Spi_Type sType, Spi_Data_Sample sDataSample, Spi_Clock_Idle sClockIdle, Spi_Transmit_Edge sTransmitEdge){
     
    
    //APFCON: ALTERNATE PIN FUNCTION CONTROL REGISTER
 APFCONbits.C2OUTSEL=1; // c2out on ra5
 APFCONbits.CCP1SEL=0;
 APFCONbits.SDOSEL=0;
 APFCONbits.SCKSEL=0;
 APFCONbits.SDISEL=0;
 APFCONbits.TXSEL=0; // 0 = TX is on pin RC6
 APFCONbits.RXSEL=0; // Rx on rc7
 APFCONbits.CCP2SEL=0; 

 INTCONbits.GIE= 0;  // general interrupt enable. 1=en
 INTCONbits.PEIE=0;  // Peripheral Interrupt Enable bit 1=en
 INTCONbits.TMR0IE=0;//Timer0 Overflow Interrupt Enable bit 1=en
 INTCONbits.INTE=0;  // INT External Interrupt Enable bit 0=dis
 INTCONbits.IOCIE=0; // Interrupt-on-Change Enable bit 0=dis
 INTCONbits.INTF=0;  // Timer0 Overflow Interrupt Flag bit 1=overflow
 INTCONbits.IOCIF=0; //  Interrupt-on-Change Interrupt Flag bit 1= change state

 //PIE1: PERIPHERAL INTERRUPT ENABLE REGISTER 1
 PIE1bits.TMR1GIE =0 ; //Timer1 Gate Interrupt Enable bit 1=en
 PIE1bits.ADIE =0;     //A/D Converter (ADC) Interrupt Enable bit
 PIE1bits.RCIE=0;      //USART Receive Interrupt Enable bit
 PIE1bits.TXIE=0;      //USART Transmit Interrupt Enable bit
 PIE1bits.SSP1IE=0;    //Synchronous Serial Port (MSSP) Interrupt Enable bit
 PIE1bits.CCP1IE=0;    //CCP1 Interrupt Enable bit        
 PIE1bits.TMR2IE=0;    //TMR2 to PR2 Match Interrupt Enable bit        
 PIE1bits.TMR1IE=0;    //Timer1 Overflow Interrupt Enable bit

 PIR1bits.TMR1GIF=0; //Timer1 Gate Interrupt Flag bit
 PIR1bits.ADIF=0;    //A/D Converter Interrupt Flag bit   
 PIR1bits.RCIF=0;    //USART Receive Interrupt Flag bit
 PIR1bits.TXIF=0;    //USART Transmit Interrupt Flag bit
 PIR1bits.SSP1IF=0;  //Synchronous Serial Port (MSSP) Interrupt Flag bit            
 PIR1bits.CCP1IF=0;  //CCP1 Interrupt Flag bit            
 PIR1bits.TMR2IF=0;  //Timer2 to PR2 Interrupt Flag bit            
 PIR1bits.TMR1IF=0;  //Timer1 Overflow Interrupt Flag bit 
 
 SSPCON1bits.CKP=1;
 SSPCON1bits.SSPEN=0;
 SSPCON1bits.SSPM0 = 0;
SSPCON1bits.SSPM1 = 0;
SSPCON1bits.SSPM2 = 0;
SSPCON1bits.SSPM3 = 0;
SSPCON1bits.SSPOV=0;
SSPCON1bits.WCOL=0;
    
SSPSTAT = sDataSample | sTransmitEdge;

    LATBbits.LATB1 = 1; // Deselect Chip before initalization
    TRISCbits.TRISC5 = 1;   // ADXL345 SDO
    TRISCbits.TRISC4 = 0;   // ADXL345 SDI
    TRISBbits.TRISB1 = 0;   // Chip Select
    TRISBbits.TRISB6 = 0;   // ADXL345 SCLK
    

//    SSPCON1 = 0b00100010;
//    SSPSTAT = 0b01000000;
    CM2CON0bits.C2ON=0;   // 1 to enable
    CM1CON0bits.C1ON=0;   // 1 to enable
    CM3CON0bits.C3ON=0;   // 1 to enable


//    SSPCON1 = sType | sClockIdle; 
    SSPCON1bits.SSPEN = 1;  // Datasheet p.261

}

static void spiReceiveWait()
{
    while ( !SSPSTATbits.BF ); // Wait for Data Receive complete
}

char spiWrite(char dat)  //Write data to SPI bus
{
    SSP1BUF = dat;
//    __delay_ms(1);
    while (!SSPSTATbits.BF);
    return SSP1BUF;
//    return 'a';
}

unsigned spiDataReady() //Check whether the data is ready to read
{
    if(SSPSTATbits.BF)
        return 1;
    else
        return 0;
}

char spiRead() //REad the received data
{
     spiWrite(0x00);
//     spiWrite(0x00);spiWrite(0x00);spiWrite(0x00);
//     while (!SSPSTATbits.BF);
     return SSP1BUF;
//    c = spiWrite(0x00);
    //spiReceiveWait();        // wait until the all bits receive
//    return 'c'; // read the received data from the buffer
}
