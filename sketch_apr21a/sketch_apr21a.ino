#define X_STOP 18
#define X_DIR 21
#define X_STEP 15

#define Y_STOP 19
#define Y_DIR 23
#define Y_STEP 22

#define XY_EN 14
enum {
  STOP = 0,
  RUN
};
enum {
  RUNNING = 0,
  STOPPED
};
enum {
  DIR_RIGHT = 0,
  DIR_LEFT

};
enum {
  DIR_UP = 0,
  DIR_DOWN
};

enum {
  STATE_NONE = 0,
  STATE_RIGHT,
  STATE_LEFT,
  STATE_UP,
  STATE_DOWN,
  STATE_CROSS
};

char x_stopped = STOPPED;
char y_stopped = STOPPED;

char toggle_flag_x = 0;
int step_count_x = 0;
int how_many_counts_x = 0;
int distance_x = 0;
int stop_run_flag_x = STOP;

char toggle_flag_y = 0;
int step_count_y = 0;
int how_many_counts_y = 0;
int distance_y = 0;
int stop_run_flag_y = STOP;

int motor_state = STATE_RIGHT;

void x_direction(char dir) {
  if (dir == DIR_LEFT)digitalWrite(X_DIR, LOW);
  else if (dir == DIR_RIGHT)digitalWrite(X_DIR, HIGH);
}
void y_direction(char dir) {
  if (dir == DIR_UP)digitalWrite(Y_DIR, LOW);
  else if (dir == DIR_DOWN)digitalWrite(Y_DIR, HIGH);
}
void init_func() {
  distance_x = 50;
  how_many_counts_x = distance_x * 80;

  distance_y = 50;
  how_many_counts_y = distance_y * 80;

  x_stopped = RUNNING;
  y_stopped = RUNNING;

  motor_state = STATE_RIGHT;

  x_direction(DIR_RIGHT);
  y_direction(DIR_DOWN);

  stop_run_flag_x = RUN;
  stop_run_flag_y = STOP;
}

void loop() {
  if (motor_state == STATE_RIGHT) {

    if (x_stopped == STOPPED) {
      Serial.println("X Motor stopped !!");

      motor_state = STATE_DOWN;

      y_direction(DIR_DOWN);
      y_stopped = RUNNING;
      stop_run_flag_y = RUN;
    }
  }
  else if (motor_state == STATE_DOWN) {
    if (y_stopped == STOPPED)
    {
      Serial.println("Y Motor stopped !!");
      motor_state = STATE_LEFT;

      x_direction(DIR_LEFT);
      x_stopped = RUNNING;

      stop_run_flag_x = RUN;
    }
  }
  else if (motor_state == STATE_LEFT) {

    if (x_stopped == STOPPED) {
      Serial.println("X Motor stopped !!");

      motor_state = STATE_UP;

      y_direction(DIR_UP);
      y_stopped = RUNNING;
      stop_run_flag_y = RUN;
    }
  }
  else if (motor_state == STATE_UP) {
    if (y_stopped == STOPPED)
    {
      Serial.println("Y Motor stopped !!");
      motor_state = STATE_CROSS;

      x_direction(DIR_RIGHT);
      y_direction(DIR_DOWN);
      x_stopped = RUNNING;
      y_stopped = RUNNING;

      stop_run_flag_x = RUN;
      stop_run_flag_y = RUN;
    }
  }
  else if(motor_state == STATE_CROSS){
    if(y_stopped == STOPPED && x_stopped == STOPPED){
      Serial.println("ALL STOP");
      motor_state = STATE_NONE;
    }
  }

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //X축
  pinMode(X_STOP, INPUT);
  pinMode(X_DIR, OUTPUT);
  pinMode(X_STEP, OUTPUT);
  digitalWrite(X_DIR, LOW);

  //Y축
  pinMode(Y_STOP, INPUT);
  pinMode(Y_DIR, OUTPUT);
  pinMode(Y_STEP, OUTPUT);
  digitalWrite(Y_DIR, LOW);

  //Enable
  pinMode(XY_EN, OUTPUT);
  digitalWrite(XY_EN, LOW);

  TCCR2A = 0x02; //CTC모드
  TCCR2B = 0x04;
  TCNT2 = 0;
  OCR2A = 50;
  TIMSK2 = 0x02;

  init_func();
}

SIGNAL(TIMER2_COMPA_vect) {

  if (stop_run_flag_x == RUN) {

    if (toggle_flag_x == 0) {
      toggle_flag_x = 1;
      digitalWrite(X_STEP, toggle_flag_x);
    }
    else if (toggle_flag_x == 1) {
      toggle_flag_x = 0;
      digitalWrite(X_STEP, toggle_flag_x);

      step_count_x++;

      //--------------------------------------------
      if (step_count_x > how_many_counts_x) {
        step_count_x = 0;
        stop_run_flag_x = STOP;
        x_stopped = STOPPED;
        digitalWrite(X_DIR, DIR_RIGHT);
      }
    }
  }
  //============================================== X축
  if (stop_run_flag_y == RUN) {

    if (toggle_flag_y == 0) {
      toggle_flag_y = 1;
      digitalWrite(Y_STEP, toggle_flag_y);
    }
    else if (toggle_flag_y == 1) {
      toggle_flag_y = 0;
      digitalWrite(Y_STEP, toggle_flag_y);

      step_count_y++;

      //------------------------------------------------
      if (step_count_y > how_many_counts_y) {
        step_count_y = 0;
        stop_run_flag_y = STOP;
        y_stopped = STOPPED;
      }
    }
  }
  //================================================Y축
}
