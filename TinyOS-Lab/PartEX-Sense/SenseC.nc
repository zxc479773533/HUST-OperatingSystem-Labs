/*
 * HUST IOT TinyOS Lab Part EX - Sense
 *
 * By Pan Yue, modifyed from tinyos/apps/test/TestSerialC.nc
 */

#include "Timer.h"
#include "SensirionSht11.h"

module SenseC
{
  uses {
    interface Boot;
    interface Leds;
    interface Timer<TMilli>;
    interface Read<uint16_t> as readTemp;
    interface Read<uint16_t> as readHumidity;
    interface Read<uint16_t> as readPhoto;
    interface Packet;
    interface AMPacket;
    interface AMSend;
    interface SplitControl as AMControl;
  }
}

implementation
{
  // the message struct
  message_t sense_packet;
  bool busy = FALSE;

  // Sampling frequency in binary milliseconds
  #define SAMPLING_FREQUENCY 100

  // Datas
  uint16_t TempData;
  uint16_t HumidityData;
  uint16_t PhotoData;

  event void Boot.booted() {
    call AMControl.start();
  }

  event void AMControl.startDone(error_t err) {
    if (err == SUCCESS) {
      call Timer.startPeriodic(SAMPLING_FREQUENCY);
    }
    else {
      call AMControl.start();
    }
  }

  event void AMControl.stopDone(error_t err) {
  }

  event void Timer.fired()
  {	
    call readTemp.read(); // Read temporary
    call readHumidity.read();  // Read humidity
    call readPhoto.read();  // Read photovoltaic
  }

  // Temporary handler
  event void readTemp.readDone(error_t result, uint16_t val) 
  {
    if (!busy && result == SUCCESS) {
      SenseMsg *payload = (SenseMsg*) call Packet.getPayload(&sense_packet, sizeof(SenseMsg));
      if (payload == NULL) {
        return;
      }
      payload->nodeid = TOS_NODE_ID;
      payload->kind = TEMPORARY;
      payload->data = val;
      if (call AMSend.send(AM_BROADCAST_ADDR, &sense_packet, sizeof(SenseMsg)) == SUCCESS) {
        call Leds.led0Toggle();
        busy == TRUE;
      }
    }
  }

  // Humidity handler
  event void readHumidity.readDone(error_t result, uint16_t val) 
  {
    if (!busy && result == SUCCESS) {
      SenseMsg *payload = (SenseMsg*) call Packet.getPayload(&sense_packet, sizeof(SenseMsg));
      if (payload == NULL) {
        return;
      }
      payload->nodeid = TOS_NODE_ID;
      payload->kind = HUMIDITY;
      payload->data = val;
      if (call AMSend.send(AM_BROADCAST_ADDR, &sense_packet, sizeof(SenseMsg)) == SUCCESS) {
        call Leds.led1Toggle();
        busy == TRUE;
      }
    }
  }

  // Photovoltaic handler  
  event void readPhoto.readDone(error_t result, uint16_t val) 
  {
    if (!busy && result == SUCCESS) {
      SenseMsg *payload = (SenseMsg*)call Packet.getPayload(&sense_packet, sizeof(SenseMsg));
      if (payload == NULL) {
        return;
      }
      payload->nodeid = TOS_NODE_ID;
      payload->kind = PHOTOVOLTAIC;
      payload->data = val;
      if (call AMSend.send(AM_BROADCAST_ADDR, &sense_packet, sizeof(SenseMsg)) == SUCCESS) {
        call Leds.led2Toggle();
        busy == TRUE;
      }
    }
  }

  event void AMSend.sendDone(message_t* msg, error_t err) {
    if (&sense_packet == msg) {
      busy = FALSE;
    }
  }
}
