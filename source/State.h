/**
* @file
* @brief Struct and functionality to handle state of elevator
*/

#ifndef STATE_H
#define STATE_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "hardware.h"



/**
* @brief cleanOrders function to clear all orders
*/
typedef struct{
    int currentFloor;                               // -1 if between floors
    int betweenFloors[2];
    int Direction;
    bool reachedFloor;
}State;

/**
* @brief Enum containg values for elevator direction
*/
enum DIRECTION {UP = 1, STOP = 0, DOWN = -1};

/**
* @brief Function to initialize state of elevator (use only outside of while-loop)
* @param[in] Elevator Elevator to initialize
*/
void stateDefault(State* Elevator);

/**
* @brief Function to check if elevator is in a defined state (returns 1 if it is, 0 if not)
*/
int checkState();

/**
* @brief Function to return the floor elevator is in (needs to be in a defined state to return anything)
* @param[in] check Parameter to tell function if elevator is in a defined state
*/
int getFloor(int check);

/**
* @brief Function to update the state of elevator throughout the loop
* @param[in] Elevator Elevator to follow state of
*/
void stateControl(State* Elevator);


#endif
