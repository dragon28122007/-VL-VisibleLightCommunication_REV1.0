/**
 *	Raise your ARM 2015 sample code http://raiseyourarm.com/
 *	Author: Pay it forward club
 *	http://www.payitforward.edu.vn
 *  version 0.0.1
 */

/**
 * @file	SystemConfig.h
 * @brief	Config system
 */

#ifndef SYSTEMCONFIG_H_
#define SYSTEMCONFIG_H_


#define DRV_ENABLE_LEFT_CHN_PERIPHERAL		SYSCTL_PERIPH_GPIOB
#define DRV_ENABLE_RIGHT_CHN_PERIPHERAL		SYSCTL_PERIPH_GPIOB
#define DRV_ENABLE_LEFT_CHN_PORT			GPIO_PORTB_BASE
#define DRV_ENABLE_RIGHT_CHN_PORT			GPIO_PORTB_BASE
#define DRV_ENABLE_LEFT_CHN_PIN				GPIO_PIN_3
#define DRV_ENABLE_RIGHT_CHN_PIN			GPIO_PIN_7

#define BOOST_ENABLE_PREIPHERAL				SYSCTL_PERIPH_GPIOA
#define BOOST_ENABLE_PORT					GPIO_PORTA_BASE
#define BOOST_ENABLE_PIN					GPIO_PIN_7

#define LED1_PERIPHERAL						SYSCTL_PERIPH_GPIOF
#define LED1_PORT							GPIO_PORTF_BASE
#define LED1_PIN							GPIO_PIN_1
#define LED1_ON()							ROM_GPIOPinWrite(LED1_PORT, LED1_PIN, LED1_PIN)
#define LED1_OFF()							ROM_GPIOPinWrite(LED1_PORT, LED1_PIN, 0x00)
#define LED1_TOGGLE()						ROM_GPIOPinWrite(LED1_PORT, LED1_PIN, ~ROM_GPIOPinRead(LED1_PORT,LED1_PIN))

#define LED2_PERIPHERAL						SYSCTL_PERIPH_GPIOF
#define LED2_PORT							GPIO_PORTF_BASE
#define LED2_PIN							GPIO_PIN_2
#define LED2_ON()							ROM_GPIOPinWrite(LED2_PORT, LED2_PIN, LED2_PIN)
#define LED2_OFF()							ROM_GPIOPinWrite(LED2_PORT, LED2_PIN, 0x00)
#define LED2_TOGGLE()						ROM_GPIOPinWrite(LED2_PORT, LED2_PIN, ~ROM_GPIOPinRead(LED2_PORT,LED2_PIN))

#define LED3_PERIPHERAL						SYSCTL_PERIPH_GPIOF
#define LED3_PORT							GPIO_PORTF_BASE
#define LED3_PIN							GPIO_PIN_3
#define LED3_ON()							ROM_GPIOPinWrite(LED3_PORT, LED3_PIN, LED3_PIN)
#define LED3_OFF()							ROM_GPIOPinWrite(LED3_PORT, LED3_PIN, 0x00)
#define LED3_TOGGLE()						ROM_GPIOPinWrite(LED3_PORT, LED3_PIN, ~ROM_GPIOPinRead(LED3_PORT,LED3_PIN))
typedef enum
{
	SYSTEM_POWER_UP = 0,
	SYSTEM_RUN,
	SYSTEM_ERROR
} SYSTEM_STATE;

extern void Config_System(void);
extern void Config_Pins(void);
extern void BattSense_init(void);
extern void LED_Display_init(void);
extern SYSTEM_STATE system_GetState(void);
extern void system_SetState(SYSTEM_STATE SysState);
extern void system_Process_System_State(void);
extern uint32_t u32_UsrSystemClockGet();

#endif /* SYSTEMCONFIG_H_ */
