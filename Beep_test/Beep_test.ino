void setup() {
  // put your setup code here, to run once:
  pinMode(27,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
      digitalWrite(27,HIGH);
      delay(1000);
      digitalWrite(27,LOW);
      delay(1000);
}
