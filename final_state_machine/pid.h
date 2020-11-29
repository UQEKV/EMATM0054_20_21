#ifndef _PID_h
#define _PID_h

class PID_c {
  public:

    PID_c(float P, float I, float D);                 // Constructor, not order of P I & D arguments when calling.
    void setGains(float P, float I, float D);       // This function updates the values of the gains
    void reset();                                   // Useful to remove any intergral wind-up
    float pid_update(float demand, float measurement);  // This update takes a demand and measurement.

    void printComponents(); //This function prints the individual components of the control signal and can be used for debugging

    float Kp_output = 0;
    float Ki_output = 0;
    float Kd_output = 0;

    /* Private functions and variables are defined here. These functions / variables cannot be accessed from outside the class.
       For example, if we try to set the value of Kp in the file "Romi.h", we will get an error (Try it out!)
    */
  private:

    //Control gains
    float Kp; //Proportional
    float Ki; //Integral
    float Kd; //Derivative


    //Values to store
    float output_signal       = 0;
    float last_error          = 0; //For calculating the derivative term
    float integral_error      = 0; //For storing the integral of the error
    unsigned long last_millis = 0;
    float error = 0;
    float error_delta = 0;

};

/*
   Class constructor
   This runs whenever we create an instance of the class
*/
PID_c::PID_c(float P, float I, float D)
{
  //Store the gains
  setGains(P, I, D);
  //Set last_millis
  reset();
}

/*
   This function prints the individual contributions to the total contol signal
   You can call this yourself for debugging purposes, or set the debug flag to true to have it called
   whenever the update function is called.
*/
void PID_c::printComponents() {

  Serial.print(Kp_output);
  Serial.print(", ");
  Serial.print(Kd_output);
  Serial.print(", ");
  Serial.print(Ki_output);
  Serial.print(", ");
  Serial.println(output_signal);
  
}


void PID_c::reset() {
  last_error      = 0;
  integral_error  = 0;
  Kp_output       = 0;
  Ki_output       = 0;
  Kd_output       = 0;
  last_millis     = millis();
  error = 0;
  error_delta = 0;
}

/*
   This function sets the gains of the PID controller
*/
void PID_c::setGains(float P, float I, float D) {
  Kp = P;
  Kd = D;
  Ki = I;
}

/*
   This is the update function.
   This function should be called repeatedly.
   It takes a measurement of a particular quantity and a desired value for that quantity as input
   It returns an output; this can be sent directly to the motors,
   or perhaps combined with other control outputs
*/
float PID_c::pid_update(float demand, float measurement) {
  
  //Calculate how much time (in milliseconds) has 
  // bassed since the last update call
  // Note, we do this in type "long", and then
  // typecast the final result to "float".
  long time_now = millis();
  long diff_time = time_now - last_millis;
 
  
  float time_delta = (float)diff_time;

  if (diff_time > 50){
       last_millis = time_now;
          // Calculate error between demand and measurement.
       error = demand - measurement;
    
      //This represents the error derivative
       error_delta = (error - last_error) / time_delta;
       last_error = error;
    
      // Integral term.
       integral_error = integral_error + (error * time_delta);
    
      //Calculate P,I,D Term contributions.
       Kp_output = Kp * error;
       Kd_output = -Kd * error_delta; 
       Ki_output = Ki * integral_error; 
    
      //Add the three components to get the total output
       output_signal = Kp_output + Kd_output + Ki_output;
//       Serial.print(demand);
//       Serial.print(",");
//       Serial.print(measurement);
//       Serial.print(",");
//       Serial.println(error);
//       Serial.print(",");
//       Serial.println(output_signal);
    
      // Pass the result back.
      
  }
  return output_signal; 
}


#endif
