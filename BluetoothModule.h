#include <xc.h>

#pragma config FOSC = INTOSC // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF // Code Protection bit (Program memory code protection is enable)
#pragma config BOREN = ON // Brown Out Detect (BOR enabled) //PWM OFF->ON??ON->OFF????????(10%??????????????????)

#define _XTAL_FREQ 16000000 // Frequency 8MHz


void Initialize_Bluetooth();
void BT_load_char(char byte);
void broadcast_BT();
void BT_load_string(char* string);