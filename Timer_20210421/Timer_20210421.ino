int count = 0;
unsigned long cur_millis = 0;
unsigned long pre_millis = 0;
int pwm_duty = 1;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  TCCR2A = 0x00;
  TCCR2B = 0x02;
  TCNT2 = 256 - 200;
  TIMSK2 = 0x01;
  
  pinMode(13,OUTPUT);
}
void loop(){
  cur_millis = millis();
  if(cur_millis - pre_millis >10){
    pre_millis = cur_millis;

    pwm_duty ++;
    if(pwm_duty == 100){
      pwm_duty = 1;
    }
  }
  
}
SIGNAL(TIMER2_OVF_vect){
  TCNT2 = 256-200;
  count++;
  if(count ==100){
    digitalWrite(13,HIGH);
    count = 0;
  }
  else if(count == pwm_duty){
    digitalWrite(13,LOW);
  }
}
