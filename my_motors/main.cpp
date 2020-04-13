#include <iostream>
#include <signal.h>
#include "motors.h"
#include "lidar_lite.h"


void exit_signal_handler(int signo);

motors mym;

int main()
{
    Lidar_Lite l1(1);
    int err = l1.connect();
    if (err < 0) {
        printf("%d", l1.err);
    }

	signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
    
    
    mym.detect(); // Make sure that the GoPiGo3 is communicating and that the firmware is compatible with the drivers.

    // Reset the encoders
    mym.offset_motor_encoder(MOTOR_LEFT, mym.get_motor_encoder(MOTOR_LEFT));
    mym.offset_motor_encoder(MOTOR_RIGHT, mym.get_motor_encoder(MOTOR_RIGHT));
    bool loop = true;
    while (loop) {
        // Read the encoders
        int32_t EncoderLeft = mym.get_motor_encoder(MOTOR_LEFT);
        int32_t EncoderRight = mym.get_motor_encoder(MOTOR_RIGHT);

        // Use the encoder value from the left motor to control the position of the right motor
        mym.set_motor_position(MOTOR_RIGHT, EncoderLeft);

        int dist = l1.getDistance();
        
        // Display the encoder values
        printf("Encoder Left: %6d  Right: %6d    lidar: %d\n", EncoderLeft, EncoderRight, dist);

        // Delay for 20ms
        usleep(20000);
        //loop = false;
    }
    
    
	std::cout << "hello!" << std::endl;


}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo) {
    if (signo == SIGINT) {
        mym.reset_all();    // Reset everything so there are no run-away motors
        exit(-2);
    }
}
