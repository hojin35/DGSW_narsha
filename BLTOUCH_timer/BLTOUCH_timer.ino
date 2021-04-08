#define DIGITAL_WRITE_7_HIGH PORTH |= 0x10;
#define DIGITAL_WRITE_7_LOW PORTH &= ~0x10;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(7, OUTPUT);

  TCCR2A = 0x00;
  TCCR2B = 0x02;
  TCNT2 = 192;
  TIMSK2 = 0x01; 
}
unsigned long curr_millis = 0;
unsigned long pre_millis = 0;
int duty = 20;
int count = 0;

char toggle_1sec = 0;

SIGNAL(TIMER2_OVF_vect){
  TCNT2 = 192;
  count++;
  if(count == 625){
    DIGITAL_WRITE_7_HIGH;
    count = 0;
  }
  else if(count == duty){
    DIGITAL_WRITE_7_LOW;
  }
}

void loop() {

  // put your main code here, to run repeatedly:
  curr_millis = millis();
    if (curr_millis - pre_millis > 1000) {
    pre_millis = curr_millis;
    if (toggle_1sec == 0) {
      toggle_1sec = 1;
      duty = 20;
    }
    else if (toggle_1sec == 1) {
      toggle_1sec = 0;
      duty = 46;
    }
  }


  
  //=============================================================
  //  for (int i = 0; i < 50; i++) {
  //    digitalWrite(7, HIGH);
  //    delayMicroseconds(647);
  //    digitalWrite(7, LOW);
  //    delay(17);
  //    delayMicroseconds(3000 - 647);
  //  }
  //  for (int i = 0; i < 50; i++) {
  //    digitalWrite(7, HIGH);
  //    delayMicroseconds(1473);
  //    digitalWrite(7, LOW);
  //    delay(17);
  //    delayMicroseconds(3000 - 1473);
  //  }

  //===============================================================
}
