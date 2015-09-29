/**
 *	Raise your ARM 2015 sample code http://raiseyourarm.com/
 *	Author: Pay it forward club
 *	http://www.payitforward.edu.vn
 *  version 0.0.1
 */

/**
 * @file	Button.c
 * @brief	Push button driver
 */

#include "../include.h"
#include "Button.h"

#define GPIO_PORT_BUTTON 		GPIO_PORTF_BASE
#define INT_BUTTON 				INT_GPIOF
#define SYSCTRL_PERIPH_BUTTON 	SYSCTL_PERIPH_GPIOF
#define GPIO_PIN_BUTTON_RIGHT 	GPIO_PIN_0
//#define GPIO_PIN_BUTTON_LEFT 	GPIO_PIN_5

//* Private function prototype ----------------------------------------------*/
static void ButtonsISR(void);

void GPIOPinUnlockGPIO(uint32_t ui32Port, uint8_t ui8Pins)
{
    HWREG(ui32Port + GPIO_O_LOCK) = GPIO_LOCK_KEY;      // Unlock the port
    HWREG(ui32Port + GPIO_O_CR) |= ui8Pins;             // Commit specified pins for next access
    HWREG(ui32Port + GPIO_O_AFSEL) &= ~ui8Pins;         // Function: GPIO (disable alternate function)
    HWREG(ui32Port + GPIO_O_DEN) &= ~ui8Pins;           // Disable digital driver
    HWREG(ui32Port + GPIO_O_LOCK) = 0;                  // Lock the port
}

void Button_init(void)
{

	ROM_SysCtlPeripheralEnable(SYSCTRL_PERIPH_BUTTON);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4);
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR)  = GPIO_PIN_BUTTON_RIGHT;
    ROM_GPIOPinTypeGPIOInput(GPIO_PORT_BUTTON, GPIO_PIN_BUTTON_RIGHT);
    ROM_GPIOPadConfigSet(GPIO_PORT_BUTTON, GPIO_PIN_BUTTON_RIGHT, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD_WPU);

    ROM_GPIOIntTypeSet(GPIO_PORT_BUTTON, GPIO_PIN_BUTTON_RIGHT, GPIO_FALLING_EDGE);

	GPIOIntRegister(GPIO_PORT_BUTTON, &ButtonsISR);
	ROM_IntEnable(INT_BUTTON);
	GPIOIntEnable(GPIO_PORT_BUTTON, GPIO_PIN_BUTTON_RIGHT);
	GPIOIntClear(GPIO_PORT_BUTTON, GPIO_PIN_BUTTON_RIGHT);
}

void ButtonsISR(void)
{
	uint32_t ui32_IntStatus;
	ui32_IntStatus = GPIOIntStatus(GPIO_PORT_BUTTON, true);
	GPIOIntClear(GPIO_PORT_BUTTON, ui32_IntStatus);

	if (ui32_IntStatus & GPIO_PIN_BUTTON_RIGHT)
	{
		switch (system_GetState())
		{
		case SYSTEM_POWER_UP:
			system_SetState(SYSTEM_RUN);
			Long_Uart0_Init();
			Long_Uart2_Init();
			Timer_Init();
			break;
		case SYSTEM_RUN:
			system_SetState(SYSTEM_ERROR);
			break;
		case SYSTEM_ERROR:
			system_SetState(SYSTEM_POWER_UP);
			Config_Pins();
		}
	}
}
