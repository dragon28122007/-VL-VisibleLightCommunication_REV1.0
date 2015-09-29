/**
 *	Raise your ARM 2015 sample code http://raiseyourarm.com/
 *	Author: Pay it forward club
 *	http://www.payitforward.edu.vn
 *  version 0.0.1
 */

/**
 * @file	timer.c
 * @brief	timer event managment
 */
#include <stdint.h>
#include <stdbool.h>
#include "../include.h"
#include "Timer.h"

void TIMER_ISR(void);

void Timer_Init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4);

	ROM_TimerConfigure(TIMER4_BASE, TIMER_CFG_PERIODIC);
	ROM_TimerLoadSet(TIMER4_BASE, TIMER_A, ROM_SysCtlClockGet()/960000/2);

	// TimerIntRegister(TIMER4_BASE, TIMER_A, &TIMER_ISR);
	ROM_IntEnable(INT_TIMER4A);
	ROM_TimerIntEnable(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
	ROM_TimerIntClear(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
	ROM_TimerControlStall(TIMER4_BASE, TIMER_A, false);
	ROM_TimerEnable(TIMER4_BASE, TIMER_A);
}

void TIMER_ISR(void)
{
//	 TimerIntClear(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
	// TIMER4_BASE = 0x40034000;
	// TIMER_TIMA_TIMEOUT = 0x00000001;
	// TIMER_O_ICR = 0x00000024
//	ASSERT(_TimerBaseValid(0x40034000));
	HWREG(0x40034000 + 0x00000024) = 0x00000001;
	// GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4));
	// GPIO_PORTF_BASE = 0x40025000;
	// GPIO_PIN_4 = 0x00000010;
	// GPIO_O_DATA = 0x00000000;
//	ASSERT(_GPIOBaseValid(0x40025000));
	HWREG(0x40025000 + (0x00000000 + (0x00000010 << 2))) = ~HWREG(0x40025000 + (0x00000000 + (0x00000010 << 2)));
}
