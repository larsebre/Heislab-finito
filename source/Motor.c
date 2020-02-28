/**
* @file
* @brief Implementation of Motor.h
*/

#include "Motor.h"

void elevatorDrive(Panel* p, State* s){

	while (hardware_read_stop_signal()){
		hardware_command_movement(HARDWARE_MOVEMENT_STOP);
		hardware_command_stop_light(1);
		cleanOrders(p->orders);
		setOrderLights(p);
		p->justPressedStop = true;

		if (s->betweenFloors[0] == s->betweenFloors[1]){
			hardware_command_door_open(1);
			p->orders[3] = s->betweenFloors[0];
		}
		hardware_command_door_open(0);
	}
	hardware_command_stop_light(0);
    
    if (checkIfOrders(p) == true){
    	
    	int nextFloor = floorCalculations(p, s);

		if (p->justPressedStop){		//For right startup between floors
			
			if (((double)nextFloor > ((s->betweenFloors[0] + s->betweenFloors[1])/2.0)) && (s->Direction == DOWN)){
				s->Direction = UP;
				s->betweenFloors[0] = s->betweenFloors[1];
				hardware_command_movement(HARDWARE_MOVEMENT_UP);
			}
			if (((double)nextFloor > ((s->betweenFloors[0] + s->betweenFloors[1])/2.0)) && (s->Direction == UP)){
				s->Direction = UP;
				s->betweenFloors[1] = s->betweenFloors[0];
				hardware_command_movement(HARDWARE_MOVEMENT_UP);
			}

			if (((double)nextFloor < ((s->betweenFloors[0] + s->betweenFloors[1])/2.0)) && (s->Direction == DOWN)){
				s->Direction = DOWN;
				s->betweenFloors[1] = s->betweenFloors[0];
				hardware_command_movement(HARDWARE_MOVEMENT_DOWN);	
			}
			if (((double)nextFloor < ((s->betweenFloors[0] + s->betweenFloors[1])/2.0)) && (s->Direction == UP)){
				s->Direction = DOWN;
				s->betweenFloors[0] = s->betweenFloors[1];
				hardware_command_movement(HARDWARE_MOVEMENT_DOWN);	
			}

			p->justPressedStop = false;
		}
    	
    	if ((s->reachedFloor == true) && (s->betweenFloors[0] == s->betweenFloors[1])){
    		clearExecuted(p, s);
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
           	s->reachedFloor = false;
			hardware_command_door_open(1);
           	delay(p,3);
           	hardware_command_door_open(0);
    	} else {
			if (s->betweenFloors[0] == s->betweenFloors[1]){
				if((nextFloor - s->betweenFloors[0]) >= 0){
            		hardware_command_movement(HARDWARE_MOVEMENT_UP);
            		s->Direction = UP;
            	}
            	if((nextFloor - s->betweenFloors[0]) <= 0){
            		hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
            		s->Direction = DOWN;
				}
            }
		}
    	
    	/*if((s->reachedFloor == false) && (s->betweenFloors[0] == s->betweenFloors[1])){

            if((nextFloor - s->betweenFloors[0]) >= 0){
            	hardware_command_movement(HARDWARE_MOVEMENT_UP);
            	s->Direction = UP;
            }
            if((nextFloor - s->betweenFloors[0]) <= 0){
            	hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
            	s->Direction = DOWN;
            }*/
			
   		
    }else{
    	hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    }  
}
