int led = 13;
int green=12;
void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  pinMode(green,OUTPUT);
}

int duty = 1;
int count =0;
int count_variable=0;
int check1=0;
void loop() {
  // put your main code here, to run repeatedly:
//
//  digitalWrite(green,HIGH);
//  delay(1000);
//  digitalWrite(green,LOW);
//  delay(1000);
  
//  for(int i = 0;i<1;i++){
//    digitalWrite(led,HIGH);
//  delayMicroseconds(duty);
//  digitalWrite(led,LOW);
//  delayMicroseconds(10000-duty);
//  }
//  duty += 100;
//  if(duty > 9900) duty = 100;

// duty
// ---------------------------
  if(count == 100){
    count = 0;
    digitalWrite(13,HIGH);
  }
  else if(count == duty){
    digitalWrite(13,LOW);
  }
// ---------------------------
// 가변
  count_variable++;
  if(count_variable == 100){
      count_variable = 0; 
      if(check1 == 0) duty++;
      else if(check1 == 1)duty--;
      if(duty == 99)check1 = 1;
      else if(duty == 0) check1 = 0;
  }
//---------------------------
  count++;
  delayMicroseconds(100);
  
  
}
