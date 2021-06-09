#include<stdarg.h>

#define LCD_BACKLIGHT 14

#define RS 3
#define RW 4
#define E 5
#define DB0 6
#define DB1 7
#define DB2 8
#define DB3 9
#define DB4 10
#define DB5 11
#define DB6 12
#define DB7 13

char buf[50];

int count = 0;
enum charLCD_LINE {
  LINE_1 = 1,
  LINE_2,
  LINE_3,
  LINE_4
};
enum {
  DISABLE = 0,
  ENABLE
};
char *lcd_menu[] = {
  ">1.PWM BL PWM   ",
  " 2.PWM BL On/Off",
  " 3.Servo Motor  ",
  " 4.ADC          "
};
char toggle_flag_line1 = 0;
char toggle_flag_line2 = 0;
char backlight_pwm_enable = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(LCD_BACKLIGHT, OUTPUT);
  digitalWrite(LCD_BACKLIGHT, HIGH);
  for (int i = 0; i < 11; i++) {
    pinMode(RS + i, OUTPUT);
  }
  digitalWrite(RS, HIGH);
  digitalWrite(RW, LOW);
  //-----------------------------------
  charLCD_inst_8(0x30);
  charLCD_inst_8(0x30);
  charLCD_inst_8(0x30);
  charLCD_inst_8(0x38);
  charLCD_inst_8(0x06);
  charLCD_inst_8(0x0C);
  charLCD_inst_8(0x80);
  charLCD_inst_8(0x01);
  delay(2);


  //----------------------------------
  // charLCD_string_8("hello world");

  //charLCD_inst_8(0xC0);

  cursor_set(LINE_1, 0);
  printf_LCD("%s", lcd_menu[0]);
  cursor_set(LINE_2, 0);
  printf_LCD("%s", lcd_menu[1]);
}

char line_pos = LINE_1;
char menu_pos = LINE_1;
char enter_pos = LINE_1;
unsigned long pre_micros = 0;
unsigned long cur_micros = 0;
unsigned long pre_millis = 0;
unsigned long cur_millis = 0;
int pwm_count = 0;
int pwm_duty = 1;
void loop() {
  cur_micros = micros();
  cur_millis = millis();
  if (backlight_pwm_enable == 1) {
    if (cur_micros - pre_micros > 100)
    {
      pre_micros = cur_micros;

      pwm_count++;
      if (pwm_count == 100) {
        digitalWrite(LCD_BACKLIGHT, HIGH);
        pwm_count = 0;
      }
      else if (pwm_count == pwm_duty) {
        digitalWrite(LCD_BACKLIGHT, LOW);
      }
    }
    if (cur_millis - pre_millis > 50) {
      pre_millis = cur_millis;
      pwm_duty++;
      if (pwm_duty == 99) {
        pwm_duty = 1;
      }
    }
  }
// ==============B/L 켜기================== 
  if (Serial.available()) {
    char in_key = Serial.read();
    if (in_key == 'w')
    {
      if (line_pos == LINE_2) {
        line_pos = LINE_1;
        cursor_set(LINE_2, 0);
        printf_LCD(" ");
        cursor_set(LINE_1, 0);
        printf_LCD(">");
      }
      else {
        menu_pos--;
        if (menu_pos > 0) {
          for (int i = 0; i < 2; i++) {
            cursor_set(LINE_1 + i, 0);
            printf_LCD("%s", lcd_menu[(menu_pos - 1) + i]);
          }
        }
        else {
          menu_pos = 1;
        }
      }
      cursor_set(LINE_1, 0);
      printf_LCD(">");
      //Serial.println("UP");
    }
    else if (in_key == 's')
    {
      if (line_pos == LINE_1) {
        line_pos = LINE_2;
        cursor_set(LINE_1, 0);
        printf_LCD(" ");
        cursor_set(LINE_2, 0);
        printf_LCD(">");
      }
      else
      {
        menu_pos++;
        if (menu_pos < 4) {
          for (int i = 0; i < 2; i++) {
            cursor_set(LINE_1 + i, 0);
            printf_LCD("%s", lcd_menu[(menu_pos - 1) + i]);
          }
          cursor_set(LINE_2, 0);
          printf_LCD(">");
        }
        else {
          menu_pos = 3;
        }
      }
      //      Serial.println("DOWN");
    }
    else if (in_key == ' ') {
      enter_pos = line_pos + menu_pos - 1;

      if (enter_pos == LINE_1) {
        if (toggle_flag_line1 == 0) {
          toggle_flag_line1 = 1;
          backlight_pwm_enable = 1;
        }
        else
        {
          toggle_flag_line1 = 0;
          backlight_pwm_enable = 0;
        }
          Serial.println("1");
      }
      else if (enter_pos == LINE_2) {
         if (enter_pos == LINE_2) {
        if (toggle_flag_line2 == 0) {
          toggle_flag_line2 = 1;
          backlight_pwm_enable = DISABLE;
          digitalWrite(LCD_BACKLIGHT,LOW);
        }
        else
        {
          toggle_flag_line2 = 0;
          backlight_pwm_enable = DISABLE;
          digitalWrite(LCD_BACKLIGHT,HIGH);
        }
        Serial.println("2");
      }
      }
      else if (enter_pos == LINE_3) {
        Serial.println("3");
      }
      else if (enter_pos == LINE_4) {
        Serial.println("4");
      }
    }

    // delay(2);
  }
  // put your main code here, to run repeatedly
  //
  //  printf_LCD("count = %d", count++);
  //  //  sprintf(buf,"count : %d",count++);
  //  //  charLCD_string_8(buf);
  //  delay(1000);
}

void cursor_set(char line, char column) {
  if (line == LINE_1) {
    charLCD_inst_8(0x80 | column);
  }
  else if (line == LINE_2) {
    charLCD_inst_8(0xC0 | column);
  }
}
void charLCD_inst_8(unsigned char inst) {
  //Inst : 0x30
  digitalWrite(RS, LOW);

  for (int i = 0; i < 8; i++) {
    if (inst & 0x01 << i ) {
      digitalWrite(DB0 + i, HIGH);
    }
    else {
      digitalWrite(DB0 + i, LOW);
    }
  }
  digitalWrite(E, HIGH);
  digitalWrite(E, LOW);
  digitalWrite(RS, HIGH);

  delayMicroseconds(50);
}

void charLCD_data_8(unsigned char data) {
  //Inst : 0x30

  for (int i = 0; i < 8; i++) {
    if (data & 0x01 << i ) {
      digitalWrite(DB0 + i, HIGH);
    }
    else {
      digitalWrite(DB0 + i, LOW);
    }
  }
  digitalWrite(E, HIGH);
  digitalWrite(E, LOW);


  delayMicroseconds(50);
}

void charLCD_string_8(char *str) {
  for (int i = 0; i < strlen(str); i++) {
    charLCD_data_8(*(str + i));
    //    charLCD_data_8(str[i]);
  }
}
void printf_LCD(char *format, ...)
{
  char buf[512];
  va_list arglist;

  va_start(arglist, format);
  vsprintf(buf, format, arglist);
  va_end(arglist);
  charLCD_string_8(buf);

}
