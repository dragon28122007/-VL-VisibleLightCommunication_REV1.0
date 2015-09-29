/**
 *	Raise your ARM 2015 sample code http://raiseyourarm.com/
 *	Author: Pay it forward club
 *	http://www.payitforward.edu.vn
 *  version 0.0.1
 */

/**
 * @file	SystemConfig.c
 * @brief	System config
 */
#include "include.h"
#include "inc/hw_gpio.h"
#include "driverlib/systick.h"

//* Private function prototype ----------------------------------------------*/
static void SysTickIntHandle(void);
static void system_SystickConfig(uint32_t ui32_msInterval);
//* Private variables -------------------------------------------------------*/
static SYSTEM_STATE e_SystemState = SYSTEM_POWER_UP;
static uint32_t ms_Tickcount = 0;
static uint32_t systemClock = 80000000;
static uint32_t ui32Counter;
uint32_t u32_UsrSystemClockGet();

uint32_t u32_UsrSystemClockGet()
{
	return systemClock;
}
void Config_System(void)
{
	ROM_FPUEnable();
	ROM_FPULazyStackingEnable();
	// Config clock
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5| SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);  //80MHz
	system_SystickConfig(1);
}

void Config_Pins(void)
{
	ROM_SysCtlPeripheralEnable(GPIO_PORTF_BASE);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4);

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);			// Enable PORT-D peripheral
	GPIOPinUnlockGPIO(GPIO_PORTD_BASE, GPIO_PIN_7);     		// Configure PD7 as GPIO
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_7);		// Configure PD7 as output

	HWREG(GPIO_PORTD_BASE + GPIO_O_DEN) |= GPIO_PIN_7;  		// Enable digital driver

	ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4,
							GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD_WPU);
	ROM_GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_7,
							GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD_WPU);
}

static void system_SystickConfig(uint32_t ui32_msInterval)
{
	ROM_SysTickPeriodSet(ROM_SysCtlClockGet() * ui32_msInterval / 1000);
	SysTickIntRegister(&SysTickIntHandle);
	ROM_SysTickIntEnable();
	ROM_SysTickEnable();
}

static void SysTickIntHandle(void)
{
	ms_Tickcount++;
}

void system_Enable_BoostCircuit(bool Enable)
{
	if (Enable)
	{
		ROM_GPIOPinWrite(BOOST_ENABLE_PORT, BOOST_ENABLE_PIN, 0xff);
	}
	else
	{
		ROM_GPIOPinWrite(BOOST_ENABLE_PORT, BOOST_ENABLE_PIN, 0x00);
	}
}

void LED_Display_init(void)
{
	ROM_SysCtlPeripheralEnable(LED1_PERIPHERAL);
	ROM_SysCtlPeripheralEnable(LED2_PERIPHERAL);
	ROM_SysCtlPeripheralEnable(LED3_PERIPHERAL);
	ROM_GPIOPinTypeGPIOOutput(LED1_PORT, LED1_PIN);
	ROM_GPIOPinTypeGPIOOutput(LED2_PORT, LED2_PIN);
	ROM_GPIOPinTypeGPIOOutput(LED3_PORT, LED3_PIN);

	LED1_OFF();
	LED2_OFF();
	LED3_OFF();
}

SYSTEM_STATE system_GetState(void)
{
	return e_SystemState;
}

void system_SetState(SYSTEM_STATE SysState)
{
	e_SystemState = SysState;
}

void system_Process_System_State(void)
{
	char c;
	switch (system_GetState())
	{
		case SYSTEM_POWER_UP:
			LED1_ON();
			ROM_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_7, GPIO_PIN_7);
			break;
		case SYSTEM_RUN:
			LED1_OFF();
			LED2_ON();
			if (Long_Uart0_GetChar(&c) == 0)
			{
				Long_Uart2_PutChar(c);
			}
			if (Long_Uart2_GetChar(&c) == 0)
			{
				Long_Uart0_PutChar(c);
			}
			break;
		case SYSTEM_ERROR:
			IntMasterDisable();
			while (1)
			{
				LED1_ON();
				LED2_ON();
				LED3_ON();
				ROM_SysCtlDelay(ROM_SysCtlClockGet() / 3);
				LED1_OFF();
				LED2_OFF();
				LED3_OFF();
				ROM_SysCtlDelay(ROM_SysCtlClockGet() / 3);
			}
	}
}
