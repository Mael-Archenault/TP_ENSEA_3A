#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"
#include "sleep.h"

#define INCREASING 1
#define DECREASING 0

#define TEST_LEDS_STATE 1
#define SW2_LEDS_STATE 2
#define CHENILLER_STATE 4


XGpio leds;
XGpio switches;
XGpio buttons;

void initGPIO(){
	XGpio_Initialize(&leds,XPAR_AXI_GPIO_0_DEVICE_ID);
	XGpio_SetDataDirection(&leds, 1, 0x0000);
	XGpio_Initialize(&switches,XPAR_AXI_GPIO_0_DEVICE_ID);
	XGpio_SetDataDirection(&switches, 2, 0xffff);
	XGpio_Initialize(&buttons,XPAR_AXI_GPIO_1_DEVICE_ID);
	XGpio_SetDataDirection(&buttons, 1, 0xffff);
}

void testLeds() {
	XGpio_DiscreteWrite(&leds,1,0xaaaa);
}

void sw2Leds() {
	uint32_t i = XGpio_DiscreteRead(&switches,2);
	XGpio_DiscreteWrite(&leds,1,i);
}

void cheniller(int* state, int* i) {

	XGpio_DiscreteWrite(&leds, 1, *i);
	if (*state == INCREASING) {
		if (*i >= 1<<15) {
			*state = DECREASING;
		}
		*i = *i << 1;
	}
	else if (*state == DECREASING) {
		*i = *i >> 1;
		if (*i <= 1) {
			*state = INCREASING;
		}
	}
	usleep(100000);
}

int main()
{
    init_platform();

    initGPIO();
    int state = TEST_LEDS_STATE;
    int moving_state = INCREASING;
    int led_values = 1;
    while(1){
    	int read_value = XGpio_DiscreteRead(&buttons,1);
    	state = (read_value==0) ? state: read_value;

    	if (state==TEST_LEDS_STATE){
    		testLeds();
    	}
    	if (state==SW2_LEDS_STATE){
    		sw2Leds();
    	}

    	if (state==CHENILLER_STATE){
    		cheniller(&moving_state, &led_values);
    	}
    }

    cleanup_platform();
    return 0;
}
