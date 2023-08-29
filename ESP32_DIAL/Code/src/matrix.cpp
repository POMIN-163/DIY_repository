#include <arduino.h>

#include "matrix.h"

#define MATRIX_DEBOUNCE_VALUE 3

void KeyboardMatrix::init(){
  for (int i =0 ; i < ROW_PINS; i++){
    pinMode(PIN_ROW[i], INPUT_PULLUP);
    //attachInterrupt(PIN_ROW[i], keyboard_isr, FALLING);
    //attachInterrupt(PIN_ROW[i], keyboard_isr, RISING);
    gpio_wakeup_enable((gpio_num_t)PIN_ROW[i], GPIO_INTR_LOW_LEVEL);
  }
    gpio_wakeup_enable((gpio_num_t)26, GPIO_INTR_LOW_LEVEL);
  for (int i =0 ; i < COL_PINS; i++){
    pinMode(PIN_COL[i], OUTPUT);
    digitalWrite(PIN_COL[i], LOW);
  }
  initialized = true;
}

void KeyboardMatrix::scan(int *buff){
  if (!initialized){
    Serial.println("Keyboard Matrix not initialized");
    return;
  }
  for (int i = 0; i < COL_PINS; i++){
      digitalWrite(PIN_COL[i], LOW);
      for (int j = 0; j < ROW_PINS; j++){
        if (PIN_COL[i] == 26 && j == 3){
            pinMode(26, INPUT_PULLUP);
            if (digitalRead(26) == 0) {
              if (key_pressed[j][i] == MATRIX_DEBOUNCE_VALUE){
                buff[j * 3 + i] = KEY_PRESS_EVENT;
              }
              key_pressed[j][i]++;
            } else {
              if (key_pressed[j][i] != 0){
                buff[j * 3 + i] = KEY_UNPRESS_EVENT;
                key_pressed[j][i] = 0;
              }
            }
            // pinMode(26, OUTPUT);
            // digitalWrite(PIN_COL[i], LOW);
            // continue;
        }
        // if (!digitalRead(PIN_ROW[j])){
        //   if (key_pressed[j][i] == MATRIX_DEBOUNCE_VALUE){
        //     buff[j * 3 + i] = KEY_PRESS_EVENT;
        //   }
        //   key_pressed[j][i]++;
        // } else {
        //   if (key_pressed[j][i] != 0){
        //     buff[j * 3 + i] = KEY_UNPRESS_EVENT;
        //     key_pressed[j][i] = 0;
        //   }
        // }
      }
      digitalWrite(PIN_COL[i], HIGH);
    }
}
