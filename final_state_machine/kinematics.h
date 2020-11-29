#ifndef _KINEMATICS_H
#define _KINEMATICS_H


class kinematics_c {
  public:

    // What variables do you need?
    // What is the appropriate type?
    float x;
    float y;
    float theta;

    float last_x;
    float last_y;
    float last_theta;

    unsigned long last_k_ts = millis();

    // Function Prototypes
    kinematics_c(); 
    void k_reset(); // constructor 
    void k_update();    // update kinematics
    
    
}; // End of class definition.


kinematics_c::kinematics_c() {
  
  k_reset();
  
} // end of constructor.

// Routine to execute the update to
// kinematics 
void kinematics_c::k_update() {

  long time_now_k = millis();
  long diff_time_k = time_now_k - last_k_ts;
  float delta_time_k = (float) diff_time_k;
  
  if(delta_time_k > 20){ // sample every 20ms
    
      last_k_ts = time_now_k;
      
      float wheel_seperation = 14.2;     //cm
      theta = last_theta + ( (speed_e0 - speed_e1) * 0.02 ) / wheel_seperation; // accumulation degree
      last_theta = theta;

      x = last_x + ( (speed_e0 + speed_e1) * 0.02 ) / 2.0  ;   // second, cm,  total distance
      last_x = x;
  }
}

void kinematics_c::k_reset(){
     x = 0; //total distance
     y = 0;
     theta = 0;

     last_x = 0;
     last_y = 0;
     last_theta = 0;

     last_k_ts = millis();
}





#endif
