#include <xc.h>
#include "setup.h"
#include "global.h"

// configure 4 pins as digital outputs
void init_io(){
    // APFCON

    // ANSELA
    ANSELA = 0; // all pins digital IO

    // Set as outputs
    TRISA = 0;

    // Set outputs to 0
    LATA = 0;

}

// setup the clock
void init_clk(){
    //OSCCON
    OSCCONbits.SPLLEN = 0; // PLL disabled
    OSCCONbits.IRCF = 0b1010; // 500 kHz
    OSCCONbits.SCS = 00; // oscillator set in config word
}

// configure i2c slave
void init_i2c(){
    //SSPSTAT
    SSPSTATbits.SMP = 1;    // Slew rate control disabled for standard
                            // speed mode (100 kHz and 1 MHz)
    SSPSTATbits.CKE = 1;    // SMBus specific inputs (RPi testing)

    // APFCON
    APFCONbits.SDSEL = 1;   // SDA on RA3

    // TRISA
    TRISAbits.TRISA1 = 1;   // SCL input
    TRISAbits.TRISA3 = 1;   // SDA input

    //SSPCON1
    SSPCON1bits.CKP = 1;    // enable clock
    SSPCON1bits.SSPM = 0b0110; // I2C slave mode 7-bit address

    //SSPCON2
    SSPCON2bits.GCEN = 0;   // disable general call interrupts
    SSPCON2bits.ACKDT = 0;  // acknowledge
    SSPCON2bits.RCEN = 1;   // enable receive
    SSPCON2bits.SEN = 1;    // enable clock stretching

    //SSPCON3
    SSPCON3 = 0;

    //SSPMSK
    SSPMSK = 0xFF;
    SSPADD=I2C_ADDRESS << 1;

    PIR1bits.SSP1IF = 0; // clear MSSP interrupt
    PIE1bits.SSP1IE = 1; // enable MSSPinterrupt
    INTCONbits.PEIE = 1; // enable peripheral interrupts


    SSPCON1bits.SSPEN = 1;  // enable SDA and SCL functionality
}