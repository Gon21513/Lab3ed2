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
char pot2;
char contador;

char buffer[20]; // Variable para almacenar la cadena de caracteres del valor del ADC
char voltaje1[10];//buffer para guardar los valore de la lcd del pot1
char voltaje2[10];//buffer para guardar los valore de la lcd del pot2
char conta[20];//buffer para guardar los valore de la lcd del contador 


int volt1;
int volt2;

unsigned int unidad1; //pot1
unsigned int decima1; //pot1
unsigned int centesima1; //pot1
unsigned int unidad2; //pot2
unsigned int decima2; //pot2
unsigned int centesima2; //pot2

//*****************************************************************************
// Definici?n de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
//*****************************************************************************
// Definici?n de funciones para que se puedan colocar despu?s del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);

//funcion para le mapeo adc a voltahje 

int map(unsigned char value, int inputmin, int inputmax, int outmin, int outmax){ //función para mapear valores
    return ((value - inputmin)*(outmax-outmin)) / (inputmax-inputmin)+outmin;}

//*****************************************************************************
// C?digo Principal
//*****************************************************************************
void main(void) {
    setup();
    Lcd_Init(); //Iniciar pantalla LCD
    
    
    //confgurar posicion del cursor
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("POT1: POT2: CONT:");
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        
        //----------slave1
       PORTCbits.RC2 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(1); // envio al slave
       pot1 = spiRead(); //leo lo que recibo sel slave
       __delay_ms(1);
      
//       __delay_ms(1);
       PORTCbits.RC2 = 1;       //Slave Deselect 
      // __delay_ms(1);
//       //vuelvo a activar el slave 1 pero con el contador

//slave 1 otra vez
       //       
        PORTCbits.RC2 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(3); // envio al slave1 el id del contador 
       contador = spiRead(); //leo lo que recibo sel slave

       
       __delay_ms(1);
       PORTCbits.RC2 = 1;       //Slave Deselect 
       
       
       //--------------slave2
       
       PORTCbits.RC1 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(2); // envio al slave
       pot2 = spiRead(); //leo lo que recibo sel slave2

  
       __delay_ms(1);
       PORTCbits.RC1 = 1;       //Slave Deselect 
       
 
       

        __delay_ms(10);
        
        
        //mostrar en lcd el valor del primer pot
        volt1 = map(pot1, 0, 255, 0, 100); //mapear valor del voltaje de 0 a 100
        unidad1 = (volt1*5)/100; //Separar las unidades del valor del voltaje
        decima1 = ((volt1*5)/10)%10; //Separar las decimas del valor del voltaje
        centesima1 = (volt1*5)%10; //Separar las centesimas del valor del voltaje
        Lcd_Set_Cursor(2,1); //Cursor en (1,7)
        sprintf(buffer, "%u.%u%u" , unidad1 , decima1 , centesima1 ); //convertir variable a una cadena de caracteres
        Lcd_Write_String(buffer); //Mostrar cadena de caracteres en pantalla
        
        
        //mostrar en el lcd el valor del segundo pot
        volt2 = map(pot2, 0, 255, 0, 100); //mapear valor del voltaje de 0 a 100
        unidad2 = (volt2*5)/100; //Separar las unidades del valor del voltaje
        decima2 = ((volt2*5)/10)%10; //Separar las decimas del valor del voltaje
        centesima2 = (volt2*5)%10; //Separar las centesimas del valor del voltaje
        Lcd_Set_Cursor(2,7); //Cursor en (1,7)
        sprintf(buffer, "%u.%u%u" , unidad2 , decima2 , centesima2 ); //convertir variable a una cadena de caracteres
        Lcd_Write_String(buffer); //Mostrar cadena de caracteres en pantalla
        
        
        Lcd_Set_Cursor(2,14); //Cursor en (1,7)
        sprintf(conta, "%03u" , contador); //convertir variable a una cadena de caracteres
        Lcd_Write_String(conta); //Mostrar cadena de caracteres en pantalla
       // __delay_ms(150);
  

//  // mostar en lcd el valor del pot1
//        
//        Lcd_Set_Cursor(2,1);
//        sprintf(buffer, "%3u", pot1);
//        Lcd_Write_String(buffer);
//    
//  //mostar el valor del pot2
//         Lcd_Set_Cursor(2,7);
//        sprintf(buffer, "%3u", pot2);
//        Lcd_Write_String(buffer);
    
    }
    return;
}
//*****************************************************************************
// Funci?n de Inicializaci?n
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    
    //salidas de los esclavos
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC1 = 0;

    //TRISA = 0;
    TRISB = 0;
    TRISD = 0;
    
    //PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    
    
    //apagar pueros de esclavos(ESTA INVERTIDA ))
    PORTCbits.RC2 = 1;
    PORTCbits.RC1 = 1;

    
    
        
// --------------- Oscilador --------------- 
    OSCCONbits.IRCF = 0b111; // 8 MHz
    OSCCONbits.SCS = 1; // Seleccionar oscilador interno
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

}