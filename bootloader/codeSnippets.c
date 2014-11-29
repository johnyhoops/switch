#define BAUD_RATE 19200
UBRRH = (((F_CPU/BAUD_RATE)/16)-1)>>8; 	// set baud rate
UBRRL = (((F_CPU/BAUD_RATE)/16)-1);
UCSRB = (1<<RXEN)|(1<<TXEN);  // enable Rx & Tx
UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);  // config USART; 8N1


#define outb(sfr,val)  (_SFR_BYTE(sfr) = (val))
#define inb(sfr) _SFR_BYTE(sfr)	
while (!(inb(UCSRA) & _BV(UDRE)));
outb(UDR, 65 + i - reg_map);
