#define X_DIR 21
#define X_STEP 15
#define X_EN 14
#define Y_DIR 23
#define Y_STEP 22

void setup() {
  // put your setup code here, to run once:
  //X-DIR = PC 5번 아두이노 번호 21번
  //X-STEP = PD 7번 아두이노 번호 15번
  //Y-DIR = PC 7번 아두이노 번호 23번
  //Y-STEP = PC 6번 아두이노 번호 22번
  //XYEENABLE = PD 6번 아두이노 번호 14번

  pinMode(X_DIR,OUTPUT);
  pinMode(X_STEP,OUTPUT);
  pinMode(X_EN,OUTPUT);
  pinMode(Y_DIR,OUTPUT);
  pinMode(Y_STEP,OUTPUT);

  digitalWrite(Y_DIR,HIGH);
  digitalWrite(X_DIR,HIGH);
  digitalWrite(X_EN,LOW);
  
}

void X_Move(){
  for(int i=0;i<4000;i++){
  digitalWrite(X_STEP,HIGH);
  delayMicroseconds(200);
  digitalWrite(X_STEP,LOW);
  delayMicroseconds(200);
  }
}
void Y_Move(){
  for(int i=0;i<4000;i++){
  digitalWrite(Y_STEP,HIGH);
  delayMicroseconds(200);
  digitalWrite(Y_STEP,LOW);
  delayMicroseconds(200);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(X_DIR, HIGH);
  X_Move();
  digitalWrite(Y_DIR,HIGH);
  Y_Move();
  digitalWrite(X_DIR, LOW);
  X_Move();
  digitalWrite(Y_DIR,LOW);
  Y_Move();
  delay(6000);
  /*
  for(int i=0;i<1000;i++){
  digitalWrite(X_DIR, LOW);
  digitalWrite(Y_DIR, LOW);
  digitalWrite(X_STEP,HIGH);
  digitalWrite(Y_STEP,HIGH);
  delayMicroseconds(200);
  digitalWrite(X_STEP,LOW);
  digitalWrite(Y_STEP,LOW);
  delayMicroseconds(200);
  }*/
}
