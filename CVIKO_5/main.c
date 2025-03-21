// REV INTERRUPT
#pragma config FOSC = HSMP      // Oscillator Selection bits (HS oscillator (medium power 4-16 MHz))
#pragma config PLLCFG = ON      // 4X PLL Enable (Oscillator multiplied by 4)
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (Watch dog timer is always disabled. SWDTEN has no effect.)

#include <xc.h>

#define _XTAL_FREQ 32E6             
#define LED1 LATDbits.LATD2
#define LED2 LATDbits.LATD3
#define LED3 LATCbits.LATC4 
#define LED4 LATDbits.LATD4
#define LED5 LATDbits.LATD5
#define LED6 LATDbits.LATD6




#define BTTN1 PORTCbits.RC0
#define BTTN2 PORTAbits.RA4
#define BTTN3 PORTAbits.RA3
#define BTTN4 PORTAbits.RA2
//Vytvo?te obecnou obsluhu pomoci p?eru?ení pro v?echna 4 tla?ítka:
//pomoci jednoho ?asova?e (tj. pou?ijete jedno p?eru?ení) 
//periodicky detekujte stav na jednotlivých tla?ítkách, a provád?je debouncing. 
//V hlavním programu ovládejte celou sadu LED,
//zobrazujte na ní binární ?íslo n a reagujte na stisknutí jednotlivých tla?ítek následovn?:

//BUT1 ? inkrementuje n,
//BUT2 ? dekrementuje n,
//BUT3 ? bitové invertuje n,
//BUT4 ? nastaví n na 0
volatile unsigned char button1_pressed = 0;
volatile unsigned char button2_pressed = 0;
volatile unsigned char button3_pressed = 0;
volatile unsigned char button4_pressed = 0;

 volatile unsigned char  last_state_bttn1 = 1;
 volatile unsigned char  last_state_bttn2 = 1;
 volatile unsigned char  last_state_bttn3 = 1;
 volatile unsigned char  last_state_bttn4 = 1;
 volatile unsigned int value = 0;
 
void updateLEDs(unsigned int value);

void __interrupt() T1_ISR_HANDLER(void){
     
     if (TMR1IF && TMR1IE) {  
       
        
        if (BTTN1 == 0 && last_state_bttn1 == 1) {
            button1_pressed = 1; 
        }
        last_state_bttn1 = BTTN1;
    
        if (BTTN2 == 0 && last_state_bttn2 == 1) {
            button2_pressed = 1; 
        }
        last_state_bttn2 = BTTN2;

        if (BTTN3 == 0 && last_state_bttn3 == 1) {
            button3_pressed = 1; 
        }
        last_state_bttn3 = BTTN3;
       
        if (BTTN4 == 0 && last_state_bttn4 == 1) {
            button4_pressed = 1; 
        }
        last_state_bttn4 = BTTN4;
        TMR1IF = 0;  
    }
    
}

void init(void){
    
    TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD3 = 0;
    TRISDbits.TRISD4 = 0;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    TRISCbits.TRISC4 = 0;
    
    TRISCbits.TRISC0 = 1;
    TRISAbits.RA4 = 1; 
    ANSELAbits.ANSA3 = 0; 
    ANSELAbits.ANSA2 = 0;
    
    T1CONbits.TMR1CS = 0b00;        // zdroj casovace 1
    T1CONbits.T1CKPS = 0b11;        // nastaveni delicky  
    

    T5CONbits.TMR5CS = 0b00;        // zdroj casovace 5
    T5CONbits.T5CKPS = 0b11;        // nastaveni delicky  
    
    TMR1IE = 1;                     // povoleni preruseni pro TMR1
    TMR1IF = 0;                     // smazani priznaku (pro jistotu)
                                    // smazani priznaku (pro jistotu)
    
    PEIE = 1;                       // povoleni preruseni od periferii
    TMR1ON = 1;                     
    
    GIE = 1;                        // globalni povoleni preruseni
    
    LATD &= (0b10000011);
 
}

void main(void) {
    init();                       
    
    while(1){
           if (button1_pressed) {
               if (value < 0b111111){
               value = value++;}
               updateLEDs( value);
            button1_pressed = 0;
            }
           if (button2_pressed) {
                if (value > 0){
                value = value--;}
                updateLEDs( value);
            button2_pressed = 0;
            }
           if (button3_pressed) {
                value ^= 0b111111;
                updateLEDs( value);
            button3_pressed = 0;
           }
           if (button4_pressed) {
                value = 0;
                updateLEDs( value);
            button4_pressed = 0;
           }
       
    }
}
void updateLEDs(unsigned int value) {
 
    LATDbits.LATD6 = ~(value & 0x01);
    LATDbits.LATD5 = ~(value & 0x02) >> 1;
    LATDbits.LATD4 = ~(value & 0x04) >> 2;
    LATCbits.LATC4 = ~(value & 0x08) >> 3;
    LATDbits.LATD3 = ~(value & 0x10) >> 4;
    LATDbits.LATD2 = ~(value & 0x20) >> 5;
}