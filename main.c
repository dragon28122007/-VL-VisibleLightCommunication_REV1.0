/**
 * @file	main.c
 * @brief	main code
 */
#include "include.h"

void main(void)
{
	system_SetState(SYSTEM_POWER_UP);
	Config_System();

	Button_init();
	Config_Pins();
	LED_Display_init();



	ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_PIN_4);
	ROM_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_7, GPIO_PIN_7);
	ROM_IntMasterEnable();
	Timer_Init();

	while (1)
	{
		system_Process_System_State();
	}
}
