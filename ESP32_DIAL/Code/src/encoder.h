/**
 * @author: Pomin
 * @date: 2022-12-20 18:25:46
 * @github: https://github.com/POMIN-163
 * @lastedit: 2022-12-20 18:25:46
 * @description:
**/
const int PIN_ENC_A = 22;
const int PIN_ENC_B = 23;
class RotaryEncoder{
  int16_t last_encoder_count = 0;
  uint8_t forward_key = '[';
  uint8_t reverse_key = ']';
  public:
    void init();
    int16_t getPosition();
};
