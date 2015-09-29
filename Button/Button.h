/**
 *	Raise your ARM 2015 sample code http://raiseyourarm.com/
 *	Author: Pay it forward club
 *	http://www.payitforward.edu.vn
 *  version 0.0.1
 */

/**
 * @file	Button.h
 * @brief	Push button driver
 */

#ifndef BUTTON_BUTTON_H_
#define BUTTON_BUTTON_H_

#define BUTTON_DEBOUNCE_MS		20

#define SW1_ON (!GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4))
#define SW2_ON (!GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0))

typedef enum
{
	BUTTON_NONE = 0,
	BUTTON_LEFT,
	BUTTON_RIGHT
} BUTTON_TYPE;

extern void Switch_init(void);
extern void Button_init(void);
extern bool ButtonRegisterCallback(BUTTON_TYPE ButtonSelect, void (*ButtonCallback)());
extern void GPIOPinUnlockGPIO(uint32_t ui32Port, uint8_t ui8Pins);

#endif /* BUTTON_BUTTON_H_ */
