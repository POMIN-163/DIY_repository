
 #include "User.h"

void setup()
{
  delay(100);
  #ifdef DEBUG 
  {
    Serial.begin(115200);
  }
  #else{

  }
  #endif

  oled.begin();


  eeprom.read_all_data();

  Ticker_init();
  

  miot.begin();

  ec11.set_io(5,4,2);
  ec11.attch_task(ui_key_callb);
  ec11.speed_up(true);
  ec11.speed_up_max(20);


  ui.page_switch_flg = true;


}



void loop(){

  ui.run_task(); 
  eeprom.write_task();
  miot.run_task();
  
}
