unsigned long pre_micros = 0;
unsigned long pre_micros1 = 0;

unsigned long pre_millis = 0;
unsigned long pre_millis1 = 0;
unsigned long curr_micros = 0;

unsigned long pm = 0;
unsigned long pm1 = 0;


int count =0;
int count1 =0;
int con = 0;
int con1 = 0;
int duty =1;
int duty1 =1;
void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

   curr_micros = micros();
  if(curr_micros - pre_micros > 100){
    pre_micros = curr_micros;

    count++;
    count1++;

    if(count1 == 100){
      count1 = 0;
      digitalWrite(12,HIGH);
    }
    else if(count1 == duty1){
      digitalWrite(12,LOW);      
    }
    
    if(count ==100){
      count =0;
      digitalWrite(13,HIGH);
    }
    else if(count == duty){
      digitalWrite(13,LOW);
    }
  }
  
  if(curr_micros - pm>100){
    pm = curr_micros;
    con++;
    }
    if(con == 100){
    con = 0;
    duty++;
    if(duty == 99)duty = 1; 
    }

  if(curr_micros - pm1 > 200){
    pm1 = curr_micros;
    con1++;
    if(con1 == 100){
      con1 = 0;
      duty1++;
      if(duty1 ==99)duty1 = 1;
    }
  }
  }
