#include <p18cxxx.h>

#include <stdio.h>
#include <string.h>
#include <delays.h>
#include <stdio.h>
#include <usart.h>
#include <timers.h>
#include "spi.h"
// 14K50 CONFIGURATION **************************************************/
// These are the same settings used by the bootloader and must not be changed here.

        #pragma config CPUDIV = NOCLKDIV
        #pragma config USBDIV = OFF
        #pragma config FOSC   = IRC
        #pragma config PLLEN  = ON
        #pragma config FCMEN  = OFF
        #pragma config IESO   = OFF
        #pragma config PWRTEN = OFF
        #pragma config BOREN  = OFF
        #pragma config BORV   = 30
        #pragma config WDTEN  = OFF
        #pragma config WDTPS  = 32768
        #pragma config MCLRE  = OFF
        #pragma config HFOFST = OFF
        #pragma config STVREN = ON
        #pragma config LVP    = OFF
        #pragma config XINST  = OFF
        #pragma config BBSIZ  = OFF
        #pragma config CP0    = OFF
        #pragma config CP1    = OFF
        #pragma config CPB    = OFF
        #pragma config WRT0   = OFF
        #pragma config WRT1   = OFF
        #pragma config WRTB   = OFF
        #pragma config WRTC   = OFF
        #pragma config EBTR0  = OFF
        #pragma config EBTR1  = OFF
        #pragma config EBTRB  = OFF       




unsigned char  spi_rx_flag=0, spi_rx_char; 


void initSpiSlave(void) 
{ 

    ANSELH = 0; // RC6 digital: SS 
    ANSEL = 0; 
    ADCON0bits.ADON = 0; //A/D off 
    ANSELH = 0; // RC6 digital: SS 
    ANSEL = 0; 
    TRISCbits.TRISC6 = 1; //Define SS as Input 
    TRISBbits.TRISB6 = 1; // Define clock pin as input 
    TRISBbits.TRISB4 = 1; // Define SDI as Input 
    TRISCbits.TRISC7 = 0; // Define SDO as Output 
    //SSPSTATbits.SMP = 0; // must be cleared in slave mode 
    //SSPSTATbits.CKE = 0; //Transmitting when from active to idle 
//    OpenSPI(SLV_SSOFF, MODE_00, SMPEND); 
    // Enable SPI interrupt 
	PIR1bits.SSPIF = 0;
    PIE1bits.SSPIE = 1; 
    // Make SPI receive interrupt high priority 
    IPR1bits.SSPIP = 1; 
    OpenSPI(SLV_SSON, MODE_00, SMPEND); 
    SSPCON1bits.SSPEN = 1; //Idle state is high level, FOSC/64, enable SPI 
} 



#pragma interrupt spi_rx_handler 

void spi_rx_handler (void){
	spi_rx_flag = 1; 
    spi_rx_char = SSPBUF;//getcSPI();
while(SSPSTATbits.BF);
	PIR1bits.SSPIF = 0;
    INTCONbits.GIE = 1; 
	} 
#pragma code rx_interrupt = 0x8 

// This function handles all high priority interrupts 
void rx_interrupt (void) 
{ 
    _asm goto spi_rx_handler _endasm 
} 
#pragma code 




void main(void)
{   

unsigned long t;	
unsigned char spiInCnt,spiIn[20],spiOut;  
//	OSCCON = 0b00110000;
	OSCCON = 0b01100000;

	ANSEL = 0;						// all pins to digital mode
	ANSELH = 0;

	TRISB = 0;						// all pins to output;
	TRISC = 0;	
	
	T3CON = 0xb1;					// 16 bit, 8:1 prescale, running
		LATB = 0;
		LATC = 0;

	initSpiSlave();

    // Enable all high priority interrupts 
    INTCONbits.GIE = 1; 
    INTCONbits.PEIE = 1; 
 	while(1) {

if (spi_rx_flag){
	spi_rx_flag = 0;
	switch(spi_rx_char){
		case 0x31:{
			if(PORTCbits.RC0 == 1) 		
				LATCbits.LATC0 = 0;
			else
				LATCbits.LATC0 = 1;
			}
			break;
		case 0x32:{ 		
			if(PORTCbits.RC1 == 1) 		
				LATCbits.LATC1 = 0;
			else
				LATCbits.LATC1 = 1;
			}
			break;
		case 0x33:{ 		
			if(PORTCbits.RC2 == 1) 		
				LATCbits.LATC2 = 0;
			else
				LATCbits.LATC2 = 1;
			}
			break;
		case 0x34:{ 		
			if(PORTCbits.RC3 == 1) 		
				LATCbits.LATC3 = 0;
			else
				LATCbits.LATC3 = 1;
			}
			break;
		case 0x35:{ 		
			if(PORTCbits.RC4 == 1) 		
				LATCbits.LATC4 = 0;
			else
				LATCbits.LATC4 = 1;
			}
			break;
		case 0x36:{ 		
			if(PORTCbits.RC5 == 1) 		
				LATCbits.LATC5 = 0;
			else
				LATCbits.LATC5 = 1;
			}
			break;
		case 0x37:{ 		
			if(PORTBbits.RB5 == 1) 		
				LATBbits.LATB5 = 0;
			else
				LATBbits.LATB5 = 1;
			}
			break;
		case 0x38:{ 		
			if(PORTBbits.RB7 == 1) 		
				LATBbits.LATB7 = 0;
			else
				LATBbits.LATB7 = 1;
			}
			break;
		}
}		
	} 
}

