
//******************************************//
//AVR Frequency Measurement Unit            //
//Created: 10 Nov, 2024                     //
//Author: Md. Faridul Islam (45219)         //
//EED, Refrigeratror R&I, Walton            //
//******************************************//


  //Connect Digital Pin 21 of Arduino Mega2560 with Feedback pin
  //Connect Appropiate buzzer pin


#include "freq.h"

uint32_t pulse_per_sec = 0;
uint32_t rps, rpm;

void setup() {
  FreqMes_Init();
  Serial.begin(38400);
  //Init buzzer gpio
}

void loop() {
  pulse_per_sec  = FreqMes_Timer_Get_AvgTimerValShadow();
  pulse_per_sec *= FreqMes_Timer_Get_TimerTickUnitUs();
  pulse_per_sec  = 1000000/pulse_per_sec;
  rps            = pulse_per_sec/2;
  rpm            = rps*60;
  Serial.print("AvgTime ");
  Serial.println(FreqMes_Timer_Get_AvgTimerValShadow());
  Serial.print("PPS     ");
  Serial.println(pulse_per_sec);
  Serial.print("RPS     ");
  Serial.println(rps);
  Serial.print("RPM     ");
  Serial.println(rpm);
  Serial.println();

  if(rpm < 1200){
    //buzzer alaram
  }

  _delay_ms(100);
}
