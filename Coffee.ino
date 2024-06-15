// Shell
#include "Shell.hpp"
int running = 0;          // Указатель на строчку, которая сейчас выполняется
char *line;               // Указатель на строку ввода
char **args;              // Указатель на указатель на аргументы
int vars[26];             // Переменные (a-z)
char program[MAX_SIZE];   // Указатель на буфер исходника программы

// JSON
#include "ArduinoJson.h"

// RELAY
#define RELAY 28
bool is_RELAY = false;

// IR remote control
#include <IRremote.hpp>
#define RECV_PIN 7
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0;

// Traffic LED
#define TRAFF_RED 6
#define TRAFF_YELLOW 5
#define TRAFF_GREEN 4
bool is_TRAFF_RED = false;
//bool is_TRAFF_YELLOW = false;
//bool is_TRAFF_GREEN = false;

// TRACK LED
#define TRACK_LED 3
//bool is_TRACK_LED = false;

// ACTIVE BUZZER
#define A_BUZZER 2
void do_beep(int pinBeep);

// Fun Motor
#define FUN_IN 22
bool is_FUN_IN = false;

// Joystick
#define pinY A15       
#define pinX A14       

// Servo
#include <Servo.h>
#define SERVO_Y 9
#define SERVO_X 8
Servo servo_Y;
Servo servo_X;
int position_Y = 0;
int position_X = 0;

// Track Motor
#define TRACK_IN 24
bool is_TRACK_IN = false;

// Pump Motor
#define PUMP_IN 26
bool is_PUMP_IN = false;

// 4-Digit
#include "KETM1650.h"
int item = 0;           //display value
#define DIO 11
#define CLK 10
KETM1650 tm_4display(CLK, DIO);
bool is_TEMP = false;
bool is_HUM = false;
bool is_PHOTORESISTOR = false;
bool is_SERVO = false;

// Sonic
/*#define PIN_TRIG 25
#define PIN_ECHO 23
long duration; 
long cm;
*/


void setup(){
  // RELAY
  digitalWrite(RELAY, LOW);     
  pinMode(RELAY, OUTPUT);

  // Traffic LED
  pinMode(TRAFF_RED, OUTPUT);
  pinMode(TRAFF_YELLOW, OUTPUT); 
  pinMode(TRAFF_GREEN, OUTPUT); 

  // TRACK LED
  pinMode(TRACK_LED, OUTPUT);

  // ACTIVE BUZZER
  pinMode(A_BUZZER, OUTPUT);

  // Fun Motor
  pinMode(FUN_IN, OUTPUT);

  // Joystick
  pinMode(pinX, INPUT); 
  pinMode(pinY, INPUT); 

  // Servo
  pinMode(SERVO_Y, OUTPUT);             
  servo_Y.attach(SERVO_Y);             
  position_Y = 0;
  servo_Y.write(position_Y);
  delay(1000);

  pinMode(SERVO_X, OUTPUT);               
  servo_X.attach(SERVO_X);              
  position_X = 0;
  servo_X.write(position_X);
  delay(1000);

  // Track Motor
  pinMode(TRACK_IN, OUTPUT);
  digitalWrite(TRACK_IN, LOW);

  // Pump Motor
  pinMode(PUMP_IN, OUTPUT);
  digitalWrite(PUMP_IN, LOW);

  // 4-digital Tube
  //tm1637.init();
  //tm1637.set(BRIGHT_TYPICAL);   //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;

  // Serial
  Serial.begin(9600);
  Serial2.begin(9600);

  // IR remote control
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);    // IR Control Enable

  // 4-Digit
//  item = 8888; 
//  tm_4display.displayString(item);

  //Serial2.println("Ready.");
  Serial.println("Ready.");
  do_beep();
}


