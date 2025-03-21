
#pragma config FOSC = HSMP      // Oscillator Selection bits (HS oscillator (medium power 4-16 MHz))
#pragma config PLLCFG = ON      // 4X PLL Enable (Oscillator multiplied by 4)
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (Watch dog timer is always disabled. SWDTEN has no effect.)
#define _XTAL_FREQ 32000000

#include <xc.h>
// Tlacitko BUT3 bude fungovat jako prepinac jednotlivych rezimu (viz ulohy 2) - 4)) ZATIAL NEDOKONCENE XD
#define LED1    LATDbits.LATD2
#define LED2    LATDbits.LATD3
#define LED3    LATCbits.LATC4
#define LED4    LATDbits.LATD4
#define LED5    LATDbits.LATD5
#define LED6    LATDbits.LATD6

#define  DELAY1 __delay_ms(200)
#define  DELAY2 __delay_ms(400)
#define  DELAY3 __delay_ms(900)

volatile int i = 0;
volatile int j = 0;

void led();
void Change_speed();
void Pause_Resume();
void switch_state();

void main(void) {
    
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
    
    
    while(1) {
        
        Change_speed();
        Pause_Resume();
        
         if(PORTAbits.RA3){
         __delay_ms(50);  
             switch_state();
            
        }
    }
    
}
void led(){
           LED1 ^= 1; 
           LED2 ^= 1; 
           LED3 ^= 1; 
           LED4 ^= 1; 
           LED5 ^= 1; 
           LED6 ^= 1;
}
void Pause_Resume(){

 if(PORTAbits.RA4){
             __delay_ms(50);
            j = 1;
            while(j == 1){
                __delay_ms(50);
                if(PORTAbits.RA4){
                    __delay_ms(50);
                    j = 0;
                }
                
            }
        }

}
void Change_speed(){


if(i == 0){led();DELAY1;}
        else if(i == 1){led();DELAY2;}
        else {led();DELAY3;}  
        
        if (PORTCbits.RC0){
            
            __delay_ms(50);         
            while (PORTCbits.RC0);  
            __delay_ms(50);  
            i++;
            if (i == 3){i  = 0;}    
        }


}
void switch_state(){


