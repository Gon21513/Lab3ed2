/*
 * File:   prelabed2slave.c
 * Author: lpgp3
 *
 * Created on 30 de julio de 2023, 04:50 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definici?n e importaci?n de librer?as
//*****************************************************************************
#include <xc.h>
#include <stdint.h>
#include "SPI.h"
#include "ADC.h"
#include <stdio.h>
#include <pic16f887.h>
#include<pic.h>

//*****************************************************************************
// Definici?n de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000

unsigned char temporal;
int numadc; //variable para el valor del adc
int ADC; //valor del adc 
int read1;
int read2;

//*****************************************************************************
// Definici?n de funciones para que se puedan colocar despu?s del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
//*****************************************************************************
// C?digo de Interrupci?n 
//*****************************************************************************
void __interrupt() isr(void){
    
//    // ---- INTERRUPCION DEL ADC --------
    if (PIR1bits.ADIF == 1){ // Chequear bandera del conversor ADC
        
        ADC = read_ADC();
        numadc = ADC; // pasar valor del adc a num1
    }
       PIR1bits.ADIF = 0; // Borrar el indicador del conversor ADC

    //Interrupcion del SPI
   if(SSPIF == 1){
        temporal = spiRead();  //puede ser tambien PORTD = SSBUF;//recibo del master 
        
        if (temporal == 0){
        spiWrite(numadc);  //ennvio al master el valor del adc
        }
        
        SSPIF = 0;
    }
}
//*****************************************************************************
// C?digo Principal
//*****************************************************************************
void main(void) {
    setup();
    adc_init(0);//CONFIGURAR EL CANAL 0 DEL adc 
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        


        //verifica la conversion adc
        if (ADCON0bits.GO == 0){
            ADCON0bits.GO = 1;
            __delay_ms(50);
        }
        //PORTD = numadc; //chequo del valor del ADC
    }
    return;
}
//*****************************************************************************
// Funci?n de Inicializaci?n
//*****************************************************************************
void setup(void){
    //ANSELbits.ANS0 = 1; //Canal AN0 como entrada analógica
    //ANSELbits.ANS4 = 0; //Pin A5 como pin digital

    ANSEL = 0;
    ANSELH = 0;
    
    ANSELbits.ANS0 = 1;
    TRISAbits.TRISA0 = 1;
    
    TRISB = 0;
    TRISD = 0;
    
    //PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    
// --------------- Oscilador --------------- 
    OSCCONbits.IRCF = 0b111; // 8 MHz
    OSCCONbits.SCS = 1; // Seleccionar oscilador interno
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupci?n MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupci?n MSSP
    TRISAbits.TRISA5 = 1;       // Slave Select
   
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
   
}