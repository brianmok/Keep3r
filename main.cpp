#include <iostream>
#include "RobotCom.h"
#include "PrVector.h"
#include "Vision.h"

int const LEFT_POLE = 160;
int const RIGHT_POLE = 320;
int const GOAL_LINE;
int const PENALTY_Line;
float const MPP = 1.0 / 100;
int const SAFETY=0.1*(RIGHT_POLE-LEFT_POLE);
boolean ReturnKick=false;
int main(int argc, char** argv) {
    initVision();
    while (1) {
        bool kickInProgress;
        float y_f = processFrame(kickInProgress);
        if (kickInProgress) {
            if (y_f < (RIGHT_POLE+SAFETY) * MPP&& y_f > (LEFT_POLE-SAFETY) * MPP) {
                // joint  control to y_f
                //
                printf("Block It!\n");
                ReturnKick=true;
                
            } else {
                printf("LOL You Miss It!\n");
                // LOL you missed it!
            }
        } 
        else {
        	if (ReturnKick==true)
        	{
        	// check if return kick needed, need to pass x and compare to goal line,penalty line
        	if (y_f < (RIGHT_POLE-SAFETY) * MPP && y_f > (LEFT_POLE+SAFETY) * MPP)
        	{
        	// joint control to kick
        	}
        	ReturnKick=false;
        	}
        	else
        	{
            printf("reset\n");
            // reset joint control to zero
        	}
        }
    }
    // currently, none of the code past here testing the robot actually runs, it just does the vision loop.
  
    std::cout<<"This program tests the network connectivity between the client simulator and the servo server"<<std::endl;

/****************************************/
//TEST 1: ESTABLISH CONNECTION
/****************************************/
	RobotCom *test_robot = new RobotCom();
	
	std::cout<<"Sucessfully connected!"<<std::endl;

/****************************************/
//TEST 2: SEND MESSAGE TO THE SERVO SERVER
/****************************************/
	test_robot->jointControl(JMOVE, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	// we'll probably actually use something like test_robot->control(TRACK, targetPosArray, 6) for operational space
	
/****************************************/
//TEST 3: RECEIVE REPLIES FROM THE SERVO SERVER
/****************************************/
	float data_in[8];
	test_robot->getStatus(GET_JPOS, data_in);
	std::cout<<"Joint angles are "<<data_in[0]<<" "<<data_in[1]<<" "<<data_in[2]<<" "<<data_in[3]<<" "<<data_in[4]<<" "<<data_in[5]<<" "<<data_in[6]<<" "<<data_in[7]<<std::endl;

/****************************************/
//TEST 4: TEAR DOWN
/****************************************/

	delete test_robot;

	std::cout<<"Successfully disconnected!"<<std::endl;

  return 0;
}
