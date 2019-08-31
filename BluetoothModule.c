/*
 * File:   BluetoothModule.c
 * Author: ethgar.d
 *
 * Created on May 14, 2019, 11:07 AM
 */

#include "BluetoothModule.h"

void Initialize_Bluetooth()
{

OSCCONbits.SCS1 = 1; // internal clock
OSCCONbits.IRCF = 0b1111; // 8 MHz
//Set the pins of RX and TX//

//TRISAbits.TRISA0 = 0;
//TRISAbits.TRISA1 = 1;

TRISCbits.TRISC7=1;    // HC-05Receive            pin18
TRISCbits.TRISC6=0;    // HC-05Transmit           pin17

//Set the baud rate using the look up table in datasheet(pg114)//
SPBRGH = 0x01  ; //9600bps 16MHz Osc
SPBRGL = 0xA0;

TXSTAbits.SYNC=0;
RCSTAbits.CREN=1; //1 = Enables receiver
RCSTAbits.SPEN=1; //1 = Serial port enabled

BAUDCONbits.BRG16=1; //pooki: 0
//Turn on Asyc. Serial Port//
INTCONbits.GIE=1;
INTCONbits.PEIE=1;
TXSTAbits.TX9=0;
RCSTAbits.RX9=0;

TXSTAbits.BRGH=1;
TXSTAbits.TXEN=1;
PIE1bits.RCIE=1;

//Set 8-bit reception and transmission

}
//____BT initialized____//

//Function to load the Bluetooth Rx. buffer with one char.//
void BT_load_char(char byte)
{
TXREG = byte;
while(!PIR1bits.TXIF);
while(!TXSTAbits.TRMT);
}
//End of function//
//Function to broadcast data from RX. buffer//
void broadcast_BT()
{
//TXREG = 13;
__delay_ms(400);
}
//End of function//

//Function to Load Bluetooth Rx. buffer with string//
void BT_load_string(char* string)
{
while(*string)
BT_load_char(*string++);
broadcast_BT();
}
//End of function//
