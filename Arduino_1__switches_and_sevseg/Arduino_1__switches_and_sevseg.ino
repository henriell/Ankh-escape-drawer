echo "# Ankh-escape-drawer" >> README.md
git init
git add README.md
git commit -m "first commit"
git remote add origin https://github.com/henriell/Ankh-escape-drawer.git
git push -u origin master




#include "SevSeg.h"
//Bibliotek for 2 dig SevSeg
char tall_0[] = "0000001";
char tall_1[] = "1001111";
char tall_2[] = "0010010";
char tall_3[] = "0000110";
char tall_4[] = "1001100";
char tall_5[] = "0100100";
char tall_6[] = "0100000";
char tall_7[] = "0001111";
char tall_8[] = "0000000";
char tall_9[] = "0000100";

char *sjekke_tall[10] = {tall_0, tall_1, tall_2, tall_3, tall_4, tall_5, tall_6, tall_7, tall_8, tall_9};

SevSeg myDisplay;

char CorrectCode[][4]={{'3','8','5','2'},{'4','9','1','3'},{'2','4','6','1'}};
char AllSegments[4]={'8','8','8','8'};
int testnr=0;

int led[][3]={{40,41,42},{43,44,45},{46,47,48}} ;

int SignalSkuff=51;

int switch_one=0;
int switch_two=0;
int switch_three=0;

int RiktigSum= 0;

int fadeAmount=1;
int brightness=0;

void setup(){
  Serial.begin(9600);
  //2 dig SevSeg
  for (int j = 22; j < 36; j++) {
  pinMode(j, OUTPUT);
  }
  
  //led setup
  for (int i=0; i < 3; i++) {
    for (int j=0; j < 3; j++) {
      pinMode(led[i][j],OUTPUT);
    }
  }

  int displayType = COMMON_CATHODE;

  
       //SevSeg sifre
       int digit1 = 2; //Pin 1
       int digit2 = 3; //Pin 10
       int digit3 = 4; //Pin 4
       int digit4 = 5; //Pin 6
       
       //SevSeg segmenter
       int segA = 6; //Pin 12
       int segB = 7; //Pin 11
       int segC = 8; //Pin 3
       int segD = 9; //Pin 8
       int segE = 10; //Pin 2
       int segF = 11; //Pin 9
       int segG = 12; //Pin 7
       int segDP= 13; //Pin 5
   
  int numberOfDigits = 4;

  myDisplay.Begin(displayType, numberOfDigits, digit1, digit2, digit3, digit4, segA, segB, segC, segD, segE, segF, segG, segDP);
  
  myDisplay.SetBrightness(100);
  
  pinMode(SignalSkuff,INPUT);
}

void set_number(int startPin, int number) {
  for (int i = 0 ; i < 7; i++) {
    int pin = startPin + i;
    if (sjekke_tall[number][i] == '0') {
      digitalWrite(pin, LOW);
    }

    else {
      digitalWrite(pin, HIGH);
    }
  }
}

int settingRight(int switch_x, int led_one, int led_two, int led_three){
 
  if(switch_x >= 630 && switch_x <= 1023) {
    digitalWrite(led_one, HIGH);
    digitalWrite(led_two, LOW);
    digitalWrite(led_three,LOW);
    return 1;
  }
  if(switch_x >= 380 && switch_x <=630) {
    digitalWrite(led_two,HIGH);
    digitalWrite(led_one,LOW);
    digitalWrite(led_three,LOW);
    return 2;
  }
  if(switch_x >= 0 && switch_x <=380){
    digitalWrite(led_three, HIGH);
    digitalWrite(led_two, LOW);
    digitalWrite(led_one,LOW);
    return 3;
    }
  }

void loop(){
  
  if(digitalRead(SignalSkuff)==LOW){
    Serial.println("Signal LOW");
    for (int i=6; i< 13; i++){
      digitalWrite(i,LOW);
    }
    for(int i=40; i<49;i++){  //Skru av alle ledlys
      digitalWrite(i,LOW);
    }
    for (int k = 22; k < 36; k++) {
      digitalWrite(k, HIGH);
    }
     testnr++;
    
    while(true){
      if (digitalRead(SignalSkuff)==HIGH){
       
        break;
      }
    }
    if (testnr>=3){
    testnr=0;
    }
    Serial.println(testnr);
  }  
      
    
  
  if (digitalRead(SignalSkuff)==HIGH){
    Serial.println("Signal HIGH");
    
    set_number(22, 2);
    set_number(29, testnr*4);
      
    int sensorValue_one = analogRead(A1);
    int sensorValue_two = analogRead(A2);
    int sensorValue_three= analogRead(A3);
  
    int switch_one = sensorValue_one;
    int switch_two = sensorValue_two;
    int switch_three = sensorValue_three;
    
    settingRight(sensorValue_one,led[0][0],led[0][1],led[0][2]);
    settingRight(sensorValue_two,led[1][0],led[1][1],led[1][2]);
    settingRight(sensorValue_three,led[2][0],led[2][1],led[2][2]);
    int sum = settingRight(sensorValue_one,led[0][0],led[0][1],led[0][2])+ settingRight(sensorValue_two,led[1][0],led[1][1],led[1][2]) + settingRight(sensorValue_three,led[2][0],led[2][1],led[2][2]);

    RiktigSum = testnr+5;
    
    if (sum == RiktigSum){ //vis kode
      myDisplay.DisplayString(CorrectCode[testnr], 0);
    }

    else if (sum < RiktigSum){ //fade
      myDisplay.DisplayString(AllSegments, 0);
      myDisplay.SetBrightness(brightness);
      brightness = brightness + fadeAmount;
      if (brightness <= 0 || brightness >= 100) {
        fadeAmount = -fadeAmount;
      }
    }  
    
    else { // blink
      myDisplay.DisplayString(AllSegments, 0);
      //Serial.println(testnr);
      delay(50);
      myDisplay.DisplayString(AllSegments, 0);
    }


}
}


