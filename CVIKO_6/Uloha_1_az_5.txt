/// uart
#pragma config FOSC = HSMP      // Externi oscilator
#pragma config PLLCFG = ON      // 4X PLL 
#pragma config FCMEN = ON       // Fail-Safe Clock 
#pragma config WDTEN = OFF      // Watchdog Timer OFF
#include <stdio.h>
#include <xc.h>   
#include <stdint.h>
          

#define _XTAL_FREQ 32000000

void UART_init();
void putch(char data);
int UART_GetChar();             
void UART_PutChar(int c);       
int UART_CharAvailable();      
void UART_PutStr(char * str);   
void recieve_send();
volatile int msg_full = 0;
volatile char msg[32];
volatile  char temp;
volatile int znak;
char str[5] = "AHOJ";
volatile int j;
volatile int itr = 0; 

void UART_init(){
    ANSELC = 0x00;          // vypnuti analogovych funkci na 
    TRISD = 0x00;           // PORTD jako vystup
    TRISCbits.TRISC6 = 1;   // TX pin jako vstup
    TRISCbits.TRISC7 = 1;   // rx pin jako vstup
   
    /*baudrate*/
    SPBRG1 = 51;              
    
    TXSTA1bits.SYNC = 0;      // nastaveni asynchroniho modu
    RCSTA1bits.SPEN = 1;      // zapnuti UART
    TXSTA1bits.TXEN = 1;      // zapnuti TX
    RCSTA1bits.CREN = 1;      // zapnuti RX 
   
}
int main(void) {
    UART_init();
    UART_PutStr("UART Test");
    while(1){
        
        //UART_PutChar(UART_GetChar());
        recieve_send();
         //UART_PutStr(&str);
    }
}

void putch(char data){
    while(!TX1IF);
    TXREG1 = data;
}

int UART_GetChar(){
    
    while(!RC1IF);
    return RCREG1;

}             
void UART_PutChar(int c){
    while(!TX1IF);
        TXREG1 = c;
    
}      
int UART_CharAvailable(){
    if(RC1IF){
        return 1;
    }
    else{
        return 0;
    }
}      
void UART_PutStr(char * str){

    int i = 0;
    while(str[i] != '\0'){
        putch(str[i]);
        i++;
       
        
    }
}  
void recieve_send(){
     
// Check if character is available
    while(UART_CharAvailable()) {
        // Read the character
        temp = RCREG1;
        
        // Check for terminal condition (period)
        if(temp == '.') {
            msg_full = 1;
        }
        // Check for buffer overflow
        else if(itr >= 31) { // 32-1 to leave room for null terminator
            msg_full = 1;
            msg[itr] = temp; // Store the last character
        }
        // Store the character
        else {
            msg[itr] = temp;
            itr++;
        }
        
        // If message is complete, send it back in reverse
        if(msg_full) {
            // Add null terminator
            msg[itr] = '\0';
            
            // Send characters in reverse order
            int i;
            for(i = itr - 1; i >= 0; i--) {
                UART_PutChar(msg[i]);
            }
            
            // Reset message state
            msg_full = 0;
            itr = 0;
        }
    }
}