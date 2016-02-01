//*****************************************************************************
//new Branch!!!
// MSP432 main.c
//
// CSci 4454 Assignment #1.2
// Flashing an RGB LED using interrupts
//
//  “Dolan clock”
//
// Myles Gavic
//
// MSP432 LaunchPad
// S1  P1.1
// S2  P1.4
//
// Modify your colored blinking light program from part 1 to use interrupts.
//
//****************************************************************************


#include <msp.h>


// mode state - default is self cycle.
unsigned int stop = 0;
// color - default is red
unsigned int color = 0;
// enabling the switche interupts
unsigned char enable = 1;


void selectionRegister1 (uint8_t bit)
{
	if (P1SEL0 & bit){
		if (P1SEL1 & bit)
			P1SELC|=bit; // Changing both at the same time
		else
			P1SEL0&=~bit; // Changing only the one in register 0
	}
	else if (P1SEL1 & bit)
		P1SEL1&=~bit; // Changing only the one in register 1
}

void selectionRegister2 (uint8_t bit)
{
	if (P2SEL0 & bit){
		if (P2SEL1 & bit )
			P2SELC|=bit ;
		else
			P2SEL0&=~bit;
	}
	else if (P2SEL1 & bit )
		P2SEL1&=~bit ;
}


void PortOneInterrupt ( void ){
	unsigned short iflag=P1IV ;
	P1OUT^=BIT0 ;

}


void initalizeLED(void){

	//RED
	P2DIR|=BIT0 ;
	selectionRegister2(BIT0) ;

	//GREEN
	P2DIR|=BIT1 ;
	selectionRegister2(BIT1) ;

	//BLUE
	P2DIR|=BIT2 ;
	selectionRegister2(BIT2) ;


}

void setColorRGB(unsigned int n){

	switch(n){

	//001 RED
	case 0 :
		P2OUT&=~BIT0 ;
		P2OUT&=~BIT1 ;
		P2OUT|=BIT2 ;
		break;
		//010 GREEN
	case 1 :
		P2OUT&=~BIT0 ;
		P2OUT|=BIT1 ;
		P2OUT&=~BIT2 ;
		break;
		//011 YELLOW
	case 2 :
		P2OUT&=~BIT0 ;
		P2OUT|=BIT1 ;
		P2OUT|=BIT2 ;
		break;
		//100 BLUE
	case 3 :
		P2OUT|=BIT0 ;
		P2OUT&=~BIT1 ;
		P2OUT&=~BIT2 ;
		break;
		//101 VIOLET
	case 4 :
		P2OUT|=BIT0 ;
		P2OUT&=~BIT1 ;
		P2OUT|=BIT2 ;
		break;
		//110 TEAL
	case 5 :
		P2OUT|=BIT0 ;
		P2OUT|=BIT1 ;
		P2OUT&=~BIT2 ;
		break;
		//111 WHITE
	case 6 :
		P2OUT|=BIT0 ;
		P2OUT|=BIT1 ;
		P2OUT|=BIT2 ;
		break;
		//000 OFF
	case 7 :
		P2OUT&=~BIT0 ;
		P2OUT&=~BIT1 ;
		P2OUT&=~BIT2 ;
		break;
	}
}

void cycleColors(void){
	if(color < 6){
		color = color + 1;
		setColorRGB(color);
	}else{
		color = 0;
		setColorRGB(color);
	}
}

void initalize(void){
	// Setting the direction of port 1 line 1 (setting to 0)
	P1DIR&=~BIT1;
	// Setting the direction of port 1 line 4 (setting to 0)
	P1DIR&=~BIT4;

	// Pullup resistors
	P1REN|=BIT1 ;
	P1OUT|=BIT1 ;

	P1REN|=BIT4 ;
	P1OUT|=BIT4 ;
}


void main (void)
{
	WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

	initalizeLED();
	initalize();
	cycleColors();


	P1IE=(BIT1 | BIT4 ) ;
	P1IES|=(BIT1 | BIT4 ) ;
	NVIC_EnableIRQ(PORT1_IRQn);

	while(1){
		// read input states
		unsigned char portInput = P1IN ;

		// switch light patterns
		if (!(portInput & BIT1))
			stop=!stop;

		if(stop){
			// If switch is pushed, advance a color.
			if(!(portInput & BIT4)){
				cycleColors();
			}

		}else{
			cycleColors();
		}

		//interval speed of the flashing light
		volatile int k=0;
		for ( k=0;k<40000;++k ) ;
	}
}
