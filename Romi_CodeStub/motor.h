#ifndef _MOTOR_H
#define _MOTOR_H

// A class to neatly contain commands for the 
// motors, to take care of +/- values, a min/max
// power value, & pin setup.

class motor_c {
  public:

    // This is a function prototype.
    // The actual function is written outside
    // of the class (see below).
    motor_c( );
    
};

// Constructor: when you create an instance
// of this class, your constructor will be
// called automatically.  You can treat it 
// a bit like the main setup().  What needs
// to be setup initially within this class?
motor_c::motor_c(  ) {
  // ...
}

// You can add your own functions to the class.
// Don't forget to also prototype in the class
// definition above.
//void motor_c::myOwnFunction( float an_argument ) {
//  
//}

#endif
