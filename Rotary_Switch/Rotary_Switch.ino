#define ENCODER_A 10
#define ENCODER_B 11
#define ENCODER_BTN 16

#define RIGHT 2
#define LEFT 1
void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT);
  pinMode(ENCODER_BTN, INPUT);

  digitalWrite(ENCODER_A, HIGH);
  digitalWrite(ENCODER_B, HIGH);
  digitalWrite(ENCODER_BTN, HIGH);
}
char flag_button = 0;
char flag_up = 0;
char flag_down = 0;

unsigned long pre_millis = 0;
unsigned long curr_millis = 0;

int state = 0;
int direction = 0;
int left_state = 0;
int right_state = 0;

void loop() {
  // put your main code here, to run repeatedly:

  int in_a = digitalRead(ENCODER_A);
  int in_b = digitalRead(ENCODER_B);
  if (in_a == 1 && in_b == 1) {

    if (right_state == 3)
    {
      Serial.println("r");
    }
    
    else if (left_state == 3)
    {
      Serial.println("l");
    }
    
    left_state = 0;
    right_state = 0;
  }
  if (in_a == 0 && in_b == 1) {
    if (right_state == 0) {
      right_state = 1;
    }
    else if (left_state == 2) {
      left_state = 3;
    }
  }
  if (in_a == 0 && in_b == 0) {
    if (right_state == 1)
      right_state = 2;
    else if (left_state == 1) {
      left_state = 2;
    }
  }
  if (in_a == 1 && in_b == 0) {
    if (right_state == 2) {
      right_state = 3;
    }
    else if (left_state == 0) {
      left_state = 1;
    }
  }
  delay(1);
}

//=======================================================

//void loop() {
//  // put your main code here, to run repeatedly:
//
//  int in_a = digitalRead(ENCODER_A);
//  int in_b = digitalRead(ENCODER_B);
//  if (in_a == 1 && in_b == 1) {
//    if (state == 3&&direction == RIGHT) {
//      Serial.println("right");
//    }
//    if (state == 1&&direction == LEFT) {
//      Serial.println("left");
//    }
//    direction = 0;
//    state = 0;
//  }
//  if (in_a == 0 && in_b == 1) {
//    if(state == 0){
//      direction = RIGHT;
//    }
//
//    state = 1;
//  }
//  if (in_a == 0 && in_b == 0) {
//    state = 2;
//  }
//  if (in_a == 1 && in_b == 0) {
//    if(state == 0){
//      direction = LEFT;
//    }
//    state = 3;
//  }
//  delay(10);
//}
