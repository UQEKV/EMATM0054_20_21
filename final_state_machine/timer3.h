#ifndef _TIMER3_H
#define _TIMER3_H

#include "encoders.h"

volatile long pre_count_e0;
volatile float speed_e0;
volatile long pre_count_e1;
volatile float speed_e1;
//volatile float ave_spd_0;

  
void setupTimer3() {

  
  pre_count_e0 = 0;
  speed_e0 = 0;
  
  pre_count_e1 = 0;
  speed_e1 = 0;
  
//  ave_spd_0 = 0;

  // disable global interrupts
  cli();          

  // Reset timer3 to a blank condition.
  // TCCR = Timer/Counter Control Register
  TCCR3A = 0;     // set entire TCCR3A register to 0
  TCCR3B = 0;     // set entire TCCR3B register to 0

  // First, turn on CTC mode.  Timer3 will count up
  // and create an interrupt on a match to a value.
  // See table 14.4 in manual, it is mode 4.
  TCCR3B = TCCR3B | (1 << WGM32);

  // For a cpu clock precaler of 256:
  // Shift a 1 up to bit CS32 (clock select, timer 3, bit 2)
  // Table 14.5 in manual. 
  TCCR3B = TCCR3B | (1 << CS32);


  // set compare match register to desired timer count.
  // CPU Clock  = 16000000 (16mhz).
  // Prescaler  = 256
  // Timer freq = 16000000/256 = 62500  2000000
  // We can think of this as timer3 counting up to 62500 in 1 second.
  // compare match value = 62500/ 12.5 (we desire 12.5hz).
  OCR3A = 5000;

  // enable timer compare interrupt:
  TIMSK3 = TIMSK3 | (1 << OCIE3A);

  // enable global interrupts:
  sei(); 
}


ISR( TIMER3_COMPA_vect ) {
    // speed caculation
    long E0_diff = -(count_e0 - pre_count_e0);   // 1 encoder count equals 0.15mm ==> cm/s
    speed_e0 = (float) E0_diff * 0.015 / float(80 * 0.001);
    pre_count_e0 = count_e0;

    long E1_diff = -(count_e1 - pre_count_e1);
    speed_e1 = (float) E1_diff * 0.015 / float(80 * 0.001);
    pre_count_e1 = count_e1;
    
//    ave_spd_0 = (ave_spd_0*0.8) + (speed_e0 * 0.2);

}





#endif
