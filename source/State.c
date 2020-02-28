/**
* @file
* @brief Implementation of State.h
*/

#include "State.h"

void stateDefault(State* Elevator){

    bool validState = false;
    hardware_command_movement(HARDWARE_MOVEMENT_UP);
    while (!validState){
        for (unsigned int i = 0; i <= 3; i++){
            if (hardware_read_floor_sensor(i)){
                hardware_command_movement(HARDWARE_MOVEMENT_STOP);
                Elevator->currentFloor = i;
                Elevator->betweenFloors[0] = i;
                Elevator->betweenFloors[1] = i;
                Elevator->Direction = STOP;
                Elevator->reachedFloor = true;
                validState = true;
                break;
            }
        }
    }
}

int checkState(){

    for (unsigned int i = 0; i <= 3; i++){
        if (hardware_read_floor_sensor(i)){
            return 1;
        }
    }
    return 0;
}

int getFloor(int check){

	if (check){
		for (unsigned int i = 0; i <= 3; i++){
        	if (hardware_read_floor_sensor(i)){
            	return i;
        	}
    	}
	}
    return 0;
}

void stateControl(State* Elevator){
    
    int check = checkState();
    
    switch (check){
        case 0:
            Elevator->betweenFloors[1] = Elevator->betweenFloors[0] + Elevator->Direction;
            Elevator->reachedFloor = false;
            break;
            
        case 1:
            Elevator->currentFloor = getFloor(check);
            Elevator->betweenFloors[0] = Elevator->currentFloor;
            hardware_command_floor_indicator_on(Elevator->currentFloor);
            break;
    }
}
