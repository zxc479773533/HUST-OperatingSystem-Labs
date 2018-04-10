/*
 * HUST IOT TinyOS Lab Part I - Binary 0 - 7
 *
 * By Pan Yue, modifyed from tinyos/apps/Blink/BlinkC.nc
 */
 
#include "Timer.h"
#include "printf.h"

module BlinkC @safe()
{
  uses interface Timer<TMilli> as Timer0;
  uses interface Leds;
  uses interface Boot;
}

implementation
{
  uint32_t count = 0;
  event void Boot.booted()
  {
    call Timer0.startPeriodic( 1000 );
  }

  event void Timer0.fired()
  {
    count++;
    printf("count: %d\n", count);
    dbg("BlinkC", "Timer 0 fired @ %s.\n", sim_time_string());
    if ((count & 0x01) == 0x01) {
      call Leds.led0Toggle();
    }
    else if ((count & 0x02) == 0x02) {
      call Leds.led0Toggle();
      call Leds.led1Toggle();
    }
    else if ((count & 0x04) == 0x04) {
      call Leds.led0Toggle();
      call Leds.led1Toggle();
      call Leds.led2Toggle();
    }
    else {
      call Leds.led0Off();
      call Leds.led1Off();
      call Leds.led2Off();
    }
    if (count == 8)
      count = 0;
  }
}

