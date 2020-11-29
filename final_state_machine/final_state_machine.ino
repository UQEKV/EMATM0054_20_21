#define STATE_INITIAL 0  // define the all the states of Romi
#define STATE_DRIVE_FORWARDS 1
#define STATE_FOUND_LINE 2
#define STATE_BANGBANG 3
#define STATE_GOHOME 4

#define LINE_LEFT_PIN 22  // line sensor pin
#define LINE_CENTER_PIN 21
#define LINE_RIGHT_PIN 20


#include "motor_c.h"
#include "defs.h"
#include "beep.h"
#include "lineSensor_all.h"
#include "encoders.h"
#include "timer3.h"
#include "kinematics.h"
#include "pid.h"

int state;

unsigned long last_timestamp;
float M; // M value calculated by line sensor signal for bangbang system
float P_max;
float line_threshold; // thereshold for line sensor to decide whether on line
//float f_P;
float b_p; //motor power for bangbang

// pid parameter for left and right motors
float p_l = 1.4; //1.4 for 8 
float i_l = 0.006; //0.006
float d_l = 5.2; // 2.5

float p_r = 1.4; //1.4 for 8
float i_r = 0.006; //0.006
float d_r = 2.5; // 2.5

// pid demand for forward driving
float forwardDemand = 8;
float forwardPwr_l;
float forwardPwr_r;

//pid demand for going home
float goHomeDemand = 8;
float goHomePwr_l;
float goHomePwr_r;
float gohomeDir = 1;


//float FindLineDemand = 20;
float FindLinePwr_l = 25; // motor power for finding lines
float FindLinePwr_r = 25;

PID_c l_PID(p_l, i_l, d_l); // pid for left motor
PID_c r_PID(p_r, i_r, d_r); // pid for right motor

lineSensor_c line(LINE_LEFT_PIN, LINE_CENTER_PIN, LINE_RIGHT_PIN);
kinematics_c kin; //kinematics for finding line tactics
kinematics_c kin1; //kinematics for distance calculation
kinematics_c kin2; //kinematics for going home
int n_tactic;
int h_tactic;



void setup() {
  // put your setup code here, to run once:

  Serial.begin(BAUD_RATE);
  delay(1000);
  Serial.println("***RESET***");
  
  line.calibrate(); // calibrate the line sensor to move bias
  
  setupEncoder0(); // setup for left and right encoder to count
  setupEncoder1(); 
  setupTimer3();  // setup timer3 to calculate the speed in a time interval

  line_threshold = 0.0;
  b_p = 30;
//  f_P = 20;
  last_timestamp = millis();
  n_tactic = 1; // initializing for finding line tactics/state
  h_tactic = 1; // initializing for going home tactics/state

  state = STATE_INITIAL;

}

void loop() {
//calcuate the sample time
  unsigned long time_now = millis();
  unsigned long elapsed_time = time_now - last_timestamp;
//update kinematics
  kin.k_update();
  kin1.k_update();
  kin2.k_update();

  if (elapsed_time > 50) {
    last_timestamp = millis();
      line_threshold = 250.0;
      
      line.readCalibrated(); //read the data calibrated
      M = line.w_lineSensing(line.onLine(line_threshold)); // calculating the M factor for bangbang
      
      //final state machine
      if( state == STATE_INITIAL ) {
        //beep 5 times at the beginning
               intialisingBeeps(); 
               l_PID.reset(); //reset pid for forward driving after beeping 5 times
               r_PID.reset();
               
      } else if( state == STATE_DRIVE_FORWARDS ) {
        // drive forward after beeping 5 times
               driveForwards();     
    
      } else if( state == STATE_FOUND_LINE ) {
        // when line can't be found tactics for finding line will be actuated
               foundLineBeeps();
               
      } else if( state == STATE_BANGBANG ){
        //when line can be detected bangbang system will be used to follow line
               BANGBANG(b_p);
        
      } else if(state == STATE_GOHOME){
      // when Romi drive over a distance threshold Romi will use this goHome function
      // which include PID and degree controll to go home
               GoHome(); 
        
      } else {
        
              Serial.println("System Error, Unknown state!");
      }
  

    }
}

void intialisingBeeps() {
//beep 5 times at the beginning when line can't be detected
  if( line.onLine(line_threshold ) == false) {

      beep(); beep(); beep(); beep(); beep();
//transfer to state driving forward
      state = STATE_DRIVE_FORWARDS;
      
  }
} 

void driveForwards() {
//when line can be detected PID will be used to drive in a straight line
  if( line.onLine(line_threshold) == false  ) {

        forwardPwr_l = l_PID.pid_update(forwardDemand, speed_e0);
        forwardPwr_r = r_PID.pid_update(forwardDemand, speed_e1);
        l_motor.setPower(forwardPwr_l);
        r_motor.setPower(forwardPwr_r);
//       l_motor.setPower(25);
//       r_motor.setPower(25);

  } else if( line.onLine(line_threshold) == true ) {
//when the line can't be detected state will be transfered to bangbang system to follow the line
    state = STATE_BANGBANG;

  }

}

