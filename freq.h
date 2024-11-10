
//******************************************//
//AVR Frequency Measurement Unit            //
//Created: 10 Nov, 2024                     //
//Author: Md. Faridul Islam (45219)         //
//EED, Refrigeratror R&I, Walton            //
//******************************************//

void     FreqMes_Struct_Init(void);
void     FreqMes_Clear_Buf(void);
void     FreqMes_Clear_Calculation(void);
void     FreqMes_GPIO_Init(void);
void     FreqMes_Timer_Init(void);
void     FreqMes_Ext_Int_Init(void);
void     FreqMes_Timer_Start(void);
void     FreqMes_Timer_Stop(void);
uint16_t FreqMes_Timer_Get_Val(void);
uint8_t  FreqMes_Timer_Get_TimerStatus(void);
uint16_t FreqMes_Timer_Get_RawTimerValIndex(void);
uint16_t FreqMes_Timer_Get_RawTimerVal(uint8_t index);
uint32_t FreqMes_Timer_Get_RawTimerAccVal(void);
uint16_t FreqMes_Timer_Get_AvgTimerVal(void);
uint32_t FreqMes_Timer_Get_TimerAccValShadow(void);
uint16_t FreqMes_Timer_Get_AvgTimerValShadow(void);
uint32_t FreqMes_Timer_Get_TimerTickUnitUs(void);

void     FreqMes_Init(void);
