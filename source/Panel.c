/**
* @file
* @brief Implementation of Panel.h
*/

#include "Panel.h"

void cleanOrders(int* orders){

	for (int i = 0; i<8; i++){
		orders[i] = -1;
	}
	for (int i = 0; i<=3; i++){
		hardware_command_order_light(i, HARDWARE_ORDER_DOWN, 0);
		hardware_command_order_light(i, HARDWARE_ORDER_INSIDE, 0);
		hardware_command_order_light(i, HARDWARE_ORDER_UP, 0);
	}

}

void panelDefault(Panel* p){
	cleanOrders(p->orders);
	p->justPressedStop = false;
}

int checkOrders(int* orders, int floor){

	for(int i=0; i<=3; i++){
		if(orders[i] == floor){
		return i;
		}	
	}
	return -1;												
}

void lsOrders(int* orders){		

	for (unsigned int i = 0; i <= 2; i++){
		if (orders[i] == -1){			
			for (unsigned int j = i; j <= 2; j++){
				orders[j] = orders[j+1];
				orders[j+DIR_OFFSET] = orders[j+1+DIR_OFFSET];
			
			}
			break;
		}
	}
}

void pushOrders(Panel* p){

	for(unsigned int i=0; i<=3; i++){
		bool check = (hardware_read_order(i, HARDWARE_ORDER_UP) || hardware_read_order(i, HARDWARE_ORDER_INSIDE) || hardware_read_order(i, HARDWARE_ORDER_DOWN));
		if (check){
			if (hardware_read_order(i, HARDWARE_ORDER_UP)){
				 if (checkOrders(p->orders, i)<0){
				 	lsOrders(p->orders);
					p->orders[3] = i;
					p->orders[3+DIR_OFFSET] = 1;	
				 }

			}else if (hardware_read_order(i, HARDWARE_ORDER_INSIDE)){
				if (checkOrders(p->orders, i)<0){
					lsOrders(p->orders);
				 	p->orders[3] = i;
					p->orders[3+DIR_OFFSET] = 0;
				 }
			}else if (hardware_read_order(i, HARDWARE_ORDER_DOWN)){
				if (checkOrders(p->orders, i)<0){
					lsOrders(p->orders);
				 	p->orders[3] = i;
					p->orders[3+DIR_OFFSET] = -1;
				}
			}
		}
	}
}

void delay(Panel* p, int number_of_seconds) { 
	setOrderLights(p);
    int time = 1000000 * number_of_seconds;  
    clock_t start_time = clock(); 
    while (clock() < start_time + time){
		
		while (hardware_read_obstruction_signal()){
			pushOrders(p);
			setOrderLights(p);
			start_time = clock();
		}
		pushOrders(p);
		setOrderLights(p);
		if (hardware_read_stop_signal()) break;
	}
    	
} 

bool checkIfOrders(Panel* p){
	
	for (unsigned int i = 0; i<=3; i++){
		if (p->orders[i] != -1) return true;
	}
	return false;
}


int maxValue(Panel* p, State* s){
	
	if (checkIfOrders(p) == false){
		return s->betweenFloors[0];
	}
	int max = 0;
	for (int i = 0; i<=3; i++){
		if (p->orders[i] > max){
			max = p->orders[i];
		}
	}
	return max;	
}


int minValue(Panel* p, State* s){

	if (checkIfOrders(p) == false){
		return s->betweenFloors[0];
	}	
	int min = 3;
	for (int i = 0; i<=3; i++){
		if ((p->orders[i] < min) && (p->orders[i] != -1)){
			min = p->orders[i];
		}
	}
	return min;	
}

bool seriesOfDowns(Panel* p){

	bool serie = false;
	for (unsigned int i = 0; i<=2; i++){
		if ((p->orders[i] != -1) && (p->orders[i+1] != -1) && (p->orders[i+DIR_OFFSET] == ORDER_DOWN) && (p->orders[i+1+DIR_OFFSET] == ORDER_DOWN)){
			int a = p->orders[i];
			int b = p->orders[i+1];
			serie = true;
			for (unsigned int i = 0; i <= 3; i++){
				if (((p->orders[i] > a) && (p->orders[i] < b)) || ((p->orders[i] < a) && (p->orders[i] > b))){
					serie = false;
				}
			}
		}
		if ((i <= 1) && (p->orders[i] != -1) && (p->orders[i+2] != -1) && (p->orders[i+DIR_OFFSET] == ORDER_DOWN) && (p->orders[i+1+DIR_OFFSET] == ORDER_DOWN)){
			int a = p->orders[i];
			int b = p->orders[i+1];
			serie = true;
			for (unsigned int i = 0; i <= 3; i++){
				if ( (p->orders[i] != -1) && (((p->orders[i] > a) && (p->orders[i] < b)) || ((p->orders[i] < a) && (p->orders[i] > b)))){
					serie = false;
				}
			}
		}  
	}
	return serie;
}

