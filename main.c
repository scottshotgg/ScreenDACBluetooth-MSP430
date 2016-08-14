#include <msp430.h> 
#include <stdint.h>

// ****************** Globals ****************** //

// PORT 2 defines
int D0 = BIT0;
int D1 = BIT1;
int D2 = BIT2;
int D3 = BIT3;
int D4 = BIT4;
int D5 = BIT5;
int D6 = BIT6;
int D7 = BIT7;

// PORT 1 defines
int enable	= BIT4;
int RS 		= BIT5;

// ****************** Functions ****************** //

void delay(int amount) {
	int i = 0;
	for(i; i < amount; i++) {
		__delay_cycles(1000);
	}
}

void RSon() {
	delay(1);
	P1OUT |= RS;
}

void RSoff() {
	delay(1);
	P1OUT &= ~RS;
}

void enableScreen() {
	delay(10);
	P1OUT |= enable;
}

void disableScreen() {
	delay(10);
	P1OUT &= ~enable;
}

void pulseEnable() {
	enableScreen();
	delay(1);
	disableScreen();
}

void writeCommand(uint8_t value) {
	//P2OUT = 0;
	P2OUT = value;
	pulseEnable();
}

/*void writeCommand(uint8_t value) {
	int i = 0;
	for (i; i < 8; i++) {
		P2OUT |= ((value >> i) & 0x01);
	}
}*/

void writeChar(uint8_t value) {
	RSon();
	delay(1);
	writeCommand(value);
	delay(1);
}

void writeString(char* string) {
	int i = 0;
	for(i; i < sizeof(string)/sizeof(string[0]); i++) {
		writeChar(string[i]);
	}
}

void clearScreen() {
  delay(5);
  writeCommand(0x01);
  pulseEnable();
}

// ****************** Main ****************** //

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    // Compose the databits into an array
	int databits[8] = {D0, D1, D2, D3, D4, D5, D6, D7};
	
    // Port 1 setup
    P1DIR |= enable + RS;
    P1OUT &= ~(enable + RS);

    // Port 2 setup
    P2SEL &= ~(BIT6|BIT7);
    int i = 0;
    for(i; i < sizeof(databits)/sizeof(databits[0]); i++) {
    	P2DIR |= databits[i];
    	P2OUT &= ~databits[i];
    }

    delay(50);
    RSoff();

	delay(10);
	writeCommand(0x38);
	pulseEnable();
	delay(10);
	writeCommand(0x38);
	pulseEnable();

	delay(10);
	writeCommand(0x0C);
	pulseEnable();

	delay(10);
	writeCommand(0x01);
	pulseEnable();

	delay(1);
	//writeCommand(0x06);
	pulseEnable();

	delay(10);
	writeCommand(0xC0);
	pulseEnable();

	RSon();
	delay(10);
	writeChar(0x48);
	pulseEnable();

	pulseEnable();


}
