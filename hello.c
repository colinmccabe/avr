#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
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

  // Wait until TX is complete before going to sleep.
  // Glitches stop around 5ms. Double this.
  _delay_ms(10);
}

void timer_begin(void)
{
  TCCR1A = 0x00; // Normal counter mode
  OCR1A = 15625; // OC to 1 second's worth of cycles
  TIMSK |= _BV(OCIE1A); // Enable OC interrupt
  TCCR1B |= _BV(CS11) | _BV(CS10); // Start timer at F_CPU/64 (15625 Hz)
}

ISR(TIMER1_COMPA_vect)
{
  TCNT1 = 0;
  sleep_disable();
}

int main(void)
{
  DDRD = _BV(6);
  serial_begin();
  timer_begin();
  sei();

  while(1)
  {
    PORTD ^= _BV(6);
    println("hello");
    sleep_mode();
  }

  return 0;
}
