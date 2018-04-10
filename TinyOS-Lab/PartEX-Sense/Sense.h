/*
 * HUST IOT TinyOS Lab Part EX - Sense
 *
 * By Pan Yue
 */

#ifndef SENSE_H
#define SENSE_H

#define TEMPORARY 0
#define HUMIDITY 1
#define PHOTOVOLTAIC 2

enum {
  AM_SENSEMSG = 6,
  TIMER_PERIOD_MILLI = 250
};

typedef nx_struct SenseMsg {
  nx_uint16_t nodeid;
  nx_uint16_t kind;
  nx_uint16_t data;
} SenseMsg;

#endif