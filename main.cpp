#include <iostream>
#include "RobotCom.h"
#include "PrVector.h"
#include "Vision.h"

int const LEFT_POLE = 160;
int const RIGHT_POLE = 320;
float const MPP = 1.0 / 100;

int main(int argc, char** argv) {
    initVision();
    while (1) {
        bool kickInProgress;
        float y_f = processFrame(kickInProgress);
        if (kickInProgress) {
            if (y_f < RIGHT_POLE * MPP && y_f > LEFT_POLE * MPP) {
                // operational control to y_f
                printf("We got it!\n");
            } else {
                printf("LOL you miss it!\n");
                // LOL you missed it!
            }
        } else {
            printf("reset\n");
            // reset operational control to zero
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
