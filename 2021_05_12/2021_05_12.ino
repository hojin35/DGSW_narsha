#include<stdarg.h>

#define CS      28
#define SCK     30
#define MOSI    17

#define ENC_A   10
#define ENC_B   11

#define ENC_BTN 16

#define HOT_BED 12
#define BED_TEMP A6 //D25
enum line_number {
  LINE_1 = 1,
  LINE_2,
  LINE_3,
  LINE_4
};

enum {
  HOME = 4,
  BACK
};

enum {
  ENC_STATE_1 = 1,
  ENC_STATE_2,
  ENC_STATE_3,
  ENC_STATE_4
};
enum curr_menu {
  MAIN_MENU = 0,
  SUB_MENU_1,
  SUB_MENU_2,
  SUB_MENU_3,
  SUB_MENU_4,
  SUB_MENU_5,
  SUB_MENU_6
};

char current_menu = MAIN_MENU;

void printf_(char *format, ...)
{
  char buf[512];
  va_list arglist;

  va_start(arglist, format);
  vsprintf(buf, format, arglist);
  va_end(arglist);
  gLCD_string(buf);
}

char cursor_pos = 0;
char window_pos = 0;
int enter_pos = 0;
char enc_state = 0;
char enc_state_prev = 0;
char left_flag = 0;
char right_flag = 0;
char enc_flag = 0;


int count = 0;
char buf[50];



void shift_8bit(unsigned char data)
{
  for (int i = 0; i < 8; i++) {
    if (data & (0x80 >> i))
    {
      digitalWrite(MOSI, HIGH);
    }
    else
    {
      digitalWrite(MOSI, LOW);
    }
    digitalWrite(SCK, HIGH);
    digitalWrite(SCK, LOW);
  }
}

void gLCD_inst(unsigned char inst)
{
  unsigned char upper_byte = (inst & 0xf0);
  unsigned char lower_byte = (inst & 0x0f) << 4;

  digitalWrite(CS, HIGH);

  shift_8bit(0xf8);
  shift_8bit(upper_byte);
  shift_8bit(lower_byte);

  digitalWrite(CS, LOW);

  delayMicroseconds(100);
}
void gLCD_data(unsigned char data)
{
  unsigned char upper_byte = (data & 0xf0);
  unsigned char lower_byte = (data & 0x0f) << 4;

  digitalWrite(CS, HIGH);

  shift_8bit(0xFA);
  shift_8bit(upper_byte);
  shift_8bit(lower_byte);

  digitalWrite(CS, LOW);

  delayMicroseconds(100);
}
void gLCD_init() {
  gLCD_inst(0x30);
  gLCD_inst(0x30);
  gLCD_inst(0x30);
  gLCD_inst(0x38);
  gLCD_inst(0x06);
  gLCD_inst(0x0c);
  gLCD_inst(0x80);
  gLCD_inst(0x01);
  delay(2);
}
char *main_menu[] = {
  " 1.Step Motor1  ",
  " 2.Step Motor2  ",
  " 3.Step Motor3  ",
  " 4.Step Motor4  ",
  " 5.Step Motor5  ",
  " 6.Step Motor6  "
};
char *sub_menu[2][6] = {{
    " 1.Bed Temp     ",
    " 2.sub_menu_1   ",
    " 3.sub_menu_1   ",
    " 4.sub_menu_1   ",
    " 5.Home         ",
    " 6.Back         "
  },
  {
    " 1.sub_menu_2   ",
    " 2.sub_menu_2   ",
    " 3.sub_menu_2   ",
    " 4.sub_menu_2   ",
    " 5.Home         ",
    " 6.Back         "
  }
};
void print_all_menu(char window_pos, char cursor_pos)
{
  for (int i = 0; i < 4; i++)
  {
    cursor_set(LINE_1 + i, 0);
    printf_("%s", main_menu[i + window_pos]);
  }

  cursor_set(LINE_1 + cursor_pos, 0);
  gLCD_data('>');
}

void print_sub_menu(char current_menu, char window_pos, char cursor_pos)
{
  for (int i = 0; i < 4; i++)
  {
    cursor_set(LINE_1 + i, 0);
    printf_("%s", sub_menu[current_menu - 1][i + window_pos]);
  }
  cursor_set(LINE_1 + cursor_pos, 0);
  gLCD_data('>');
}

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 50; i++) {
    buf[i] = 0;
  }
  // put your setup code here, to run once:
  pinMode(CS, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);
  //-------------------------
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);

  digitalWrite(ENC_A, HIGH);
  digitalWrite(ENC_B, HIGH);
  //-------------------------
  pinMode(HOT_BED, OUTPUT);
  digitalWrite(HOT_BED, LOW);
  //-------------------------
  gLCD_init();
  //-----------------------
  pinMode(ENC_BTN, INPUT);
  digitalWrite(ENC_BTN, HIGH);
  //------------------------
  print_all_menu(0, 0);
}

