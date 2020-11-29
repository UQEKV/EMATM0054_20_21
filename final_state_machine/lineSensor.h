#ifndef _LINESENSOR_H
#define _LINESENSOR_H

#include "motor_c.h"
#include "defs.h"

//this head file will not be used because it can just be used for one sensor
// lineSensor_all.h is applied in my program

class lineSensor_c{
   public: 

      int pin;
      float bias;
      float calibratedLine;
      float line_in;
      unsigned long last_time_0;
 
      
//      Motor_c l_motor(PWM_L, DIR_L);
//      Motor_c r_motor(PWM_R, DIR_R);
      
      lineSensor_c(int which_pin){

          pin = which_pin;
          bias = 0;
          calibratedLine = 0;
          line_in = 0;
          last_time_0 = 0;
          pinMode(pin, INPUT);
        
        }

        float calibrate();
        float readCalibrated();
        boolean onLine(float threshold);
//        void lineSensor_c::bangbang_controller(float M_value, float power_max)
  
  };

float lineSensor_c::calibrate(){
      float value;
      for (int i = 0; i < 50; i++){
        value = analogRead(pin);
        bias = bias + value;
        delay(8);
        }
      bias = bias / 50.0;
      return bias;
  }

float lineSensor_c::readCalibrated(){
      line_in = analogRead(pin);
      calibratedLine = abs(line_in - bias);
      return calibratedLine;
  }

//
boolean lineSensor_c::onLine(float threshold){
      if (calibratedLine > threshold){
        return true;
      }else if(calibratedLine < threshold){
        return false;
      }
 }

boolean IfLine(boolean L_POS, boolean C_POS, boolean R_POS){
    if(L_POS | C_POS | R_POS){
      return true;
    }else{
      return false;
    }
}

float w_lineSensing(float L_value_c, float C_value_c, float R_value_c){
  
      float T_value_c = 0;
      float p_l = 0;
      float p_c = 0;
      float p_r = 0;
      float M = 0;
      
      T_value_c = L_value_c + C_value_c + R_value_c;
      p_l = L_value_c / T_value_c;
      p_c = C_value_c / T_value_c;
      p_r = R_value_c / T_value_c;
      M = p_l - p_r;
         
}


Motor_c l_motor(PWM_L, DIR_L);
Motor_c r_motor(PWM_R, DIR_R);

void bangbang_controller(float M_value, float power_max){

      if (( M_value > (-0.5) ) && ( M_value < 0.5 )){  // -0.5 <= m <= 0.5 ==> go forward ==> should continue to go forward

           float l_power = power_max;
           float r_power = power_max;
            
            l_motor.setPower(l_power);
            r_motor.setPower(r_power);
        
      } else {   // -1 <= m <= -0.5 ==> go left ==> should turn right, 0.5 <= m <= 1 ==> go right ==> should turn left
        
            float l_power = M_value * power_max * (-1);
            float r_power = M_value * power_max * (+1);
            
            l_motor.setPower(l_power);
            r_motor.setPower(r_power);
            
        }
      
}

#endif
