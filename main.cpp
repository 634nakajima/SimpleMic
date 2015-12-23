//main.cpp

#include "Server.h"
#include "Coordinator.h"
#include "ADC.h"
#include "ModuleManager.h"

int main(void) {
	Server s;
	ModuleManager<ADC>	adc;

 	adc.init(&s, "");
    	adc.setMInfo("/Mic", "", "/AudioOut", "Mic.png");
	
	while(1);
	return 0;
}
