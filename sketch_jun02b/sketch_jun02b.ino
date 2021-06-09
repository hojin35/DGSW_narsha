#define X_DIR 21
#define X_STEP 15
#define Y_DIR 23
#define Y_STEP 22

#define XY_EN 14

enum {
  RUNNING = 0,
  STOPPED
};
enum {
  DISABLE = 0,
  ENABLE
};
enum dir {
  LEFT = 0,
  RIGHT,
  UP,
  DOWN
};
char toggle_step_x = 0;
char toggle_step_y = 0;
int step_count_x = 0;
int step_count_y = 0;

char enable_motor_x = 0;
char enable_motor_y = 0;

int count_distance_x = 0;
int count_distance_y = 0;

int half_period_x = 0;
int half_period_y = 0;

char interrupt_stopped_x = STOPPED;
char interrupt_stopped_y = STOPPED;

void motor_direction(char dir)
{
  if (dir == LEFT)
  {
    digitalWrite(X_DIR, HIGH);
  }
  else if (dir == RIGHT)
  {
    digitalWrite(X_DIR, LOW);
  }
  else if (dir == UP)
  {
    digitalWrite(Y_DIR, LOW);
  }
  else if (dir == DOWN)
  {
    digitalWrite(Y_DIR, HIGH);
  }
}

void timer_x_enable()
{
  TIMSK1 = 0x02;
}
void timer_x_disable()
{
  TIMSK1 = 0x00;
}
void timer_y_enable()
{
  TIMSK3 = 0x02;
}
void timer_y_disable()
{
  TIMSK3 = 0x00;
}

char state = 0;
void motor_x_start(float distance, int step_period, char left_right)
{
  count_distance_x = (int)(distance * 80);
  half_period_x = step_period;
  OCR1A = half_period_x * 2;
  enable_motor_x = ENABLE;
  interrupt_stopped_x = RUNNING;

  motor_direction(left_right);
}
void motor_y_start(float distance, int step_period, char up_down)
{
  count_distance_y = (int)(distance * 80);
  half_period_y = step_period;
  OCR3A = half_period_y * 2;
  enable_motor_y = ENABLE;
  interrupt_stopped_y = RUNNING;

  motor_direction(up_down);
}
void motor_x_y_neable(char x_enable,char y_enable)
{
  if(x_enable == ENABLE)
  {
    timer_x_enable();
  }
  if(y_enable == ENABLE)
  {
    timer_y_enable();
  }
  enable_motor_x = x_enable;
  enable_motor_y = y_enable;
}
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(X_DIR, OUTPUT);
  pinMode(Y_DIR, OUTPUT);

  pinMode(X_STEP, OUTPUT);
  pinMode(Y_STEP, OUTPUT);

  pinMode(XY_EN, OUTPUT);

  digitalWrite(XY_EN, LOW);

  motor_direction(RIGHT);
  motor_direction(UP);
  //Timer 1 : X motor
  TCCR1A = 0x00;
  TCCR1B = 0x0A;
  TCCR1C = 0x00;
  TCNT1 = 0x00;
  OCR1A = 400;
  TIMSK1 = 0x02;

  //Timer 3 : Y motor
  TCCR3A = 0x00;
  TCCR3B = 0x0A;
  TCCR3C = 0x00;
  TCNT3 = 0x00;
  OCR3A = 400;
  TIMSK3 = 0x02;

  enable_motor_x = ENABLE;
  enable_motor_y = ENABLE;

  count_distance_x = (int)(43.3 * 80);
  count_distance_y = (int)(25 * 80);

  half_period_x = 200;
  half_period_y = (int)(200 * 1.732);

  OCR1A = half_period_x * 2;
  OCR3A = half_period_y * 2;

  interrupt_stopped_x = RUNNING;
  interrupt_stopped_y = RUNNING;

  state = 1;
}



void loop()
{
  if (state == 1)
  {
    if (interrupt_stopped_x == STOPPED)
    {
      //timer_x_disable();
    }
    if (interrupt_stopped_y == STOPPED)
    {
      //timer_y_disble();
    }
    if (interrupt_stopped_x == STOPPED && interrupt_stopped_y == STOPPED)
    {
      state = 2;

      count_distance_x = 0;
      count_distance_y = (int)(25 * 80);

      half_period_x = 0;
      half_period_y = (int)(200 * 1.732);
      //(int)(200 * 1.732)
      OCR1A = half_period_x * 2;
      OCR3A = half_period_y * 2;

      enable_motor_x = DISABLE;
      enable_motor_y = ENABLE;

      interrupt_stopped_x = STOPPED;
      interrupt_stopped_y = RUNNING;


      motor_direction(DOWN);

      timer_y_enable();
      enable_motor_x = DISABLE;
      enable_motor_y = ENABLE;
    }

  }
  else if (state == 2)
  {
    if (interrupt_stopped_x == STOPPED)
    {
      timer_x_disable();
    }
    if (interrupt_stopped_y == STOPPED)
    {
      timer_y_disable();
    }
    if (interrupt_stopped_x == STOPPED && interrupt_stopped_y == STOPPED)
    {
      state = 3;


      timer_x_enable();
      enable_motor_x = ENABLE;
      enable_motor_y = DISABLE;
    }
  }
  else if (state == 3)
  {
    if (interrupt_stopped_x == STOPPED)
    {
      timer_x_disable();
    }
    if (interrupt_stopped_y == STOPPED)
    {
      timer_y_disable();
    }
  }
}


SIGNAL(TIMER1_COMPA_vect)
{
  if (enable_motor_x == ENABLE) {
    if (toggle_step_x == 0)
    {
      toggle_step_x = 1;

      digitalWrite(X_STEP, HIGH);
      digitalWrite(Y_STEP, HIGH);
    }
    else
    {
      toggle_step_x = 0;
      step_count_x++;
      if (step_count_x > count_distance_x)
      {
        step_count_x = 0;
        digitalWrite(X_STEP, LOW);
        enable_motor_x = DISABLE;
        interrupt_stopped_x = STOPPED;
      }
      else
      {
        digitalWrite(X_STEP, LOW);

      }
    }
  }
}
SIGNAL(TIMER3_COMPA_vect)
{
  if (enable_motor_y == ENABLE) {
    if (toggle_step_y == 0)
    {
      toggle_step_y = 1;

      digitalWrite(Y_STEP, HIGH);
    }
    else
    {
      toggle_step_y = 0;
      step_count_y++;
      if (step_count_y > count_distance_y)
      {
        step_count_y = 0;
        digitalWrite(Y_STEP, LOW);
        enable_motor_y = DISABLE;
        interrupt_stopped_y = STOPPED;
      }
      else
      {
        digitalWrite(Y_STEP, LOW);
      }
    }
  }
}
