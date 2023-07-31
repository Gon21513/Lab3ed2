/* 
 * File:   prelab3.c
 * Author: Luis Pedro Gonzalez 21513
 *
 * Created on 30 de julio de 2023, 04:09 PM
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
#include <stdio.h>
#include "SPI.h"
#include "LCD.h"


char pot1;
char buffer[4]; // Variable para almacenar la cadena de caracteres del valor del ADC

//*****************************************************************************
// Definici?n de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
//*****************************************************************************
// Definici?n de funciones para que se puedan colocar despu?s del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);

//*****************************************************************************
// C?digo Principal
//*****************************************************************************
void main(void) {
    setup();
    Lcd_Init(); //Iniciar pantalla LCD
    
    
    //confgurar posicion del cursor
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("ADC:");
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
       PORTCbits.RC2 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(0); // envio al slave
       pot1 = spiRead(); //leo lo que recibo sel slave
       //PORTA = pot1; //escribir el valor recibido en porb
      // PORTA = spiRead();
       
       __delay_ms(1);
       PORTCbits.RC2 = 1;       //Slave Deselect 
       
      // __delay_ms(250);
       //PORTA = pot1; //escribir el valor recibido en porb
    //mostrar valor del adc en el lcd 
       
  // mostar en lcd el voltaje 1 
        Lcd_Set_Cursor(2,2);
        sprintf(buffer, "%3u", pot1);
        Lcd_Write_String(buffer);
    
    }
    return;
}
//*****************************************************************************
// Funci?n de Inicializaci?n
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISCbits.TRISC2 = 0;
    //TRISA = 0;
    TRISB = 0;
    TRISD = 0;
    
    //PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    
    
    PORTCbits.RC2 = 1;
    
    
        
// --------------- Oscilador --------------- 
    OSCCONbits.IRCF = 0b111; // 4 MHz
    OSCCONbits.SCS = 1; // Seleccionar oscilador interno
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

}