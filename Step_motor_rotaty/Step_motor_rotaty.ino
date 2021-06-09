// Encoder a pd2   10
// Encoder b pd3   11
// Encoder btn pc0 16


// A왼쪽 B오른쪽
#define ENC_A 11
#define ENC_B 10
#define ENC_BUTTON 16

#define X_STOP 18
#define X_DIR 21
#define X_STEP 15

#define Y_STOP 19
#define Y_DIR 23
#define Y_STEP 22

#define XY_EN 14

enum enc_state {
  ENC_STATE_1 = 1,
  ENC_STATE_2,
  ENC_STATE_3,
  ENC_STATE_4
};
enum run_stop {
  RUN = 0,
  STOP
};
enum direction {
  DIR_LEFT = 0,
  DIR_RIGHT,
  DIR_UP,
  DIR_DOWN
};

enum menu_pos {
  MAIN_MENU_NONE = 0,
  MAIN_LEFT_STEP,
  MAIN_RIGHT_STEP,
  MAIN_SPEED_0_1,
  MAIN_SPEED_1,
  MAIN_SPEED_10,
  MAIN_SPEED_SET
};

int x_stop_value = 0;
int value_enc_a = 0;
int value_enc_b = 0;
int curr_enc_state = 0;
int prev_enc_state = 0;
int count = 0;
double distance = 0;
int num_of_step_count = 0;
int step_count = 0;

char menu_position = MAIN_MENU_NONE;

char right_flag = 0;
char left_flag = 0;
char toggle_flag_x = 0;
char toggle_flag_y = 0;
char run_stop = 0;

void main_menu() {
  Serial.println("1. left step");
  Serial.println("2. right step");
  Serial.println("3. speed 0.1");
  Serial.println("4. speed 1");
  Serial.println("5. speed 10");
  Serial.println("6. speed set");
  Serial.println("> ");
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);

  //pull up
  digitalWrite(ENC_A, HIGH);
  digitalWrite(ENC_B, HIGH);

  pinMode(X_DIR, OUTPUT);
  pinMode(Y_DIR, OUTPUT);
  pinMode(X_STEP, OUTPUT);
  pinMode(Y_STEP, OUTPUT);
  pinMode(XY_EN, OUTPUT);

  pinMode(X_STOP, INPUT);
  pinMode(Y_STOP, INPUT);

  digitalWrite(X_STOP, HIGH);
  digitalWrite(Y_STOP, HIGH);
  //  / 또는 위에 _있으면 LOW ACTIVE

  digitalWrite(X_DIR, HIGH);

  //TIMER2 setting 8bit
  TCCR2A = 0x02;
  TCCR2B = 0x03;
  TCNT2 = 0;
  OCR2A = 100;
  TIMSK2 = 0x02;

  //init value
  distance = 1;
  num_of_step_count = distance * 80;

  run_stop = RUN;

  main_menu();
}
void direction_x(char dir) {
  if (dir == DIR_LEFT) {
    digitalWrite(X_DIR, HIGH);
  }
  else {
    digitalWrite(X_DIR, LOW);
  }
}
void direction_y(char dir) {
  if (dir == DIR_UP) {
    digitalWrite(Y_DIR, LOW);
  }
  else {
    digitalWrite(Y_DIR, HIGH);
  }
}

void loop() {

  if (Serial.available())
  {
    char key_value = Serial.read();
    if (key_value == '1') {
      menu_position = MAIN_LEFT_STEP;
      direction_x(DIR_LEFT);
      num_of_step_count = (int)(distance * 80);
      run_stop = RUN;
    }
    else if (key_value == '2') {
      menu_position = MAIN_RIGHT_STEP;
      direction_x(DIR_RIGHT);
      num_of_step_count = (int)(distance * 80);
      run_stop = RUN;
    }
    else if (key_value == '3') {
      menu_position = MAIN_SPEED_0_1;
      distance = 0.1;
    }
    else if (key_value == '4') {
      menu_position = MAIN_SPEED_1;
      distance = 1;
    }
    else if (key_value == '5') {
      menu_position = MAIN_SPEED_10;
      distance = 10;
    }
    else if (key_value == '6') {
      menu_position = MAIN_SPEED_SET;
      Serial.println("6번 선택됨");
    }
  }

  x_stop_value = digitalRead(X_STOP);

  // put your main code here, to run repeatedly:
  value_enc_a = digitalRead(ENC_A);
  value_enc_b = digitalRead(ENC_B);
  if (value_enc_a == 0 && value_enc_b == 1) {
    curr_enc_state = 1;
  }
  else if (value_enc_a == 0 && value_enc_b == 0) {
    curr_enc_state = 2;
  }
  else if (value_enc_a == 1 && value_enc_b == 0) {
    curr_enc_state = 3;
  }
  else if (value_enc_a == 1 && value_enc_b == 1) {
    curr_enc_state = 4;

    right_flag = 0;
    left_flag = 0;
  }
  if (curr_enc_state == ENC_STATE_1 && prev_enc_state == ENC_STATE_2) {
    if (left_flag == 0) {
      left_flag = 1;
      //Serial.println(--count);
      if (menu_position == MAIN_LEFT_STEP) {
        direction_x(DIR_LEFT);
        num_of_step_count = (int)(distance * 80);
        run_stop = RUN;
      }
      else if (menu_position == MAIN_SPEED_SET) {
        distance -= 0.2;
        Serial.println("distance = " + String(distance));
      }
    }
  }
  if (curr_enc_state == ENC_STATE_3 && prev_enc_state == ENC_STATE_2) {
    if (right_flag == 0) {
      right_flag = 1;
      //Serial.println(++count);
      if (menu_position == MAIN_RIGHT_STEP) {
        direction_x(DIR_RIGHT);
        num_of_step_count = (int)(distance * 80);
        run_stop = RUN;
      }
      else if (menu_position == MAIN_SPEED_SET) {
        distance += 0.2;
        Serial.println("distance = " + String(distance));
      }
    }
  }

  prev_enc_state = curr_enc_state;
  delay(1);
}
SIGNAL(TIMER2_COMPA_vect) {
  if (x_stop_value == RUN && run_stop == RUN) {

    if (toggle_flag_x == 0)
    {
      toggle_flag_x = 1;
      digitalWrite(X_STEP, HIGH);
    }
    else
    {
      toggle_flag_x = 0;
      digitalWrite(X_STEP, LOW);

      step_count++;

      if (step_count >= num_of_step_count) {
        step_count = 0;
        run_stop = STOP;
      }

      //run_stop = STOP;
    }
  }
}



//  //Serial.print(value_enc_a);
//  if(enc_state == 4 && check !=0){
//    Serial.print(count);
//    if(check == LEFT){
//    Serial.println("  LEFT");
//    count--;
//    }
//    else if(check == RIGHT){
//    count++;
//    Serial.println("  RIGHT");
//    }
//
//    check = STOP;
//  }
