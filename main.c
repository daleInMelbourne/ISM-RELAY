
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


// prototypes
void dly1mS(void);
void delay_mS(unsigned int dly);


/** VECTOR REMAPPING ***********************************************/
// On PIC18 devices, addresses 0x00, 0x08, and 0x18 are used for
// the reset, high priority interrupt, and low priority interrupt
// vectors.  However, the current Microchip USB bootloader occupies
// addresses 0x00-0xFFF. Therefore the bootloader code remaps these 
// vectors to new locations.
	
	#define REMAPPED_RESET_VECTOR_ADDRESS				0x1000
	#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x1008
	#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x1018

	void YourHighPriorityISRCode();
	void YourLowPriorityISRCode();
	
	extern void _startup (void);        // See c018i.c in your C18 compiler dir
	#pragma code REMAPPED_RESET_VECTOR = REMAPPED_RESET_VECTOR_ADDRESS
	void _reset (void)
	{
	    _asm goto _startup _endasm
	}

	#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS
	void Remapped_High_ISR (void)
	{
	     _asm goto YourHighPriorityISRCode _endasm
	}
	#pragma code REMAPPED_LOW_INTERRUPT_VECTOR = REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS
	void Remapped_Low_ISR (void)
	{
	     _asm goto YourLowPriorityISRCode _endasm
	}
	#pragma code
	
	
	//These are your actual interrupt handling routines.
	#pragma interrupt YourHighPriorityISRCode
	void YourHighPriorityISRCode()
	{
		//Check which interrupt flag caused the interrupt.
		//Service the interrupt
		//Clear the interrupt flag
		//Etc.
	}	//This return will be a "retfie fast", since this is in a #pragma interrupt section 

	#pragma interruptlow YourLowPriorityISRCode
	void YourLowPriorityISRCode()
	{
		//Check which interrupt flag caused the interrupt.
		//Service the interrupt
		//Clear the interrupt flag
		//Etc.
	
	}	//This return will be a "retfie", since this is in a #pragma interruptlow section 

	#pragma code

#define LEDgrn	LATCbits.LATC4			// green led
#define LEDred	LATCbits.LATC5			// red led
#define DELAY	167						// delay in mS (167 = 2seconds for all 12 pins)



void spi_slave(void) 
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
    OpenSPI(SLV_SSON, MODE_00, SMPEND); 
    SSPCON1bits.SSPEN = 1; //Idle state is high level, FOSC/64, enable SPI 
} 

void main(void)
{   

unsigned long t;	
unsigned char spiIn,spiOut;  
//	OSCCON = 0b00110000;
	OSCCON = 0b01100000;

	ANSEL = 0;						// all pins to digital mode
	ANSELH = 0;

	TRISB = 0;						// all pins to output;
	TRISC = 0;	
	
	T3CON = 0xb1;					// 16 bit, 8:1 prescale, running
		LATB = 0;
		LATC = 0;

spi_slave();

 
	while(1) {
/*
while(1){
		LATCbits.LATC4 ^= 1;
	//for(t=0; t<40;t++);
}
*/
 if (SSPSTATbits.BF) 
            { 
                spiIn = SSPBUF;  // Retrieve received data 
//                SSPBUF = 0xa5;//spiOut; // Send message back 

/*
if(spiIn == 0xaf){
        SSPBUF = 0xa5;//spiOut; // Send message back 
		LATCbits.LATC5 = 1;
		delay_mS(DELAY);
		LATCbits.LATC5 = 0;
		delay_mS(DELAY);
}*/
             

switch(spiIn){
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


void dly1mS(void)
{
	TMR3H = (0-1500)>>8;						// 1500 * (8/12) = 1000uS
	TMR3L = (0-1500)&0x0ff;					// must write TMR3 in this order
	PIR2bits.TMR3IF = 0;
	while(!PIR2bits.TMR3IF);				// wait 1mS
}
	
	
void delay_mS(unsigned int dly)
{
unsigned int x;
	for(x=0; x<dly; x++) dly1mS();
}	