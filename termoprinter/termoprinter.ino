#define DI 5
#define CLK 4
#define LATCH 7
#define STB1 8
#define STB2 9
#define STEP 3
#define DIR 2
#define N_SLEEP_STEPER 10
#define TEMP 3000
#define NUM_CHAR_IN_LINE 24
//#define DEBUG
#include "glcdfont.c"
uint8_t liter[1024] = {'П', 'р', 'и', 'в', 'е', 'т', ' ', 'М', 'И', 'Р', '!'};
void newLine()
{
  for (int i = 0; i < 5; i++) {
    digitalWrite(STEP, HIGH);
    delay(1);
    digitalWrite(STEP, LOW);
    delay(1);
  }
}
void printLine(uint8_t data[16])
{
  digitalWrite(LATCH, HIGH);
#ifdef   DEBUG
  Serial.println();
#endif
  for (int i = 0; i < 128; i++) {

    if ((data[i >> 3] & (1 << (i & 7))))
    {
#ifdef   DEBUG
      Serial.print(8);
#endif
      digitalWrite(DI, HIGH);
    }
    else
    {
#ifdef   DEBUG
      Serial.print(" ");
#endif
      digitalWrite(DI, LOW);
    }
    digitalWrite(CLK, HIGH);
    //delay(1);
    digitalWrite(CLK, LOW);
    //delay(1);
  }

  digitalWrite(LATCH, LOW);
  //delay(1);
  digitalWrite(STB1, LOW);
  delayMicroseconds(TEMP);
  digitalWrite(STB1, HIGH);
  //delay(1);
  digitalWrite(STB2, LOW);
  delayMicroseconds(TEMP);
  digitalWrite(STB2, HIGH);
  //delay(1);
  for (int i = 0; i < 5; i++) {
    digitalWrite(STEP, HIGH);
    delay(1);
    digitalWrite(STEP, LOW);
    delay(1);
  }
}
void printChars(uint8_t *dat, uint16_t len)
{
  uint8_t dotArrey[16];
  uint16_t count = 0;
  uint8_t line [NUM_CHAR_IN_LINE];
  uint8_t endLineCounter = 0;
  uint8_t startLineCounter = 0;

  digitalWrite(N_SLEEP_STEPER,HIGH);
  delay(1);
  while (count < len)
  {
    endLineCounter = 0;
    for (int i = 0; i < NUM_CHAR_IN_LINE;i++)
    {
      line[i] = 0;
    }
    for (int i = 0; i < NUM_CHAR_IN_LINE; i++)
    {
      endLineCounter++;
      if(dat[i + count] == '\r') 
      {
      //  endLineCounter++;
       // startLineCounter = 1;
        break;
      }
      if(dat[i + count] == '\n') count++;
      if((i+count)==len)break;

      
      line[i] = dat[i + count];
    }
    for ( int u = 0; u < 7; u++)         // u - vertical
    {
      uint8_t count_dot = 0;
      for (int r = 0; r < 16; r++)
      {
        dotArrey[r] = 0;
      }
      // Serial.println();
      for (int n = startLineCounter; n < endLineCounter ; n++)           // n - #char
      {

        for ( int m = 0; m < 5; m++)        // m - horizontal
        {
          // dotArrey[count_dot>>3] |= (pgm_read_byte(font+line[n]*5+m))&(1<<u);
          if ((pgm_read_byte(font + line[n] * 5 + m)) & (1 << u)) dotArrey[15 - (count_dot >> 3)] |= 1 << (7 - count_dot & 7);
          // else Serial.print(" ");
          count_dot++;
        }
      }

      printLine(dotArrey);

    }
    newLine();
    count += endLineCounter;
  }
  digitalWrite(N_SLEEP_STEPER,LOW);
}
void setup() {
  // put your setup code here, to run once:
//  uint8_t preMes[4][26] = {{1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0},
//    {0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0}, \
//    {1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1}, \
//    {1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1}
//  };





  pinMode(STEP, OUTPUT);
  pinMode(N_SLEEP_STEPER, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(STB1, OUTPUT);
  pinMode(STB2, OUTPUT);
  digitalWrite(STB1, HIGH);
  digitalWrite(STB2, HIGH);
  pinMode(LATCH, OUTPUT);
  pinMode(DI, OUTPUT);
  // pinMode(DO,INPUT);
  pinMode(CLK, OUTPUT);
  Serial.begin(19200);
  digitalWrite(DIR, HIGH);
  digitalWrite(STEP, LOW);
  digitalWrite(DI, LOW);
  digitalWrite(N_SLEEP_STEPER,LOW);

}
uint32_t timer;
void loop() {
  // put your main code here, to run repeatedly:
  static uint16_t count = 0;

  
 // while(!Serial.available());
  while(Serial.available())
  {
   // timer = millis();
    liter[count] = Serial.read();
    count++;
    for(int i = 0; i<500;i++)
    {
      if(Serial.available()) break;
      delay(1);
    }
  }
  if (count)
  {
   
   Serial.println(count);
  // for(int i = 0; i< count; i++) Serial.write(liter[i]);
   
  for (int i = 0; i < 50; i++) {
    digitalWrite(STEP, HIGH);
    delay(1);
    digitalWrite(STEP, LOW);
    delay(1);
  }


  printChars(liter, count);
  count = 0;
  for (int i = 0; i < 50; i++) {
    digitalWrite(STEP, HIGH);
    delay(1);
    digitalWrite(STEP, LOW);
    delay(1);
  } 

  }
}
