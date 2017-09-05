#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#include "global.h"
#include "config.h"
#include "util.h"
#include "setup.h"

//#define _TEST_

uint8_t LAT_lookup[16] = {
    // LAT value - command
    /* ________ RA 7 - N.C.
      | _______ RA 6 - N.C. 
      || ______ RA 5 - RX_SWITCH
      ||| _____ RA 4 - TX_SWITCH
      |||| ____ RA 3 - !EEPROM_WE
      ||||| ___ RA 2 - N.C.
      |||||| __ RA 1 - N.C.
      ||||||| _ RA 0 - LED
      ||||||||  */
    0b00000000, // 0b----0000
    0b00000001, // 0b----0001
    0b00001000, // 0b----0010
    0b00001001, // 0b----0011
    0b00010000, // 0b----0100
    0b00010001, // 0b----0101
    0b00011000, // 0b----0110
    0b00011001, // 0b----0111
    0b00100000, // 0b----1000
    0b00100001, // 0b----1001
    0b00101000, // 0b----1010
    0b00101001, // 0b----1011
    0b00110000, // 0b----1100
    0b00110001, // 0b----1101
    0b00111000, // 0b----1110
    0b00111001  // 0b----1111
};

uint8_t i2c_receive(uint8_t * data){
    uint8_t junk;
    uint8_t rv = 0;
    
    // If we get here that means we got a peripheral interrrupt
    if(PIR1bits.SSP1IF == 1){ // it's an I2C interrupt
        if(SSPSTATbits.R_nW){ // Master read
            if(!SSPSTATbits.D_nA){         // last byte was an address (D_nA = 0)
                junk = SSPBUF;// dummy read to clear BF bit
            }
            SSPBUF = 0x00;
            SSPCON1bits.CKP = 1;    // release CLK
        }
        if(!SSPSTATbits.R_nW){ // master write (R_nW = 0)
            if(!SSPSTATbits.D_nA){        // last byte was an address (D_nA = 0)
                junk = SSPBUF;			// read buffer to clear BF
                SSPCON1bits.CKP = 1;            // release CLK
            }
            else{               // last byte was data (D_nA = 1)
                *data = SSPBUF; // we only expect one
                rv = 1;
            }
            if(SSPCON1bits.WCOL)		// Did a write collision occur?
            {
                SSPCON1bits.WCOL = 0;       // clear WCOL bit
                junk = SSPBUF;              // clear SSPBUF
            }
            SSPCON1bits.CKP = 1;    		// release CLK
        }
        // clear interrupt
        PIR1bits.SSP1IF = 0;
    }
    
    return rv;
}

int main(void){
    uint8_t command, retval;
#ifdef _TEST_
    uint8_t counter, i;
#endif /* _TEST_ */

    /* Init
     * 1. Setup clock
     * 2. Setup I2C slave
     * 3. Setup 4 outputs
     */
    init_clk();
    init_io();
    init_i2c();

    /* Main loop.
     * Go to sleep immediately.
     * If an interrupt is generated. The CPU will wake up from sleep and receive
     * I2C data and change its outputs as requested
     */
    __delay_ms(10);

#ifdef _TEST_
    while(1){
        for(counter=0;counter<16;counter++){
            LATA = LAT_lookup[counter];
            for(i=0;i<20;i++) __delay_ms(100);
        }
    }
#endif /* _TEST_ */

    while(1){
        SLEEP(); // this will reduce power consumption to around 20 nA @ 2 V (if you don't do this, current drawn will be around 200 µA)
        retval = i2c_receive(&command);
        if(retval == 1){
            LATA = LAT_lookup[command & 0x0F];
        }
    }

    return 0;
}
