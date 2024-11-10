
//******************************************//
//AVR Frequency Measurement Unit            //
//Created: 10 Nov, 2024                     //
//Author: Md. Faridul Islam (45219)         //
//EED, Refrigeratror R&I, Walton            //
//******************************************//

#include "freq.h"

uint32_t pulse_per_sec = 0;

void setup() {
  FreqMes_Init();
  Serial.begin(38400);
}

void loop() {
  pulse_per_sec  = FreqMes_Timer_Get_AvgTimerValShadow();
  pulse_per_sec *= FreqMes_Timer_Get_TimerTickUnitUs();
  pulse_per_sec  = 1000000/pulse_per_sec;
  Serial.print("AvgTime ");
  Serial.println(FreqMes_Timer_Get_AvgTimerValShadow());
  Serial.print("PPS     ");
  Serial.println(pulse_per_sec);
  _delay_ms(100);
}
