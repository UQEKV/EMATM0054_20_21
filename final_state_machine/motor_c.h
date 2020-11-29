#ifndef _MOTOR_H
#define _MOTOR_H

#include "defs.h"

 
class Motor_c{
    public:
      int pwm_pin;
      int dir_pin;
      
      Motor_c(int pwm_pin_in, int dir_pin_in){
        pwm_pin = pwm_pin_in;
        dir_pin = dir_pin_in;
        pinMode(pwm_pin, OUTPUT);
        pinMode(dir_pin, OUTPUT);
  
        digitalWrite(pwm_pin, LOW);
        digitalWrite(dir_pin, LOW);
        }
        
      void setPower(float power_in);
  
  };

  void Motor_c::setPower(float power_in){

           if(power_in > 0){
              digitalWrite(dir_pin, LOW); //low voltage is forward
            } 
           else if(power_in < 0){
              digitalWrite(dir_pin, HIGH); //high voltage is backward
            }else{
            }

            if(power_in < 0){
              power_in *= -1;
              }
    
          if(power_in > PWM_MAX){
              power_in = PWM_MAX;
            } 
           else if(power_in < PWM_MIN){
              power_in = PWM_MIN;
            }
          analogWrite(pwm_pin,power_in);
        }



#endif