void loop(){
  if (IrReceiver.decode()) {
    // 0xBB44FF00 - Left
    // 0xBF40FF00 - OK
    // 0xBC43FF00 - Right
    // 0xE916FF00 - 1
    // 0xE619FF00 - 2
    // 0xF20DFF00 - 3
    // 0xF30CFF00 - 4
    // 0xE718FF00 - 5
    // 0xA15EFF00 - 6
    // 0xF708FF00 - 7
    // 0xE31CFF00 - 8
    // 0xA55AFF00 - 9
    //Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX); // Print "old" raw data
    /*if (IrReceiver.decodedIRData.decodedRawData == 0){
      IrReceiver.decodedIRData.decodedRawData = key_value;
    }*/
    
    switch(IrReceiver.decodedIRData.decodedRawData){
      case 0xBB44FF00:
        Serial.println("Left");
        do_beep();
        position_Y += 10;
        if(position_Y > 170) position_Y = 170;
        servo_Y.write(position_Y);
        break;

      case 0xBF40FF00:
        Serial.println("OK");
        do_beep();
        break;

      case 0xBC43FF00:
        Serial.println("Right");
        do_beep();
        position_Y -= 10;
        if(position_Y < 0) position_Y = 0;
        servo_Y.write(position_Y);
        break;

      case 0xE916FF00:
        Serial.println("1");
        do_beep();
        if(!is_TRAFF_RED){
          digitalWrite(TRAFF_RED, HIGH);
          is_TRAFF_RED = !is_TRAFF_RED;
        }
        else{
         digitalWrite(TRAFF_RED, LOW);
         is_TRAFF_RED = !is_TRAFF_RED; 
        }
        break;

      case 0xE619FF00:
        Serial.println("2");
        do_beep();
        break;

      case 0xF20DFF00:
        Serial.println("3");
        do_beep();
        break;

      case 0xF30CFF00:
        Serial.println("4");
        do_beep();
        break;

      case 0xE718FF00:
        Serial.println("5");
        do_beep();
        if(!is_FUN_IN){
          digitalWrite(FUN_IN, HIGH);
          digitalWrite(TRAFF_YELLOW, HIGH);
          is_FUN_IN = !is_FUN_IN;
        }
        else{
         digitalWrite(FUN_IN, LOW);
         digitalWrite(TRAFF_YELLOW, LOW);
         is_FUN_IN = !is_FUN_IN; 
        }
        break;

      case 0xA15EFF00:
        Serial.println("6");
        do_beep();
        if(!is_TRACK_IN){
          digitalWrite(TRACK_IN, HIGH);
          digitalWrite(TRACK_LED, HIGH);
          is_TRACK_IN = !is_TRACK_IN;
        }
        else{
         digitalWrite(TRACK_IN, LOW);
         digitalWrite(TRACK_LED, LOW);
         is_TRACK_IN = !is_TRACK_IN; 
        }
        break;

      case 0xF708FF00:
        Serial.println("7");
        do_beep();
        if(!is_PUMP_IN){
          digitalWrite(PUMP_IN, HIGH);
          digitalWrite(TRAFF_GREEN, HIGH);
          is_PUMP_IN = !is_PUMP_IN;
        }
        else{
         digitalWrite(PUMP_IN, LOW);
         digitalWrite(TRAFF_GREEN, LOW);
         is_PUMP_IN = !is_PUMP_IN; 
        }
        break;

      case 0xE31CFF00:
        Serial.println("8");
        do_beep();
        break;

      case 0xA55AFF00:
        Serial.println("9");
        do_beep();
        break;
    }
    //key_value = IrReceiver.decodedIRData.decodedRawData;
    /* USE NEW 3.x FUNCTIONS */
    //IrReceiver.printIRResultShort(&Serial); // Print complete received data in one line
    //IrReceiver.printIRSendUsage(&Serial); // Print the statement required to send this data
    IrReceiver.resume(); // Enable receiving of the next value
    delay(300);
  }

/*  if(is_TRACK_IN){
    // Сначала генерируем короткий импульс длительностью 2-5 микросекунд.
    digitalWrite(PIN_TRIG, LOW);
    delayMicroseconds(5);
    digitalWrite(PIN_TRIG, HIGH);
    // Выставив высокий уровень сигнала, ждем около 10 микросекунд. В этот момент датчик будет посылать сигналы с частотой 40 КГц.
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);
    //  Время задержки акустического сигнала на эхолокаторе.
    duration = pulseIn(PIN_ECHO, HIGH);
    // Теперь осталось преобразовать время в расстояние
    cm = (duration / 2) / 29.1;
    Serial.print("Расстояние до объекта: ");
    Serial.print(cm);
    Serial.println(" см.");
    delay(250);
  }*/

  int X = analogRead(pinX);      // считываем значение оси X
  int Y = analogRead(pinY);      // считываем значение оси Y
   
  if (Y < 450) {
    position_Y -= 1;
    if(position_Y < 0) position_Y = 0;
    servo_Y.write(position_Y);
    delay(10);
  }

  if (Y > 600) {  
    position_Y += 1;
    if(position_Y > 170) position_Y = 170;
    servo_Y.write(position_Y);
    delay(10);
  }
 
  if (X > 600) {
    position_X += 1;
    if(position_X > 170) position_X = 170;
    servo_X.write(position_X);
    delay(10);
  }
  
  if (X < 450) { 
    position_X -= 1;
    if(position_X < 0) position_X = 0;
    servo_X.write(position_X);
    delay(10);
  }

  // Shell
  shell();
}


// Active Buzzer
void do_beep(){
  digitalWrite(A_BUZZER, HIGH);
  delay(100);
  digitalWrite(A_BUZZER, LOW);
}