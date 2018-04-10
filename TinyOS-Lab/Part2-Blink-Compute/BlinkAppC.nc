/*
 * HUST IOT TinyOS Lab Part II - Blink Compute
 *
 * By Pan Yue, modifyed from tinyos/apps/Blink/BlinkAppC.nc
 */

configuration BlinkAppC
{
}

implementation
{
  components MainC, BlinkC, LedsC;
  components new TimerMilliC() as Timer0;
  components new TimerMilliC() as Timer1;
  components new TimerMilliC() as Timer2;
  // Add printf components
  components PrintfC;
  components SerialStartC;

  BlinkC -> MainC.Boot;
  BlinkC.Timer0 -> Timer0;
  BlinkC.Timer1 -> Timer1;
  BlinkC.Timer2 -> Timer2;
  BlinkC.Leds -> LedsC;
}

