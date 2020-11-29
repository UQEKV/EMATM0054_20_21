#ifndef _BEEP_H
#define _BEEP_H
#define BUZZER_PIN 6

void beep(){ // beep
  pinMode(BUZZER_PIN,OUTPUT);
  analogWrite(BUZZER_PIN,80);
  delay(100);
  analogWrite(BUZZER_PIN,0);
  delay(100);
  }

#endif
