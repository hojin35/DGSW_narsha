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
  TCCR2B = 0x05;
  TCNT2 = 6;
  OCR2A = 6+25;
  TIMSK2 = 0x03;

}
void loop() {
  curr_millis = millis();
  if(curr_millis - prev_millis >5){
    prev_millis = curr_millis;
    pwm_duty++;
  }
  if(pwm_duty==255)pwm_duty = 6;
}

SIGNAL(TIMER2_COMPA_vect){
    PORTH &= ~(0x01 << 4); //led7번 off
}

SIGNAL(TIMER2_OVF_vect) {
  TCNT2 = 6;
  PORTH |= (0x01 << 4); //led7번 on
  OCR2A++;
  if(OCR2A == 255){
    OCR2A=7;
  }
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
