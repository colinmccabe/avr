#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <string.h>
#include <util/delay.h>

#define UART_DIV 12  // 4800 baud

void serial_begin(void)
{
  UBRRL = UART_DIV;
  UCSRB |= _BV(TXEN);
}

void sendchar(char c)
{
  loop_until_bit_is_set(UCSRA, UDRE);
  UDR = c;
}

void println(char* s)
{
  size_t len = strlen(s);

  for (size_t i = 0; i < len; i++) {
    sendchar(s[i]);
  }

  sendchar('\r');
  sendchar('\n');
}

int main(void)
{
  serial_begin();
  DDRD = _BV(6);

  while(1)
  {
    PORTD |= _BV(6);
    _delay_ms(500);
    PORTD &= ~_BV(6);
    _delay_ms(500);

    println("hello");
  }

  return 0;
}