bool seriesOfUps(Panel* p){

	bool serie = false;
	for (unsigned int i = 0; i<=2; i++){
		if ((p->orders[i] != -1) && (p->orders[i+1] != -1) && (p->orders[i+DIR_OFFSET] == ORDER_UP) && (p->orders[i+1+DIR_OFFSET] == ORDER_UP)){
			int a = p->orders[i];
			int b = p->orders[i+1];
			serie = true;
			for (unsigned int i = 0; i <= 3; i++){
				if (((p->orders[i] > a) && (p->orders[i] < b)) || ((p->orders[i] < a) && (p->orders[i] > b))){
					serie = false;
				}
			}
		}
		if ((i <= 1) && (p->orders[i] != -1) && (p->orders[i+2] != -1) && (p->orders[i+DIR_OFFSET] == ORDER_UP) && (p->orders[i+1+DIR_OFFSET] == ORDER_UP)){
			int a = p->orders[i];
			int b = p->orders[i+1];
			serie = true;
			for (unsigned int i = 0; i <= 3; i++){
				if ((p->orders[i] != -1) && (((p->orders[i] > a) && (p->orders[i] < b)) || ((p->orders[i] < a) && (p->orders[i] > b)))){
					serie = false;
				}
			}
		}
	}
	return serie;
}

int floorCalculations(Panel* p, State* s){

	int direction = s->Direction;
	int currentFloor = s->betweenFloors[0];
	int destination = minValue(p,s);
	int distance;
	switch (direction){
		case DOWN:
			distance = currentFloor - minValue(p, s);
			for (int i = 3; i >= 0; i--){
				if (seriesOfUps(p)){
					return minValue(p,s);
				}
					
				if(((currentFloor - p->orders[i]) <= distance) && (p->orders[i] != -1) && (p->orders[i+DIR_OFFSET] != ORDER_UP) && (p->orders[i] <= currentFloor)){
					distance = currentFloor - p->orders[i];
					destination = p->orders[i];
				}
			}
			s->Direction = DOWN;
			return destination;
				
		case UP:
			distance = currentFloor - maxValue(p,s);
			for (int i = 3; i >= 0; i--){
				if (seriesOfDowns(p)){
					return maxValue(p,s);
				}

				if(((currentFloor - p->orders[i]) >= distance) && (p->orders[i] != -1) && (p->orders[i+DIR_OFFSET] != ORDER_DOWN) && (p->orders[i] >= currentFloor)){
					distance = currentFloor - p->orders[i];
					destination = p->orders[i];
				}
			}
			s->Direction = UP;
			return destination;
	}

	return destination;
}

void clearExecuted(Panel* p, State* s){
	
	for (int i = 0; i<=3; i++){
		if (p->orders[i] == s->betweenFloors[0]){
			if (p->orders[i+DIR_OFFSET] == ORDER_DOWN){
				hardware_command_order_light(p->orders[i], HARDWARE_ORDER_DOWN, 0);
			}else if(p->orders[i+DIR_OFFSET] == ORDER_INSIDE){
				hardware_command_order_light(p->orders[i], HARDWARE_ORDER_INSIDE, 0);
			}else if(p->orders[i+DIR_OFFSET] == ORDER_UP){
				hardware_command_order_light(p->orders[i], HARDWARE_ORDER_UP, 0);
			}
			p->orders[i] = -1;
			p->orders[i+DIR_OFFSET] = -1;
		}
	}
}

void floorReached(Panel* p, State* s){
	
	int floor = floorCalculations(p,s);
	
	if (checkIfOrders(p) == true){
		if (floor == s->betweenFloors[0]){
        	s->reachedFloor = true;

    	}else{
        	s->reachedFloor = false;
    	}  
	}   
}

void setOrderLights(Panel* p){
	for (int i = 0; i<=3; i++){
		if(p->orders[i] != -1){
			if (p->orders[i+DIR_OFFSET] == ORDER_DOWN){
				hardware_command_order_light(p->orders[i], HARDWARE_ORDER_DOWN, 1);
			}else if(p->orders[i+DIR_OFFSET] == ORDER_INSIDE){
				hardware_command_order_light(p->orders[i], HARDWARE_ORDER_INSIDE, 1);
			}else if(p->orders[i+DIR_OFFSET] == ORDER_UP){
				hardware_command_order_light(p->orders[i], HARDWARE_ORDER_UP, 1);
			}
		}
	}	
}
