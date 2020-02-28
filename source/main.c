#include <stdio.h>
#include <stdlib.h>
#include "Panel.h"
#include "State.h"
#include "Motor.h"


int main(){
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }
    
	Panel p;
	Panel* p_panel = &p;
	panelDefault(p_panel);
	
	State s;
	State* p_state = &s;
	stateDefault(p_state);
	
	while(1){
			
		stateControl(p_state);
		floorReached(p_panel, p_state);
		pushOrders(p_panel);
		setOrderLights(p_panel);
		elevatorDrive(p_panel, p_state);
    }

    return 0;
}
