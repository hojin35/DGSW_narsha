#define X_DIR      21
#define X_STOP     18
#define X_STEP     15

#define Y_DIR      23
#define Y_STOP     19
#define Y_STEP     22

#define XY_ENABLE  14
void setup() {
  // put your setup code here, to run once:
  pinMode(X_DIR, OUTPUT);
  pinMode(X_STOP, INPUT);
  pinMode(X_STEP, OUTPUT);

  pinMode(Y_DIR, OUTPUT);
  pinMode(Y_STOP, INPUT);
  pinMode(Y_STEP, OUTPUT);
  pinMode(XY_ENABLE, OUTPUT);

  digitalWrite(Y_DIR, HIGH);
  digitalWrite(X_DIR, HIGH);
  digitalWrite(XY_ENABLE, LOW);
}
unsigned long curr_micros = 0;
unsigned long pre_micros_x = 0;
unsigned long pre_micros_y = 0;

bool x_stopped = 0;
bool y_stopped = 0;

bool motor_stop_x = 0;
bool motor_stop_y = 1;


bool step_toggle_x = 0;
bool step_toggle_y = 0;
bool dir_toggle_x = 0;
bool dir_toggle_y = 0;

int curr_millis = 0;
int pre_millis_x = 0;
int pre_millis_y = 0;

unsigned int step_count_x = 0;
unsigned int step_count_y = 0;

int cnt = 0;
void loop() {


  curr_micros = micros();
  curr_millis = millis();

  //--------------------------------------
  if (curr_micros - pre_micros_x > 200) {
    pre_micros_x = curr_micros;

    if (motor_stop_x == 0) {
      if (step_toggle_x == 0) {
        step_toggle_x = 1;
        digitalWrite(X_STEP, HIGH);
      }
      else if (step_toggle_x == 1) {
        step_toggle_x = 0;
        digitalWrite(X_STEP, LOW);

        step_count_x++;
        if (step_count_x > 4000) {
          step_count_x = 0;
          motor_stop_x = 1;

          x_stopped = 1;
        }
      }
    }
    else if (motor_stop_x == 1) {
      digitalWrite(X_STEP, LOW);
    }
  }
  //--------------------------------------------
  if (curr_micros - pre_micros_y > 200) {
    pre_micros_y = curr_micros;
    if (motor_stop_y == 0) {
      if (step_toggle_y == 0) {
        step_toggle_y = 1;
        digitalWrite(Y_STEP, HIGH);
      }
      else if (step_toggle_y == 1) {
        step_count_y++;
        if (step_count_y > 4000) {
          step_count_y = 0;
          motor_stop_y = 1;

          y_stopped = 1;
        }
        step_toggle_y = 0;
        digitalWrite(Y_STEP, LOW);
      }
    }
    else if (motor_stop_y == 1) {
      digitalWrite(Y_STEP, LOW);
    }
  }
  //--------------------------------------------

  if (x_stopped == 1 && motor_stop_y == 1&&cnt == 0) {
    motor_stop_y = 0;
    step_count_y = 0;
  }
  else if (x_stopped == 1 && y_stopped == 1 && cnt == 0) {

    digitalWrite(X_DIR, LOW);
    digitalWrite(Y_DIR, LOW);
    motor_stop_x = 0;
    step_count_x = 0;

    motor_stop_y = 0;
    step_count_y = 0;

    cnt = 1;
  }


  //if(curr_millis - pre_millis_x >1000){
  //
  //  pre_millis_x = curr_millis;
  //
  //  if(dir_toggle_x == 0){
  //    dir_toggle_x = 1;
  //    digitalWrite(X_DIR,LOW);
  //  }
  //  else if(dir_toggle_x ==1){
  //    dir_toggle_x = 0;
  //    digitalWrite(X_DIR,HIGH);
  //  }
  //}
  //----------------------------------------------
  //  if (curr_millis - pre_millis_y > 1000) {
  //
  //    pre_millis_y = curr_millis;
  //
  //    if (dir_toggle_y == 0) {
  //      dir_toggle_y = 1;
  //      digitalWrite(Y_DIR, LOW);
  //    }
  //    else if (dir_toggle_y == 1) {
  //      dir_toggle_y = 0;
  //      digitalWrite(Y_DIR, HIGH);
  //    }
  //  }

  //--------------------------------------------
  int in_value_x = digitalRead(X_STOP);
  if (in_value_x == 1) {
    motor_stop_x = 1;
  }

  //--------------------------------------------
  int in_value_y = digitalRead(Y_STOP);
  if (in_value_y == 1) {
    motor_stop_y = 1;
  }
  //-------------------------------------


  // put your main code here, to run repeatedly:


  //  digitalWrite(X_STEP,HIGH);
  //    delayMicroseconds(200);
  //    digitalWrite(X_STEP,LOW);
  //    delayMicroseconds(200);
}




//void loop() {
//  // put your main code here, to run repeatedly:
//  int in_value = digitalRead(X_STOP);
//  if(in_value ==0){
//    digitalWrite(X_STEP,HIGH);
//    delayMicroseconds(200);
//    digitalWrite(X_STEP,LOW);
//    delayMicroseconds(200);
//  }
//}

//--------------------------------

//void loop() {
//
//  int in_value = digitalRead(X_STOP);
//
//  curr_micros = micros();
//  if (curr_micros - pre_micros > 200) {
//    pre_micros = curr_micros;
//    if (motor_stop == 0) {
//      if (step_toggle == 0) {
//        step_toggle = 1;
//        digitalWrite(X_STEP, HIGH);
//      }
//      else if (step_toggle == 1) {
//        step_toggle = 0;
//        digitalWrite(X_STEP, LOW);
//      }
//    }
//    else if (motor_stop == 1) {
//      digitalWrite(X_STEP, LOW);
//    }
//  }
//
//  if (in_value == 1) {
//    motor_stop = 1;
//  }

//--------------------------
