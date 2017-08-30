#include <xc.h>
#include "util.h"
#include "global.h"

void delay500ms(){
    uint8_t i;
    for(i=0;i<50;i++){
        __delay_ms(10);
    }
}

void test_end(){
    while(1){
        __delay_ms(10);
        LATA = 0xff;
        //LATAbits.LATA5=1;
        __delay_ms(10);
        //LATAbits.LATA5=0;
        LATA = 0x00;
    }
}

void test_pause(){
    uint8_t i;
    for(i=0;i<100;i++){
        __delay_ms(10);
        LATA = 0xff;
        //LATAbits.LATA5=1;
        __delay_ms(10);
        //LATAbits.LATA5=0;
        LATA = 0x00;
    }
}
