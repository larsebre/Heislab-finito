/**
* @file
* @brief Functionality for driving the elevator
*/

#ifndef MOTOR_H
#define MOTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "hardware.h"
#include "State.h"
#include "Panel.h"

/**
* @brief Main function for driving the elevator
* @param[in] p Panel for the elevator to drive
* @param[in] s State of the elevator to drive
*/
void elevatorDrive(Panel* p, State* s);

#endif
