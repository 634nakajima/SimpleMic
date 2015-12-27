//main.cpp

#include "Server.h"
#include "Coordinator.h"
#include "ADC.h"
#include "DAC.h"
#include "ModuleManager.h"

int main(void) {
	Server s;
	ModuleManager<ADC>	adc;
	ModuleManager<DAC>	dac;

 	adc.init(&s, "");
    	adc.setMInfo("/Mic", "", "/AudioOut", "Mic.png");
	dac.init(&s, "");
	dac.setMInfo("/Speaker", "/AudioIn,/Volume", "/AudioOut,/Level", "Speaker.png");
	
	while(1);
	return 0;
}
