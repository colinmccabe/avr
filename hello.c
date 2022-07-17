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

void sleep_init(void)
{
  // Enable watchdog, interrupt, 0.5sec overflow interval
  WDTCR |= _BV(WDIE) | _BV(WDE) | _BV(WDP2) | _BV(WDP0);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

ISR(WDT_OVERFLOW_vect)
{
  sleep_disable();
}

int main(void)
{
  DDRD = _BV(6);
  serial_begin();
  sleep_init();
  sei();

  while (1)
  {
    PORTD ^= _BV(6);
    println("hello");
    sleep_mode();
  }

  return 0;
}
