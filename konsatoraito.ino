#include <Arduino.h>
#include <U8g2lib.h>
#include <math.h>
#include <Wire.h>

U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, SCL, SDA);

unsigned long btnTime=0;
unsigned long ledTime=0;
boolean isMenu=false;//0-颜色选择界面 1-在配置菜单
int mainColor[]={255,255,255};//主颜色
int isSet=0;//代表当前选择的配置
int isColor=0;//代表当前选择的颜色
int tempSet=0;//配置变量
int tempColor[]={0,0,0};//预调色
int colorSet[2][12][3]={
  {{226,43,48},{180,224,75},{39,67,210},{243,153,57},{221,221,233},{81,85,88},{255,228,64},{1,168,96},{253,153,225},{146,56,190},{1,173,185},{166,18,106}},
  {{234,91,118},{100,149,207},{254,213,82},{175,166,144},{233,115,115},{215,56,95},{69,67,65},{199,184,60},{237,144,186},{126,108,168},{245,173,59},{120,139,197}}
  };
char setName[2][20]={"IdolM@ster","MillionLive"};
char colorName[2][12][20]={
  {"Amami Haruka","Hoshii Miki","Kisaragi Chihaya","Takatsuki Yayoi","Hagiwara Yukiho","Kikuchi Makoto","Futami Mami","Akizuki Ritsuko","Minase Iori","Miura Azusa","Ganaha Hibiki","Shijou Takane"},
  {"Kasuga Mirai","Mogami Shizuka","Ibuki Tsubasa","Kitazawa Shiho","Kosaka Umi","Julia","Tokoro Megumi","Nanao Yuriko","Hakozaki Serika","Mochizuki Anna","Yabuki Kana","Yokoyama Nao"}
  };

void setup(void) {
  Serial.begin(9600);
  u8g2.begin();
  u8g2.enableUTF8Print();
  //MainLED
  pinMode(6, OUTPUT);//R
  pinMode(5, OUTPUT);//G
  pinMode(3, OUTPUT);//B
  //SubLED
  pinMode(11, OUTPUT);//R
  pinMode(10, OUTPUT);//G
  pinMode(9, OUTPUT);//B
  //Button
  pinMode(7, INPUT_PULLUP);//Menu
  pinMode(12, INPUT_PULLUP);//Perv
  pinMode(8, INPUT_PULLUP);//OK
  pinMode(13, INPUT_PULLUP);//Next

  tempColor[0]=colorSet[isSet][isColor][0];
  tempColor[1]=colorSet[isSet][isColor][1];
  tempColor[2]=colorSet[isSet][isColor][2];
  subLED(tempColor[0],tempColor[1],tempColor[2]);
  ledTime=millis();
}

void mainLED(int R,int G,int B){
  analogWrite(6, R);
  analogWrite(5, G);
  analogWrite(3, B);
}

void subLED(int R,int G,int B){
  analogWrite(11, R);
  analogWrite(10, G);
  analogWrite(9, B);
}

void prev(){
   if(isMenu==true){
    tempSet=fmod(--isSet,2);
    if(tempSet==-1){
      tempSet=1;
    };
   } else {
    isColor=fmod(--isColor,12);
    if(isColor==-1){
      isColor=11;
     };
    tempColor[0]=colorSet[isSet][isColor][0];
    tempColor[1]=colorSet[isSet][isColor][1];
    tempColor[2]=colorSet[isSet][isColor][2];
    subLED(tempColor[0],tempColor[1],tempColor[2]);
    ledTime=millis();
   }
}

void next(){
   if(isMenu==true){
    tempSet=fmod(++isSet,2);
   } else {
    isColor=fmod(++isColor,12);
    tempColor[0]=colorSet[isSet][isColor][0];
    tempColor[1]=colorSet[isSet][isColor][1];
    tempColor[2]=colorSet[isSet][isColor][2];
    subLED(tempColor[0],tempColor[1],tempColor[2]);
    ledTime=millis();
   }
}

void ok(){
   if(isMenu==true){
    isSet=tempSet;
    isColor=0;
    isMenu=false;
    tempColor[0]=colorSet[isSet][isColor][0];
    tempColor[1]=colorSet[isSet][isColor][1];
    tempColor[2]=colorSet[isSet][isColor][2];
    subLED(tempColor[0],tempColor[1],tempColor[2]);
    ledTime=millis();
   } else {
    mainColor[0]=tempColor[0];
    mainColor[1]=tempColor[1];
    mainColor[2]=tempColor[2];
   }
}

void menu(){
  if(isMenu==true){
    isMenu=false;
   } else if(isMenu==false){
    isMenu=true;
   }
}

void loop(void) {
  Serial.println(isMenu);
  u8g2.setFont(u8g2_font_unifont_t_latin); //设置字体
  u8g2.firstPage();
  do {
    if(isMenu==true){
      u8g2.setCursor(0, 15);
      u8g2.print("Select Set");
      u8g2.setCursor(0, 32);
      u8g2.print(setName[tempSet]);
    } else {
      u8g2.setCursor(0, 15);
      u8g2.print("Select Color");
      u8g2.setCursor(2, 32);
      u8g2.print(colorName[isSet][isColor]);
    }
  }while (u8g2.nextPage());
  
  if(millis()-ledTime>5000){
    subLED(0,0,0);
  }
  mainLED(mainColor[0],mainColor[1],mainColor[2]);//同步主颜色
  if(digitalRead(12)==LOW){
    if(millis()-btnTime>200){
      prev();
      btnTime=millis();
    }
  }
  if(digitalRead(13)==LOW){
    if(millis()-btnTime>200){
      next();
      btnTime=millis();
    }
  }
  if(digitalRead(8)==LOW){
    if(millis()-btnTime>200){
      ok();
      btnTime=millis();
    }
  }
  if(digitalRead(7)==LOW){
    if(millis()-btnTime>500){
      menu();
      btnTime=millis();
    }
  }
  delay(50);
}