void foundLineBeeps() {
//When line can't be detected finding line system will be used to find line
  if( line.onLine(line_threshold) == false  ) {

//When total driving distance over 232cm and driving distance over 245cm Romi will turn right to go home
//When total driving distance over 232cm and driving distance below 245cm Romi will turn left to go home
    if(kin1.x > 232){
      
       if(kin1.x >= 245){
        
      gohomeDir = 1;
      
    } else{
      
      gohomeDir = -1;
      
    }
        kin2.k_reset(); //kinematics is reset for going home and tranfering the state to going home
        state = STATE_GOHOME;
//        l_PID.reset();
//        r_PID.reset();
      
    } else{ //When driving distance below 232 finding line tactics will be used
          
         if(n_tactic == 1){ //tacitc 1: turn right to 90 degree
          
//                FindLinePwr_l = l_PID.pid_update(FindLineDemand, speed_e0);
//                FindLinePwr_r = r_PID.pid_update(-FindLineDemand, speed_e1);
                l_motor.setPower(FindLinePwr_l);
                r_motor.setPower(-FindLinePwr_r);
                    
                    if(kin.theta >= PI/2){ // When Romi drive to 90 degree tacitc will be transferred to tactic 2
                      //and Romi will stop
                      n_tactic  = 2;

                      l_motor.setPower(0);
                      r_motor.setPower(0);
                      
                    }
                    
                 } else if(n_tactic == 2){ //tacitc 2: turn left to 90 degree

//                      FindLinePwr_l = l_PID.pid_update(-FindLineDemand, speed_e0);
//                      FindLinePwr_r = r_PID.pid_update( FindLineDemand, speed_e1);
                      l_motor.setPower(-FindLinePwr_l);
                      r_motor.setPower(FindLinePwr_r);
      
                      if(kin.theta <= -PI/2){ // When Romi drive to -90 degree tacitc will be transferred to tactic3
                     //and Romi will stop
                      n_tactic  = 3;
      
                      l_motor.setPower(0);
                      r_motor.setPower(0);
                      
                    }
                  
                 } else if(n_tactic == 3){ //tacitc 3: turn right to 0 degree

//                      FindLinePwr_l = l_PID.pid_update(FindLineDemand, speed_e0);
//                      FindLinePwr_r = r_PID.pid_update(-FindLineDemand, speed_e1);
                      l_motor.setPower(FindLinePwr_l);
                      r_motor.setPower(-FindLinePwr_r);
      
                   if(kin.theta >= 0){ // When Romi drive to 0 degree tacitc will be transferred to tactic4
                    //and Romi will stop
                      n_tactic  = 4;
                      l_motor.setPower(0);
                      r_motor.setPower(0);
          
                    }
                  } else if(n_tactic == 4){
      
//                      FindLinePwr_l = l_PID.pid_update(FindLineDemand, speed_e0);
//                      FindLinePwr_r = r_PID.pid_update(FindLineDemand, speed_e1);
                    if(kin1.x >= 75){ //When Romi drive far from 75cm Romi in tactic4 will drive forward
                      l_motor.setPower(FindLinePwr_l);
                      r_motor.setPower(FindLinePwr_r);
      
                      kin.k_reset(); // reset kinematics when finishing every finding line tactics
                    } else{ //When Romi don't drive far from 75cm Romi in tactic4 will turn right until finding line
                      
                      l_motor.setPower(FindLinePwr_l);
                      r_motor.setPower(-FindLinePwr_r);
                      
                    }
                  
                 }
            
          }
  
  

  } else if( line.onLine(line_threshold) == true  ) { // When line is found Romi will follow line by using bangbang

    state = STATE_BANGBANG;

  }

}

void BANGBANG(float B_P) {

  if( line.onLine(line_threshold) == true  ) {

          line.bangbang_controller(B_P);
          n_tactic = 1;
          kin.k_reset(); //reset kinematics after finding line in order to use degree accumulation later

  } else if( line.onLine(line_threshold) == false ) {
// When line cant be found line will be found by using method mentioned before
        state = STATE_FOUND_LINE;
//        l_PID.reset();
//        r_PID.reset();

  }

}


void GoHome() {

  Serial.print(kin2.x);
  Serial.print(",");
  Serial.print(kin2.theta);
  Serial.print(",");
  Serial.println(h_tactic);
  if (kin2.x >= 160){ //When Romi drives home over 160cm Romi will stop
    
          l_motor.setPower(0);
          r_motor.setPower(0);
  } else{ 
    
     if( h_tactic == 1  ) { // rotation direction depend on method mendtioned before

    
//          goHomePwr_l = l_PID.pid_update(goHomeDemand, speed_e0);
//          goHomePwr_r = r_PID.pid_update(-goHomeDemand, speed_e1);
          l_motor.setPower(25 * gohomeDir);
          r_motor.setPower(-25 * gohomeDir);
          

         if( abs(kin2.theta) >= 1.445 ) { // heading angle for going home and stop
          
          h_tactic = 2;
          l_motor.setPower(0);
          r_motor.setPower(0);
          l_PID.reset(); // reset pid for going home on straight line 
          r_PID.reset();
          

        }
         
      }  else if( h_tactic == 2  ) { //going home on straight line by using PID
    
          goHomePwr_l = l_PID.pid_update(goHomeDemand, speed_e0);
          goHomePwr_r = r_PID.pid_update(goHomeDemand, speed_e1);
          l_motor.setPower(goHomePwr_l);
          r_motor.setPower(goHomePwr_r);
          
  }
  
  } 
}
