int svo = 7;
int count = 0;
int duty_count = 22;
int servo_duty = 700;
unsigned int count_1 = 0;
bool ck = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(svo, OUTPUT);
  Serial.begin(9600);
}

unsigned long pre_micros = 0;
unsigned long curr_micros = 0;
unsigned long pre_millis = 0;
unsigned long curr_millis = 0;
void loop(){
  curr_micros = micros();
  curr_millis = millis();

  if(curr_millis - pre_millis >100){
    pre_millis = curr_millis;
    duty_count++;
    if(duty_count > 72)
    duty_count = 22;
  }
  
  if(curr_micros - pre_micros > 32){
    pre_micros = curr_micros;

    if(count == 625){
      count =0;
      digitalWrite(svo,HIGH);
    }
    if(count == duty_count){
      digitalWrite(svo,LOW);
    }
    count++;
  }
}
//void loop() {
//  // put your main code here, to run repeatedly:
//
//
//  //----------------------------------------------
//  if (count == 625) {
//    count = 0;
//    digitalWrite(svo, HIGH);
//  }
//
//  if (count == duty_count) {
//    digitalWrite(svo, LOW);
//  }
//
//  count++;
//  //----------------------------------------------
//
//  if (count_1 == 625) {
//    count_1 = 0;
//    if(ck==0)
//    duty_count++;
//    else if(ck == 1)
//    duty_count--;
//    if (duty_count > 72 &&ck == 0)ck=1;
//    if (duty_count < 22 &&ck == 1)ck=0;
//  }
//
//  count_1++;
//  //----------------------------------------------
//  delayMicroseconds(32);
//
//
//  //
//  //  for (int i = 0; i < 50; i++) {
//  //    digitalWrite(svo, HIGH);
//  //    delayMicroseconds(700 + cnt * 32);
//  //    digitalWrite(svo, LOW);
//  //    delay(17);
//  //    delayMicroseconds(2300 - cnt * 32);
//  //  }
//
//  //  for (int j = 0; j < 50; j++) {
//  //    for (int i = 0; i < 2; i++) {
//  //      digitalWrite(svo, HIGH);
//  //      delayMicroseconds(servo_duty);
//  //      digitalWrite(svo, LOW);
//  //      delay(17);
//  //      delayMicroseconds(3000 - servo_duty);
//  //    }
//  //    servo_duty += 32;
//  //    if(servo_duty >=2300){
//  //      servo_duty =700;
//  //    }
//  //  }
//  //  cnt++;
//  //  if (cnt >= 180) {
//  //    cnt = 0;
//  //  }
//}