int temp_count = 0;
char bed_temp_enable = 0;
void loop() {
  enc_func();
  enc_button();

  if (bed_temp_enable == 1)
  {
    temp_count++;
    if (temp_count > 1000)
    {
      temp_count = 0;
      int hot_bed_value = analogRead(A6);

      if(hot_bed_value < 910)
      {
        digitalWrite(HOT_BED,LOW);
      }
      else
      {
        digitalWrite(HOT_BED,HIGH);
      }
      cursor_set(LINE_1, 0);
      printf_("Bed_temp = %d", hot_bed_value);
    }
  }
  delay(1);
}
//void loop() {
//  //  cursor_set(LINE_1, 0);
//  //  // put your main code here, to run repeatedly:
//  //  //  sprintf(buf,"count = %d\n",count++);
//  //  //  gLCD_string(buf);
//  //  printf_("count = %d", count++);
//  //  delay(1000);
//}

void enc_button() {
  int enc_btn = digitalRead(ENC_BTN);

  if (enc_btn == 0 && enc_flag == 0)
  {
    enc_flag = 1;
  }
  else if (enc_btn == 1 && enc_flag == 1)
  {
    enc_flag = 0;
    if (current_menu != MAIN_MENU && enter_pos == HOME)
    {
      current_menu = MAIN_MENU;
      enter_pos = 0;
      cursor_pos = 0;
      window_pos = 0;
      print_all_menu(window_pos, cursor_pos);
    }
    else if (current_menu != MAIN_MENU && enter_pos == BACK) {

    }
    else if (current_menu == SUB_MENU_1)
    {
      if (enter_pos == 0)
      {
        if (bed_temp_enable == 0)
        {
          clear_lcd();
          bed_temp_enable = 1;
          digitalWrite(HOT_BED, HIGH);
        }
        else if (bed_temp_enable == 1)
        {
          bed_temp_enable = 0;
          current_menu = MAIN_MENU;
          enter_pos = 0;
          cursor_pos = 0;
          window_pos = 0;
          print_all_menu(window_pos, cursor_pos);
        }
      }
    }
    else
    {
      current_menu = enter_pos + 1;
      enter_pos = 0;
      cursor_pos = 0;
      window_pos = 0;

      print_sub_menu(current_menu, window_pos, cursor_pos);
    }
  }

}
void clear_lcd()
{
  for (int i = 0; i < 4; i++)
  {
    cursor_set(LINE_1 + i, 0);
    printf_("                ");
  }
}
void enc_func() {
  int enc_a = digitalRead(ENC_A);
  int enc_b = digitalRead(ENC_B);

  if (enc_a == 0 && enc_b == 1)enc_state = ENC_STATE_1;
  else if (enc_a == 0 && enc_b == 0)enc_state = ENC_STATE_2;
  else if (enc_a == 1 && enc_b == 0)enc_state = ENC_STATE_3;
  else if (enc_a == 1 && enc_b == 1)
  {
    enc_state = ENC_STATE_4;
    left_flag = 0;
    right_flag = 0;
  }

  if (enc_state == ENC_STATE_3 && enc_state_prev == ENC_STATE_2)
  {
    if (right_flag == 0)
    {
      right_flag = 1;
      //Serial.println("R");

      //cursor_set(LINE_1, 0);
      //printf_("count = %3d", ++count);
      cursor_set(LINE_1 + cursor_pos, 0);
      gLCD_data(' ');

      cursor_pos++;
      if (cursor_pos < 4)
      {
        cursor_set(LINE_1 + cursor_pos, 0);
        gLCD_data('>');
      }
      else if (cursor_pos >= 4)
      {
        cursor_pos = 3;

        window_pos++;
        if (window_pos <= 2)
        {
          if (current_menu == MAIN_MENU)
          {
            print_all_menu(window_pos, cursor_pos);
          }
          else
          {
            print_sub_menu(current_menu, window_pos, cursor_pos);
          }
        }
        else
        {
          window_pos = 2;

          cursor_set(LINE_1 + cursor_pos, 0);
          gLCD_data('>');
        }
      }
      enter_pos = window_pos + cursor_pos;
    }
  }
  else if (enc_state == ENC_STATE_1 && enc_state_prev == ENC_STATE_2)
  {
    if (left_flag == 0)
    {
      left_flag = 1;
      //Serial.println("L");
      //cursor_set(LINE_1, 0);
      //printf_("count = %3d", --count);

      cursor_set(LINE_1 + cursor_pos, 0);
      gLCD_data(' ');

      cursor_pos--;
      if (cursor_pos >= 0)
      {
        cursor_set(LINE_1 + cursor_pos, 0);
        gLCD_data('>');
      }
      else if (cursor_pos < 0)
      {
        cursor_pos = 0;

        window_pos--;
        if (window_pos >= 0)
          print_all_menu(window_pos, cursor_pos);
        else
        {
          window_pos = 0;

          cursor_set(LINE_1 + cursor_pos, 0);
          gLCD_data('>');
        }
      }
      enter_pos = window_pos + cursor_pos;
    }
  }
  enc_state_prev = enc_state;
}
void gLCD_string(char *str)
{
  for (int i = 0; i < strlen(str); i++)
  {
    gLCD_data(*(str + i));
  }
}
void cursor_set(char line, char column)
{
  if (line == LINE_1)
  {
    gLCD_inst(0x80 + 0 + column); // C0 = 80
  }
  else if (line == LINE_2)
  {
    gLCD_inst(0x80 + 16 + column); // D0 = 90
  }
  else if (line == LINE_3)
  {
    gLCD_inst(0x80 + 8 + column);
  }
  else if (line == LINE_4)
  {
    gLCD_inst(0x80 + 24 + column);
  }
}
