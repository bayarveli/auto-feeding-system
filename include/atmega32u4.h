/*
 * atmega32u4.h
 *
 *  Created on: Dec 22, 2019
 *      Author: veli-
 */

#ifndef INCLUDE_ATMEGA32U4_H_
#define INCLUDE_ATMEGA32U4_H_

// userconfig.h gibi bir dosyaya taşınacak.
#define CLOCK_FREQ 			(16000000UL)

#define GPIOB_BASE			(0x23)
#define GPIOC_BASE			(0x26)
#define GPIOD_BASE			(0x29)
#define GPIOE_BASE			(0x2C)
#define GPIOF_BASE			(0x2F)

#define USART1_BASE			(0xC8)

#define TIMERINTFLAG_BASE 	(0x15)
#define TIMERINTMASK_BASE 	(0x6E)
#define TIMER0_BASE			(0x44)
#define TIMER1_BASE			(0x80)
#define TIMER3_BASE			(0x90)
#define TIMER4A_BASE		(0xBE)
#define TIMER4B_BASE		(0xCF)

#define I2C_BASE			(0xB8)

/*------------- General Purpose Input/Output (GPIO) --------------------------*/
typedef struct
{
	uint8_t sPIN;
	uint8_t sDDR;
	uint8_t sPORT;
} GpioTypeDef;

/*------------- USART --------------------------*/
typedef struct
{
	uint8_t sUCSRA;
	uint8_t sUCSRB;
	uint8_t sUCSRC;
	uint8_t sUCSRD;
	uint8_t sUBRRL;
	uint8_t sUBRRH;
	uint8_t sUDR;
} UsartTypeDef;

/*------------- TIMERS --------------------------*/

typedef struct
{
	uint8_t sTIFR0;
	uint8_t sTIFR1;
	uint8_t Reserved;
	uint8_t sTIFR3;
	uint8_t sTIFR4;
} TimerIntFlagTypeDef;

typedef struct
{
	uint8_t sTIMSK0;
	uint8_t sTIMSK1;
	uint8_t Reserved;
	uint8_t sTIMSK3;
	uint8_t sTIMSK4;
} TimerIntMaskTypeDef;

typedef struct
{
	uint8_t sTCCR0A;
	uint8_t sTCCR0B;
	uint8_t sTCNT0;
	uint8_t sOCR0A;
	uint8_t sOCR0B;
} Timer0TypeDef;

typedef struct
{
	uint8_t sTCCR13A;
	uint8_t sTCCR13B;
	uint8_t sTCCR13C;
	uint8_t Reserved;
	uint8_t sTCNT13L;
	uint8_t sTCNT13H;
	uint8_t sICR13L;
	uint8_t sICR13H;
	uint8_t sOCR13AL;
	uint8_t sOCR13AH;
	uint8_t sOCR13BL;
	uint8_t sOCR13BH;
	uint8_t sOCR13CL;
	uint8_t sOCR13CH;
} Timer13TypeDef;


typedef struct
{
	uint8_t sTCNT4;
	uint8_t sTC4H;
	uint8_t sTCCR4A;
	uint8_t sTCCR4B;
	uint8_t sTCCR4C;
	uint8_t sTCCR4D;
	uint8_t sTCCR4E;
} Timer4aTypeDef;

typedef struct
{
	uint8_t sOCR4A;
	uint8_t sOCR4B;
	uint8_t sOCR4C;
	uint8_t sOCR4D;
	uint8_t Reserved;
	uint8_t sDT4;
} Timer4bTypeDef;

typedef struct
{
	uint8_t sTWBR;
	uint8_t sTWSR;
	uint8_t sTWAR;
	uint8_t sTWDR;
	uint8_t sTWCR;
	uint8_t sTWAMR;
} I2CTypeDef;

#define GPIOB	((GpioTypeDef *) GPIOB_BASE)
#define GPIOC	((GpioTypeDef *) GPIOC_BASE)
#define GPIOD	((GpioTypeDef *) GPIOD_BASE)
#define GPIOE	((GpioTypeDef *) GPIOE_BASE)
#define GPIOF	((GpioTypeDef *) GPIOF_BASE)

#define USART1	((UsartTypeDef *) USART1_BASE)

#define TIMERINTFLAG 	((TimerIntFlagTypeDef *) TIMERINTFLAG_BASE)
#define TIMERINTMASK 	((TimerIntMaskTypeDef *) TIMERINTMASK_BASE)
#define TIMER0			((Timer0TypeDef *) TIMER0_BASE)
#define TIMER1			((Timer13TypeDef *) TIMER1_BASE)
#define TIMER3			((Timer13TypeDef *) TIMER3_BASE)
#define TIMER4A			((Timer4aTypeDef *) TIMER4A_BASE)
#define TIMER4B			((Timer4bTypeDef *) TIMER4B_BASE)

#define I2C				((I2CTypeDef *) I2C_BASE)

#endif /* INCLUDE_ATMEGA32U4_H_ */
