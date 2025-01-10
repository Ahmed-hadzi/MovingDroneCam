// RCEncoder.H

#ifndef RCEncoder_h
#define RCEncoder_h

#include <Arduino.h>

#ifdef __cplusplus
extern "C"{
#endif


#define ledTest1 8
#define ledTest2 9

#define NBR_OF_CHANNELS  2
#define MIN_CHANNEL_PULSE 965  // around 1ms (manual calibration)
#define MID_CHANNEL_PULSE 1500  // 1.5ms
#define MAX_CHANNEL_PULSE 2000  // 2 ms
#define INTER_CHAN_DELAY  20   // 20 microseconds 
#define FRAME_RATE        40000 // 40 ms 
#define SYNC_PULSE_WIDTH (FRAME_RATE - (NBR_OF_CHANNELS * (MID_CHANNEL_PULSE + INTER_CHAN_DELAY))) 
#define MS_TO_TICKS(_ms)  ((_ms)*2)  // use macro here

void encoderBegin(byte pin);
void encoderWrite(byte channel, int microseconds);

#ifdef __cplusplus
} // extern "C"
#endif

#endif