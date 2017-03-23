/*
 * user_defines.h
 *
 *  Created on: 12 mar 2017
 *      Author: Mateusz
 */

#ifndef USERSOURCE_USER_DEFINES_H_
#define USERSOURCE_USER_DEFINES_H_

////////////////////////////////MOTOR1 DEFINES////////////////////////////////
#define MOTOR1_MAX_FORWARD_PWM             700
#define MOTOR1_MAX_FORWARD_PWM_CHANGE      30
#define MOTOR1_MAX_BACKWARD_PWM            700
#define MOTOR1_MAX_BACKWARD_PWM_CHANGE     30

#define MOTOR1_PID_CURRENT_KP              40
#define MOTOR1_PID_CURRENT_TI              400000       /*in nanoseconds*/
#define MOTOR1_PID_CURRENT_TD              0            /*in nanoseconds*/
#define MOTOR1_PID_CURRENT_TP              50000        /*in nanoseconds*/

////////////////////////////////MOTOR2 DEFINES////////////////////////////////
#define MOTOR2_MAX_FORWARD_PWM             700
#define MOTOR2_MAX_FORWARD_PWM_CHANGE      20
#define MOTOR2_MAX_BACKWARD_PWM            700
#define MOTOR2_MAX_BACKWARD_PWM_CHANGE     20

#define MOTOR2_PID_CURRENT_KP              40
#define MOTOR2_PID_CURRENT_TI              350000        /*in nanoseconds*/
#define MOTOR2_PID_CURRENT_TD              0             /*in nanoseconds*/
#define MOTOR2_PID_CURRENT_TP              50000         /*in nanoseconds*/

#endif /* USERSOURCE_USER_DEFINES_H_ */
