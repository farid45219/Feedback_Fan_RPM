
//******************************************//
//AVR Frequency Measurement Unit            //
//Created: 10 Nov, 2024                     //
//Author: Md. Faridul Islam (45219)         //
//EED, Refrigeratror R&I, Walton            //
//******************************************//


#include  <avr/io.h>
#include  <avr/interrupt.h>
#include  "freq.h"


#define   FREQ_MES_DDR        DDRD
#define   FREQ_MES_PORT       PORTD
#define   FREQ_MES_PIN        PIN
#define   FREQ_MES_BP         0U
#define   FREQ_MES_SMPL       32U
#define   FREQ_MES_AVG_RSHIFT 5U

typedef struct freq_mes_t{
  volatile uint8_t  TimerStatus;
  volatile uint16_t RawTimerValIndex;
  volatile uint16_t RawTimerVal[FREQ_MES_SMPL];
  volatile uint32_t RawTimerAccVal;
  volatile uint16_t AvgTimerVal;
  volatile uint32_t TimerAccValShadow;
  volatile uint16_t AvgTimerValShadow;
  volatile uint32_t TimerTickUnitUs;
}freq_mes_t;


enum{
  TIMER_STOPPED = 0,
  TIMER_STARTED = 1
};


freq_mes_t FreqMes;



void FreqMes_Struct_Init(void){
  FreqMes.TimerStatus = TIMER_STOPPED;
  FreqMes.RawTimerValIndex = 0;
  for(uint8_t i=0; i<FREQ_MES_SMPL; i++){
    FreqMes.RawTimerVal[i] = 0;
  }
  FreqMes.RawTimerAccVal = 0;
  FreqMes.AvgTimerVal = 0;
  FreqMes.TimerAccValShadow = 1000000;
  FreqMes.AvgTimerValShadow = 0;
  FreqMes.TimerTickUnitUs = F_CPU;
  FreqMes.TimerTickUnitUs /= 1024;
  FreqMes.TimerAccValShadow /= FreqMes.TimerTickUnitUs;
  FreqMes.TimerTickUnitUs = FreqMes.TimerAccValShadow;
  FreqMes.TimerAccValShadow = 0;
}

void FreqMes_Clear_Buf(void){
  for(uint8_t i=0; i<FREQ_MES_SMPL; i++){
    FreqMes.RawTimerVal[i] = 0;
  }
  FreqMes.RawTimerValIndex = 0;
}

void FreqMes_Clear_Calculation(void){
  FreqMes.TimerAccValShadow = FreqMes.RawTimerAccVal;
  FreqMes.AvgTimerValShadow = FreqMes.AvgTimerVal;
  FreqMes.RawTimerValIndex = 0;
  FreqMes.RawTimerAccVal = 0;
  FreqMes.AvgTimerVal = 0;
}

void FreqMes_GPIO_Init(void){
  FREQ_MES_DDR  &=~ (1<<FREQ_MES_BP);
  FREQ_MES_PORT |=  (1<<FREQ_MES_BP);
}


void FreqMes_Timer_Init(void){
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1C = 0;
  TIMSK1 = 0;
  TIFR1  = 0xFF;
  TIMSK1 = (1<<TOIE1);
  sei();
}

void FreqMes_Ext_Int_Init(void){
  EIMSK &=~(1<<INT0);
  EICRA |= (1<<ISC00)|(1<<ISC01);
  EIMSK |= (1<<INT0);
  sei();
}

void FreqMes_Timer_Start(void){
  if(TIFR1 != 0){
    TIFR1  = 0xFF;
  }
  TCCR1B = (1<<CS10)|(1<<CS12);
  TCNT1  = 0;
  FreqMes.TimerStatus = TIMER_STARTED;
}

void FreqMes_Timer_Stop(void){
  TCCR1B = 0;
  TCNT1  = 0;
  if(TIFR1 != 0){
    TIFR1  = 0xFF;
  }
  FreqMes.TimerStatus = TIMER_STOPPED;
}

uint16_t FreqMes_Timer_Get_Val(void){
  uint16_t val = 0;
  cli();
  val = TCNT1;
  sei();
  return val;
}


uint8_t FreqMes_Timer_Get_TimerStatus(void){
  uint8_t temp = 0;
  cli();
  temp = FreqMes.TimerStatus;
  sei();
  return temp;
}

uint16_t FreqMes_Timer_Get_RawTimerValIndex(void){
  uint16_t temp = 0;
  cli();
  temp = FreqMes.RawTimerValIndex;
  sei();
  return temp;
}

uint16_t FreqMes_Timer_Get_RawTimerVal(uint8_t index){
  uint16_t temp = 0;
  cli();
  temp = FreqMes.RawTimerVal[index];
  sei();
  return temp;
}

uint32_t FreqMes_Timer_Get_RawTimerAccVal(void){
  uint32_t temp = 0;
  cli();
  temp = FreqMes.RawTimerAccVal;
  sei();
  return temp;
}

uint16_t FreqMes_Timer_Get_AvgTimerVal(void){
  uint16_t temp = 0;
  cli();
  temp = FreqMes.AvgTimerVal;
  sei();
  return temp;
}

uint32_t FreqMes_Timer_Get_TimerAccValShadow(void){
  uint16_t temp = 0;
  cli();
  temp = FreqMes.TimerAccValShadow;
  sei();
  return temp;
}

uint16_t FreqMes_Timer_Get_AvgTimerValShadow(void){
  uint16_t temp = 0;
  cli();
  temp = FreqMes.AvgTimerValShadow;
  sei();
  return temp;
}

uint32_t FreqMes_Timer_Get_TimerTickUnitUs(void){
  return FreqMes.TimerTickUnitUs;
}




void FreqMes_Init(void){
  FreqMes_Struct_Init();
  FreqMes_Clear_Buf();
  FreqMes_Clear_Calculation();
  FreqMes_GPIO_Init();
  FreqMes_Timer_Init();
  FreqMes_Ext_Int_Init();
}












ISR(TIMER1_OVF_vect){
  FreqMes_Timer_Stop();
}

ISR(INT0_vect){
  if(FreqMes.TimerStatus == TIMER_STARTED){
    FreqMes.RawTimerVal[FreqMes.RawTimerValIndex] = TCNT1;
    FreqMes.RawTimerValIndex++;
    FreqMes.RawTimerAccVal += FreqMes.RawTimerVal[FreqMes.RawTimerValIndex];
    if(FreqMes.RawTimerValIndex >= FREQ_MES_SMPL){
      FreqMes.RawTimerValIndex = 0;
      FreqMes.TimerAccValShadow = FreqMes.RawTimerAccVal;
      FreqMes.RawTimerAccVal >>= FREQ_MES_AVG_RSHIFT;
      FreqMes.AvgTimerVal = FreqMes.RawTimerAccVal;
      FreqMes.AvgTimerValShadow = FreqMes.AvgTimerVal;
      FreqMes.RawTimerAccVal = 0;
    }
    TCNT1  = 0;
    if(TIFR1 != 0){
      TIFR1  = 0xFF;
    }
  }
  else if(FreqMes.TimerStatus == TIMER_STOPPED){
    FreqMes_Timer_Start();
    FreqMes_Clear_Calculation();
  }
}