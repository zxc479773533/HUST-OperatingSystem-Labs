/*
 * HUST IOT TinyOS Lab Part I - Binary 0 - 7
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
  // Add printf components
  components PrintfC;
  components SerialStartC;

  BlinkC -> MainC.Boot;
  BlinkC.Timer0 -> Timer0;
  BlinkC.Leds -> LedsC;
}

