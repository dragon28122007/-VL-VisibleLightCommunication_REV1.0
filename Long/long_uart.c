/*
 * long_uart0.c
 *
 *  Created on: Sep 13, 2015
 *      Author: WIN
 */

#include "../include.h"
#include "ringbuf.h"

static RINGBUF long_Uart0_RxRingBuf;
static uint8_t long_Uart0_RxBuf[1024];

static RINGBUF long_Uart0_TxRingBuf;
static uint8_t long_Uart0_TxBuf[1024];

static RINGBUF long_Uart2_RxRingBuf;
static uint8_t long_Uart2_RxBuf[1024];

static RINGBUF long_Uart2_TxRingBuf;
static uint8_t long_Uart2_TxBuf[1024];

static void UART0_RxTxHandler(void);
static void UART2_RxTxHandler(void);

void Long_Uart0_Init(void)
{
	uint32_t ui32_SystemClock;
	RINGBUF_Init(&long_Uart0_TxRingBuf, long_Uart0_TxBuf, sizeof(long_Uart0_TxBuf));
	RINGBUF_Init(&long_Uart0_RxRingBuf, long_Uart0_RxBuf, sizeof(long_Uart0_RxBuf));
	ui32_SystemClock = u32_UsrSystemClockGet();
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	IntDisable(INT_UART0);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTConfigSetExpClk(UART0_BASE, ui32_SystemClock, 19200,
						(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
						UART_CONFIG_PAR_NONE));
	UARTIntRegister(UART0_BASE, &UART0_RxTxHandler);
	UARTIntClear(UART0_BASE, UART_INT_RX | UART_INT_TX);
	UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
	UARTTxIntModeSet(UART0_BASE, UART_TXINT_MODE_EOT);
	IntEnable(INT_UART0);
}

void Long_Uart2_Init(void)
{
	uint32_t ui32_SystemClock;
	RINGBUF_Init(&long_Uart2_TxRingBuf, long_Uart2_TxBuf, sizeof(long_Uart2_TxBuf));
	RINGBUF_Init(&long_Uart2_RxRingBuf, long_Uart2_RxBuf, sizeof(long_Uart2_RxBuf));
	ui32_SystemClock = u32_UsrSystemClockGet();
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinConfigure(GPIO_PD6_U2RX);
	GPIOPinConfigure(GPIO_PD7_U2TX);
	IntDisable(INT_UART2);
	GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
	UARTConfigSetExpClk(UART2_BASE, ui32_SystemClock, 19200,
						(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
						UART_CONFIG_PAR_NONE));
	UARTIntRegister(UART2_BASE, &UART2_RxTxHandler);
	UARTIntClear(UART2_BASE, UART_INT_RX | UART_INT_TX);
	UARTIntEnable(UART2_BASE, UART_INT_RX | UART_INT_RT);
	UARTTxIntModeSet(UART2_BASE, UART_TXINT_MODE_EOT);
	IntEnable(INT_UART2);
}

void Long_Uart0_PutChar(char c)
{
	uint32_t byteCnt;
	byteCnt = RINGBUF_GetFill(&long_Uart0_TxRingBuf);
	if (byteCnt)
	{
		RINGBUF_Put(&long_Uart0_TxRingBuf, c);
	}
	else
	{
		UARTCharPutNonBlocking(UART0_BASE, c);
		UARTIntEnable(UART0_BASE, UART_INT_TX);
	}
}

void Long_Uart2_PutChar(char c)
{
	uint32_t byteCnt;
	byteCnt = RINGBUF_GetFill(&long_Uart2_TxRingBuf);
	if (byteCnt)
	{
		RINGBUF_Put(&long_Uart2_TxRingBuf, c);
	}
	else
	{
		UARTCharPutNonBlocking(UART2_BASE, c);
		UARTIntEnable(UART2_BASE, UART_INT_TX);
	}
}

void Long_Uart0_PutString(char *s)
{
	while (*s)
	{
		Long_Uart0_PutChar(*s);
		s++;
	}
}

void Long_Uart2_PutString(char *s)
{
	while (*s)
	{
		Long_Uart2_PutChar(*s);
		s++;
	}
}


int32_t Long_Uart0_GetChar(char *c)
{
	return RINGBUF_Get(&long_Uart2_RxRingBuf, c);
}

int32_t Long_Uart2_GetChar(char *c)
{
	return RINGBUF_Get(&long_Uart0_RxRingBuf,c);
}

static void UART0_RxTxHandler(void)
{
	uint32_t IntStatus, byteCnt, timeout = 1000000;
	uint8_t c;
	IntStatus = UARTIntStatus(UART0_BASE, true);
	UARTIntClear(UART0_BASE, IntStatus);
	if (IntStatus & UART_INT_TX)
	{
		byteCnt = RINGBUF_GetFill(&long_Uart0_TxRingBuf);
		if (byteCnt)
		{
			RINGBUF_Get(&long_Uart0_TxRingBuf, &c);
			UARTCharPutNonBlocking(UART0_BASE, c);
			if (byteCnt == 1)
			{
				UARTIntDisable(UART0_BASE, UART_INT_TX);
			}
		}
		else
		{
			UARTIntDisable(UART0_BASE, UART_INT_TX);
		}
	}
	else if (IntStatus & (UART_INT_RX | UART_INT_RT))
	{
		while(!UARTCharsAvail(UART0_BASE) && (timeout--));
		c = UARTCharGet(UART0_BASE);
		RINGBUF_Put(&long_Uart2_RxRingBuf, c);
	}
	else
	{
		c = UARTCharGet(UART0_BASE);
	}
}

static void UART2_RxTxHandler(void)
{
	uint32_t IntStatus, byteCnt, timeout = 1000000;
	uint8_t c;
	IntStatus = UARTIntStatus(UART2_BASE, true);
	UARTIntClear(UART2_BASE, IntStatus);
	if(IntStatus & UART_INT_TX)
	{
		byteCnt = RINGBUF_GetFill(&long_Uart2_TxRingBuf);
		if (byteCnt)
		{
			RINGBUF_Get(&long_Uart2_TxRingBuf, &c);
			UARTCharPutNonBlocking(UART2_BASE, c);
			if (byteCnt == 1)
			{
				UARTIntDisable(UART2_BASE, UART_INT_TX);
			}
		}
		else
		{
			UARTIntDisable(UART2_BASE, UART_INT_TX);
		}
	}
	else if (IntStatus & (UART_INT_RX | UART_INT_RT))
	{
		while(!UARTCharsAvail(UART2_BASE) && (timeout--));
		c = UARTCharGet(UART2_BASE);
		RINGBUF_Put(&long_Uart0_RxRingBuf,c);
	}
	else
	{
		c = UARTCharGet(UART2_BASE);
	}
}
