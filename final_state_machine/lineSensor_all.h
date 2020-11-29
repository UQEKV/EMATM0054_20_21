#ifndef _3LINESENSOR_ALL_H
#define _3LINESENSOR_ALL_H

#include "motor_c.h"
#include "defs.h"

class lineSensor_c{ //class for 3 line sensor
   public: 

      int l_pin;
      int c_pin;
      int r_pin;
      
      float l_bias;
      float c_bias;
      float r_bias;
      
      float l_calibratedLine;
      float c_calibratedLine;
      float r_calibratedLine;
      
      float l_line_in;
      float c_line_in;
      float r_line_in;
      unsigned long last_cali_ts;
      float m;
      
//      unsigned long last_time_0;
 
      
//      Motor_c l_motor(PWM_L, DIR_L);
//      Motor_c r_motor(PWM_R, DIR_R);
      
      lineSensor_c(int L_pin, int C_pin, int R_pin){

          last_cali_ts = millis();
          
          l_pin = L_pin;
          c_pin = C_pin;
          r_pin = R_pin;
          
          l_bias = 0;
          c_bias = 0;
          r_bias = 0;
          
          l_calibratedLine = 0;
          c_calibratedLine = 0;
          r_calibratedLine = 0;
          
          l_line_in = 0;
          c_line_in = 0;
          r_line_in = 0;

          m = 0;
//          last_time_0 = 0;
          pinMode(l_pin, INPUT);
          pinMode(c_pin, INPUT);
          pinMode(r_pin, INPUT);
        
        }

        void calibrate();
        void readCalibrated();
        boolean onLine(float threshold);
        float w_lineSensing(boolean ONLINE);
        void bangbang_controller(float power_max);
        
  };

void lineSensor_c::calibrate(){// sample line signal every 20ms and calculating average of 50 samples
  //for calibrate line sensors to move offset
      float l_value;
      float c_value;
      float r_value;
      
      for (int i = 0; i < 50; i++){

        if(millis() - last_cali_ts > 20){
          
            l_value = analogRead(l_pin);
            c_value = analogRead(c_pin);
            r_value = analogRead(r_pin);
            
            l_bias = l_bias + l_value;
            c_bias = c_bias + c_value;
            r_bias = l_bias + r_value;
        
           }

        }
        
      l_bias = l_bias / 50.0;
      c_bias = c_bias / 50.0;
      r_bias = r_bias / 50.0;
      
  }

void lineSensor_c::readCalibrated(){//read data calibrated
      l_line_in = analogRead(l_pin);
      c_line_in = analogRead(c_pin);
      r_line_in = analogRead(r_pin);
      
      l_calibratedLine = abs(l_line_in - l_bias);
      c_calibratedLine = abs(c_line_in - c_bias);
      r_calibratedLine = abs(r_line_in - r_bias);
  }


boolean lineSensor_c::onLine(float threshold){ // decide whether Romi on the line
  
      if ( (l_calibratedLine > threshold) | (c_calibratedLine > threshold) | (r_calibratedLine > threshold) ){
        
        return true;
        
      } else{
        
        return false;
        
      }
 }
 

float  lineSensor_c::w_lineSensing(boolean ONLINE){ // calculating M factor for bangbang system

    if(ONLINE){
      float T_value_c;
      float p_l;
      float p_c;
      float p_r;
      
      T_value_c = l_calibratedLine + c_calibratedLine + r_calibratedLine;
      p_l = l_calibratedLine / T_value_c;
      p_c = c_calibratedLine / T_value_c;
      p_r = r_calibratedLine / T_value_c;
      
      m = p_l - p_r;
      
      return m;
    } else{
      
    }
         
}

//create left and right motor instance
Motor_c l_motor(PWM_L, DIR_L);
Motor_c r_motor(PWM_R, DIR_R);

void lineSensor_c::bangbang_controller(float power_max){
//When M factor is between -0.5 and 0.5 Romi will move forward
      if (( m > (-0.5) ) && ( m < 0.5 )){  // -0.5 <= m <= 0.5 ==> go forward ==> should continue to go forward

           float l_power = power_max;
           float r_power = power_max;
            
            l_motor.setPower(l_power);
            r_motor.setPower(r_power);
        
      } else {   // -1 <= m <= -0.5 ==> go left ==> should turn right, 0.5 <= m <= 1 ==> go right ==> should turn left
        //When M factor is not between -0.5 and 0.5 Rotation of Romi will depend on M factor
            float l_power = m * power_max * (-1);
            float r_power = m * power_max * (+1);
            
            l_motor.setPower(l_power);
            r_motor.setPower(r_power);
            
        }
      
}

 

#endif
