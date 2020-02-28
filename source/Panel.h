/**
* @file
* @brief Functionality in conjunction with elevator panel
*/

#ifndef PANEL_H
#define PANEL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "hardware.h"
#include "State.h"


/**
* @brief Struct to contain information about buttons pushed on elevator panels
* @param[out] orders Two-dimensional array containing information about incoming orders
*/
typedef struct{
	int orders[8];
	bool justPressedStop;
}Panel;

/**
* @brief Enum containing numbers according to type of order
*/
enum ORDERTYPE {ORDER_UP = 1, ORDER_INSIDE = 0, ORDER_DOWN = -1};
enum ARRAY_OFFSET{DIR_OFFSET = 4};

/**
* @brief Function to clear all orders
* @param[in] orders Array of orders 
*/
void cleanOrders(int* orders);

/**
* @brief Function to check if order is already in array, return -1 if not
* @param[in] orders Array of orders
* @param[in] floor Floor to check
*/
int checkOrders(int* orders, int floor);

/**
* @brief Function to initialize panel
* @param[in] p Panel to initialize 
*/
void panelDefault(Panel* p);

/**
* @brief Function to shift orders to the left side
* @param[in] orders Array to leftshift 
*/
void lsOrders(int* orders);

/**
* @brief Function to push orders into orders array
* @param[in] p Panel to push orders into 
*/
void pushOrders(Panel* p);

/**
* @brief Function to add time-delay and be able to push orders while in delay
* @param[in] p Panel to push orders into while in delay
* @param[in] number_of_seconds Number of seconds to delay
*/
void delay(Panel* p, int number_of_seconds);

/**
* @brief Function to return highest value in orders array
* @param[in] p Panel containing orders to check out
* @param[in] s State of elevator
*/
int maxValue(Panel* p, State* s);

/**
* @brief Function to return lowest value in orders array
* @param[in] p Panel containing orders to check out
* @param[in] s State of elevator
*/
int minValue(Panel* p, State* s);

/**
* @brief seriesOfDowns function to check if orders include a series of downward-orders (return true if there is)
* @param[in] p Panel containing orders to check out  
*/
bool seriesOfDowns(Panel* p);

/**
* @brief seriesOfUps function to check if orders include a series of upward-orders (return true if there is)
* @param[in] p Panel containing orders to check out   
*/
bool seriesOfUps(Panel* p);

/**
* @brief Function to return next floor to go to
* @param[in] p Panel containing orders to check out
* @param[in] s State of elevator
*/
int floorCalculations(Panel* p, State* s);

/**
* @brief Function to clear executed orders
* @param[in] p Panel containing executed orders to clear 
* @param[in] s State of elevator
*/
void clearExecuted(Panel* p, State* s);

/**
* @brief Function to check if elevator has reached the correct floor
* @param[in] p Panel containing orders to check out 
* @param[in] s State of elevator
*/
void floorReached(Panel* p, State* s);

/**
* @brief Function to check if there are orders in the orders array
* @param[in] p Panel containing orders to check out
*/
bool checkIfOrders(Panel* p);

/**
* @brief Function to set order lights
* @param[in] p Panel containing orders we want to turn on lights for 
*/
void setOrderLights(Panel* p);

#endif
