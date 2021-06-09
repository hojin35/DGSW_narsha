#define DATA 7
#define CLK 8
#define LATCH 9

int value_74595= 0x03;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  for (int i = 0; i < 3; i++) {
    pinMode(DATA + i, OUTPUT);
  }
  spi_74595(value_74595);
}
int count = 0;
int count_variable = 0;
int pwm_duty = 1;
void loop() {
  // put your main code here, to run repeatedly:
  count++;
  count_variable++;
  if(count ==100){
    count = 0;
    value_74595 |= 0x01;
    spi_74595(value_74595);
  }
  else if(count == pwm_duty)
  {
    value_74595 &= ~0x01;
    spi_74595(value_74595);
  }
  if(count_variable == 100)
  {
    count_variable = 0;
    pwm_duty++;

    if(pwm_duty==99)
    {
      pwm_duty = 1;
    }
  }
  delayMicroseconds(100);
}
void spi_74595(unsigned char data){
  for (int i = 0; i < 8; i++) {
    if (data & (0x80 >> i))
    {
      digitalWrite(DATA, HIGH);
    }
    else
    {
      digitalWrite(DATA, LOW);
    }
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW);
  }
  digitalWrite(LATCH, HIGH);
  digitalWrite(LATCH, LOW);
}
