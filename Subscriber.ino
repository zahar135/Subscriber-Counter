#include <YoutubeApi.h> 
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h> 
#include <avr/pgmspace.h>
#include <FastLED.h>
#include "font.h"

#define DATA_PIN 2

#define NUM_LEDS 256
#define MATRIX_WIDTH 16            
#define MATRIX_HEIGHT 16 
#define BRIGHTNESS 70
const String fontNumStr="0123456789!";
uint8_t curX;
uint8_t curY;
CRGB leds[NUM_LEDS];
int t ;

 
//------- Заменить на свои значения ------
char ssid[] = "Len_NEW";        // Имя точки доступа
char password[] = "321654987";  // Пароль от точки доступа
#define API_KEY "___________"  // API KEY выданный Google 
#define CHANNEL_ID "UCC-3Txx9GKDZLdOf1iB3UTQ" // ID YouTube канала
WiFiClientSecure client;
YoutubeApi api(API_KEY, client);
unsigned long api_mtbs = 60000; // Время между запросами к API
unsigned long api_lasttime;   //Переменная последнего запроса к API
 
long subs = 0;
 
void setup() {
 
  //Serial.begin(115200); // Скорость подключения к ESP для отладки проекта
FastLED.addLeds<NEOPIXEL,DATA_PIN>(leds, NUM_LEDS);  // Init of the Fastled library
FastLED.setBrightness(BRIGHTNESS);//0-255
for(int p = 0; p < NUM_LEDS; p++) {
    leds[p] = pgm_read_dword(&(you[p]));  // Read array from Flash
  }
Serial.begin (9600);

showText(String("!!!"), 1, 8, 0x0000FF);

FastLED.show();
     delay(1000);


  
  Serial.flush();
    client.setInsecure();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
 
  // Подключаемся к Wifi:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  
 }
 
void loop() {
 
  if (millis() - api_lasttime > api_mtbs)  {
    if(api.getChannelStatistics(CHANNEL_ID))
    {
      Serial.println("---------Статистика---------");
      Serial.print("Кол-во Подписчиков: ");
      Serial.println(api.channelStats.subscriberCount);
      Serial.print("Кол-во Просмотров: ");
      Serial.println(api.channelStats.viewCount);
      Serial.print("Кол-во Комментариев: ");
      Serial.println(api.channelStats.commentCount);
      Serial.print("Кол-во Видео: ");
      Serial.println(api.channelStats.videoCount);
  
       
    }
    api_lasttime = millis();
  }


for(int p = 0; p < NUM_LEDS; p++) {
    leds[p] = pgm_read_dword(&(you[p]));  // Read array from Flash
  }




   int t = (api.channelStats.subscriberCount);
    Serial.println(t);
   if (t < 10){
      showText(String(t), 11, 8, 0x0000FF);
      showText("00", 1, 8, 0x0000FF);
   } 

   if (t < 100, t >= 10){
      showText(String(t), 6, 8, 0x0000FF);
      showText("0", 1, 8, 0x0000FF);
   } 
if (t >= 100){
      showText(String(t), 1, 8, 0x0000FF);
   } 

   
 
  FastLED.show();
  delay(1000);
      

   
}



void setPixel (byte x, byte y, uint32_t color) {
   uint16_t curLed;
   if (x%2==0){
      curLed=MATRIX_HEIGHT*x+y;
   }
   else {
      curLed=(MATRIX_HEIGHT*(x+1)-1)-y; 
   }
   
   leds[curLed]=color;
}

byte getPixel (uint8_t vertPixels, byte startY, uint32_t color) {
   byte pixel;
   for (int k = 7; k >=0; k--) {//
        pixel = ((vertPixels >> k) & 0x01);
        if (pixel==1){
            setPixel (curX, curY, color);
        }
        curY++;
    }
    curY=startY;
    curX++;
}



void showText(String text, byte startX, byte startY, uint32_t color){//вывод статического текста
    int i;
    byte n;
    bool isRusLetter=false;
    byte vertPixels;
   
    curX=startX;
    curY=startY;
   
    for (i=0;i<text.length();i++){
        if (int(text[i])>=0){//цифры и символы
            n=fontNumStr.indexOf(text[i]);
            for (byte j=0;j<4;j++){
               vertPixels=pgm_read_byte(&(font7num[n][j]));
               
                   getPixel (vertPixels, startY, color);
             
                if (curX>=MATRIX_WIDTH) {
                    break;  
                }
            }
            curX++;
            if (curX>=MATRIX_WIDTH) {
                break;  
            }
        }
        
    }




















  
}
