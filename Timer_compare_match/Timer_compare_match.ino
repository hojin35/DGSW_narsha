int curr_millis = 0;
int prev_millis = 0;
boolean led = 1;
unsigned int count = 0;
bool toggle_flag = 0;
int pwm_duty = 1;
void setup() {
  // put your setup code here, to run once:
  //pinMode(13,OUTPUT);
  DDRH |= (0x01 << 4);

  TCCR2A = 0x03;
  TCCR2B = 0x02;
  TCNT2 = 0;
  OCR2A = 200;
  TIMSK2 = 0x02;

}
void loop() {
  curr_millis = millis();
  if(curr_millis - prev_millis >10){
    prev_millis = curr_millis;
    pwm_duty++;
  }
  if(pwm_duty==99){
    pwm_duty = 1;
  }
}

SIGNAL(TIMER2_COMPA_vect){
  count++;
  if (count == 100) {
    count = 0;
    PORTH |= (0x01 << 4); //led7번 on
  }
  else if (count == pwm_duty) {
    PORTH &= ~(0x01 << 4); //led7번 off
  }
}

SIGNAL(TIMER2_OVF_vect) {

}


//SIGNAL(TIME2_OVF_vect){
//  TCNT2 = 56;
//  count++;
//  if(count == 10000){
//    count = 0;
//    if(toggle_flag == 0){
//      toggle_flag = 1;
//      PORTH |= (0x01 << 4); //led7번 on
//    }
//    else if(toggle_flag == 1){
//      toggle_flag = 0;
//      PORTH &= ~(0x01 << 4); //led7번 off
//    }
//  }
//}
