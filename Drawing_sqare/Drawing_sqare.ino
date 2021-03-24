#define X_DIR 21
#define X_STEP 15
#define X_EN 14
#define Y_DIR 23
#define Y_STEP 22

int len;
void setup() {
  // put your setup code here, to run once:
  //X-DIR = PC 5번 아두이노 번호 21번
  //X-STEP = PD 7번 아두이노 번호 15번
  //Y-DIR = PC 7번 아두이노 번호 23번
  //Y-STEP = PC 6번 아두이노 번호 22번
  //XYEENABLE = PD 6번 아두이노 번호 14번
  // 원하는 길이(mm) / 0.0125

  Serial.begin(9600);
  pinMode(X_DIR,OUTPUT);
  pinMode(X_STEP,OUTPUT);
  pinMode(X_EN,OUTPUT);
  pinMode(Y_DIR,OUTPUT);
  pinMode(Y_STEP,OUTPUT);

  digitalWrite(Y_DIR,HIGH);
  digitalWrite(X_DIR,HIGH);
  digitalWrite(X_EN,LOW);
}

void X_Move(int len){
  for(int i=0;i<len;i++){
  digitalWrite(X_STEP,HIGH);
  delayMicroseconds(200);
  digitalWrite(X_STEP,LOW);
  delayMicroseconds(200);
  }
}
void Y_Move(int len){
  for(int i=0;i<len;i++){
  digitalWrite(Y_STEP,HIGH);
  delayMicroseconds(200);
  digitalWrite(Y_STEP,LOW);
  delayMicroseconds(200);
  }
}



void loop() {
  len = Serial.parseInt();
  len*=80;
  Serial.println(len);
 
  digitalWrite(X_DIR, HIGH);
  X_Move(len);
  digitalWrite(Y_DIR,LOW);
  Y_Move(len);
  digitalWrite(X_DIR, LOW);
  X_Move(len);
  digitalWrite(Y_DIR,HIGH);
  Y_Move(len);
  delay(6000);
}
