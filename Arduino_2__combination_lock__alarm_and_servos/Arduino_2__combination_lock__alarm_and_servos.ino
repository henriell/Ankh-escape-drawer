#include <Servo.h>
Servo servoUp;
Servo servoDown;
const int servoUp_pin = 10;
const int servoDown_pin = 11;

const int button_1 = 22;
const int button_2 = 23;
const int button_3 = 24;
const int button_4 = 25;
const int button_5 = 26;
const int button_6 = 27;
const int button_7 = 28;
const int button_8 = 29;
const int button_9 = 30;
const int button_0 = 31;
const int button_enter = 32;

const int ledPinRed = 33;
const int ledPinGreen = 34;

const int stjerne_1 = 35;
const int stjerne_2 = 36;
const int stjerne_3 = 37;
const int stjerne_4 = 38;
const int stjerner[] = {stjerne_1, stjerne_2, stjerne_3, stjerne_4,};

const int alarmLedPin = 5;
const int signal_Pin=50;
const int skuff_Pin = 51;


char rightCode[] = {'3','8','5','2','4','9','1','3','2','4','6','1'}; //3 kodekombinasjoner

int count = 0;
bool RightCode = false;


char inputCode[4];

int start_time;
int maxVerdi_alarmLed = 1023;

void setup() {
  Serial.begin(9600);
  for (int i = 22; i < 33; i++) {
    pinMode(i, INPUT);
  }

  for (int i = 35; i < 39; i++) {
    pinMode(i, OUTPUT);
  }

  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);
  servoUp.attach(servoUp_pin);
  servoDown.attach(servoDown_pin);

  pinMode(alarmLedPin, OUTPUT);
}

void loop() {

  count = 0;
  RightCode = false;

  for (int test = 0; test < 3; test++) {
    for(int i = 0; i < 4; i++)
      inputCode[i] = "0";
    servoUp.write(1);
    servoDown.write(4);

    digitalWrite(signal_Pin,LOW); //sender signal om at oppgaven ikke er løst

    while (digitalRead(skuff_Pin) == LOW) { // når skuffen er lukket, skjer ingenting
      //Serial.println('r'); // raspberry pi
      for (int i = 35; i < 39; i++) { //setter * til LOW
        digitalWrite(i, LOW);
      }
      digitalWrite(ledPinRed, LOW); 
      digitalWrite(ledPinGreen, LOW); 
      digitalWrite(alarmLedPin,LOW);
    }
    //skuffen åpen
    //Serial.println('g'); // raspberry pi
    start_time = millis();

    RightCode = false;
    while (!RightCode) {
      alarmLysing ();
      if (digitalRead(skuff_Pin) == LOW) {
        digitalWrite(ledPinRed, LOW);
        digitalWrite(ledPinGreen, LOW);
        digitalWrite(alarmLedPin, LOW);
        count = 0; // start på nytt
        break;
      }

    //input taster
      if (digitalRead(skuff_Pin) == LOW) {
        break;
      }
      if (digitalRead(button_1) == HIGH) {
        buttonPress('1',button_1);
        //Serial.print('1');

      }
      if (digitalRead(button_2) == HIGH) {
        buttonPress('2',button_2);
        //Serial.print('2');

      }
      if (digitalRead(button_3) == HIGH) {
        buttonPress('3',button_3);
        //Serial.print('3');

      }
      if (digitalRead(button_4) == HIGH) {
        buttonPress('4',button_4);
        //Serial.print('4');

      }
      if (digitalRead(button_5) == HIGH) {
        buttonPress('5',button_5);
        //Serial.print('5');

      }
      if (digitalRead(button_6) == HIGH) {
        buttonPress('6',button_6);
        //Serial.print('6');

      }
      if (digitalRead(button_7) == HIGH) {
        buttonPress('7',button_7);
        //Serial.print('7');

      }
      if (digitalRead(button_8) == HIGH) {
        buttonPress('8',button_8);
        //Serial.print('8');

      }
      if (digitalRead(button_9) == HIGH) {
        buttonPress('9',button_9);
        //Serial.print('9');

      }
      if (digitalRead(button_0) == HIGH) {
        buttonPress('0',button_0);
        //Serial.print('0');

      }
      if (digitalRead(button_enter) == HIGH) {
        if (count == 4) {
          RightCode = true;
          for (int i = 0; i < 4; i++) {
            if (inputCode[i] != rightCode[(test * 4) + i]) { //sjekk om koden er riktig
              RightCode = false;
              break;
            }
          }
        }
      
        if (!RightCode){
          count = 0;
          for (int i = 35; i < 39; i++) {
            digitalWrite(i, LOW);
          } 
          
          digitalWrite(ledPinRed, HIGH);
      
          delay(1000);
          digitalWrite(ledPinRed, LOW);
        }
        while (true) {
          if (digitalRead(button_enter) == LOW) {
            break;
          }
        }
      }
      if (digitalRead(skuff_Pin) == LOW) {
        break;
      }
    }
    if(RightCode){
      //Serial.println('r'); // raspberry pi
      digitalWrite(ledPinGreen, HIGH);
      digitalWrite(alarmLedPin,LOW);
      servoUp.write(80);
      servoDown.write(80);

      digitalWrite(signal_Pin,HIGH); // sender signal om at oppgaven er løst
    
      delay(1000);
      digitalWrite(ledPinGreen, LOW);
      
      for (int i= 0; i<4; i++){
         digitalWrite(stjerner[i],LOW);
      }
      count = 0;
      for (int i = 35; i < 39; i++) {
        digitalWrite(i, LOW);
      } 
      while (digitalRead(skuff_Pin) == HIGH){
        delay(10);
      }
      digitalWrite(alarmLedPin,LOW);    
    }
  }
}

void buttonPress(char button,int pin){
  if (count != 0)
    if( inputCode[count-1] == button)
      return;
  if (count >= 4)
    return;
  inputCode[count] = button;
  digitalWrite(stjerner[count],HIGH);
  count ++;

  while(true){
    if (digitalRead(pin)==LOW){
      break;
    }
  }
}

// alarm
void alarmLysing (){
   int now_time= millis(); //millisekunder siden start 
   int halvPeriode = 2000; // 1 sek
   int periodeVerdi = (now_time - start_time) % halvPeriode;
   analogWrite(alarmLedPin,(now_time - start_time) % halvPeriode);   
}
