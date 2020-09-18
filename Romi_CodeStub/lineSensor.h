#ifndef _LINESENSOR_H
#define _LINESENSOR_H

class lineSensor_c {
  
  public:

    int pin;

    // Constructor, accepts a pin number as
    // argument and sets this as input.
    lineSensor_c( int which_pin ) {

       // Record which pin we used.
       pin = which_pin;

       // Set this pin to input.
       pinMode( pin, INPUT );
    };


    // Write your calibration routine here
    // to remove bias offset
    void calibrate() {
      float value;

      // To help get you started, we use 
      // the pin set/recorded by the 
      // constructor to do an analogRead.
      value = analogRead( pin );

      // ...
      
    }


    // Write a routine here to check if your
    // sensor is on a line (true or false).
    boolean onLine( float threshold ) {

      // if () {
      //    return true;
      //}
             
      return false;
    }

    // You can define other functions for
    // yourself. 
    // ...

};

#endif
