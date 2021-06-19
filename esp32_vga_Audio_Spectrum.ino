// REQUIRES the following Arduino libraries:
// - VGA Library: https://github.com/bitluni/ESP32Lib
// - Arduino FFT library: https://github.com/kosme/arduinoFFT
// - Getting Started ESP32 / ESP32S : https://www.youtube.com/watch?v=9b0Txt-yF7E
// Find All "Great Projects" Videos : https://www.youtube.com/c/GreatProjects
#include <FastLED.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Lib.h>
#include "arduinoFFT.h"
#include "index.h"  //Web page header file
#include <Ressources/CodePage437_9x16.h>

#define vgaWidth 400
#define vgaHeight 300
#define bands 16
#define cols vgaWidth / bands
#define pixelStart 3
#define dotPixelHeight 4
#define colPixelHeight 8
#define SAMPLING_FREQ   40000

////pin configuration
//const int redPin = 27;
//const int greenPin = 26;
//const int bluePin = 25;
//const int vsyncPin = 32;
//const int hsyncPin = 33;

//pin configuration for TTgo Board
const int redPin = 21;
const int greenPin = 18;
const int bluePin = 4;
const int vsyncPin = 15;
const int hsyncPin = 23;

arduinoFFT FFT = arduinoFFT();

//VGA3BitI vga;
VGA3Bit vga;  // Works with TTGO Board
//VGA14BitI vga;

WebServer server(80);
char ssid[30] = "";
char pass[30] = "";
const char* Apssid = "Esp32-VGA";
const char* Appassword = "3tawi-GP";

uint32_t lastTime = 0, lastdoT = 0, lastblack = 0;
int sp2 = 40, sp3 = 4, sp4 = 20000, sp5 = 4;
int sensorPin = A6;    // select the input pin for the Mic
#define SAMPLES 1024
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime;
unsigned int sampling_period_us;
int Exval[bands] = {0};
int Exdot[bands] = {0};
int vR, yh, vpx;

// standard colors
uint16_t myRGB = vga.RGB(0, 0, 0);
uint16_t myRED = vga.RGB(255, 0, 0);
uint16_t myGREEN = vga.RGB(0, 255, 0);
uint16_t myBLUE = vga.RGB(0, 0, 255);
uint16_t myWHITE = vga.RGB(255, 255, 255);
uint16_t myYELLOW = vga.RGB(255, 255, 0);
uint16_t myCYAN = vga.RGB(0, 255, 255);
uint16_t myMAGENTA = vga.RGB(255, 0, 255);
uint16_t myBLACK = vga.RGB(0, 0, 0);
uint16_t myCOLORS[8] = {myRED, myYELLOW, myCYAN, myBLUE, myWHITE, myMAGENTA, myGREEN, myWHITE};
uint8_t ac = 0;
int colId = 0;
int colId2 = 1;

void(* resetFunc) (void) = 0;//declare reset function at address 0

void getWifi() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFi.disconnect();
  }
  WiFi.begin(ssid, pass);
  int xc = 0;
  while (WiFi.status() != WL_CONNECTED && xc < 50) {
    delay(100);
    xc++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    WiFi.softAPdisconnect(true);
    vga.setCursor(25, 100);
    vga.fillRect(0, 100, 400, 30, myBLACK);
    vga.setTextColor(myCOLORS[ac]);
    vga.print("CONNECTED To: ");
    vga.print(ssid);
    vga.setCursor(20, 115);
    vga.print("IP Address: http://");
    vga.print(WiFi.localIP().toString().c_str());
  }
  else {
    WiFi.softAP(Apssid, Appassword);
    vga.setCursor(25, 100);
    vga.fillRect(0, 100, 400, 30, myBLACK);
    vga.setTextColor(myCOLORS[ac]);
    vga.print("Ssid or password is incorrect");
    vga.setCursor(20, 115);
    vga.print("Ssid: Esp32-VGA IP: http://192.168.4.1");
  }
}

void getdoT() {
  EVERY_N_SECONDS(5) {
    colId  = (colId + 1) % 8;
  }  
  for (int x = bands - 1; x >= 0; x--) {
    int xl = (cols * x) + pixelStart, xr = 387 - xl;
    yh = Exdot[x];
    if (yh <= 295) {
      vga.fillRect(xl, yh, cols - 2, dotPixelHeight, myBLACK);
      //vga.fillRect(xr, yh, 13, 2, myBLACK);
      vga.fillRect(xl, yh + 1, cols - 2, 2, (myCOLORS[colId]));
      Exdot[x]++;
    }
  }
}

void getblack() {
  for (int x = 0; x < bands; x++) {
    int xl = (cols * x) + pixelStart, xr = 387 - xl;
    yh = Exval[x];
    if (yh < 290) {
      vga.fillRect(xl, yh, cols, 14, myBLACK);
      //vga.fillRect(xr, yh, 13, 6, myBLACK);
      Exval[x] += 5;
    }
  }
}

void displayBand(int ban, int vmic) {
  EVERY_N_SECONDS(5) {
    colId2  = (colId2 + 1) % 8;
  }
  if (vmic < 0) {
    vmic = 0;
  }
  int xl = (cols * ban) + pixelStart, xr = 387 - xl;
  //ac = ban / 2;
  //for (int x = 290; x > vmic; x -= colPixelHeight+1) {  // this creats a space between pixel in the columns
  for (int x = 290; x > vmic; x -= colPixelHeight) {
    vga.fillRect(xl, x, cols - 2, colPixelHeight, myCOLORS[colId2]);
  }
  if (Exdot[ban] > vmic) {
    Exdot[ban] = vmic;
  }
  if (Exval[ban] > vmic) {
    Exval[ban] = vmic;
  }
}

void getdata() {
  for (int i = 0; i < SAMPLES; i++) {
    newTime = micros();
    vReal[i] = analogRead(sensorPin); // A conversion takes about 1uS on an ESP32
    vImag[i] = 0;
    while ((micros() - newTime) < sampling_period_us) { /* chill */ }
  }
  FFT.DCRemoval();
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  
  for (int i = 2; i < 512; i++) {
    if (vReal[i] > 900) {
      if (i <= 7 && i <= 9 )           {
        vR = map((int)vReal[i], sp4, 0, 0, 145), displayBand(0, vR);
      }
      else if (i > 9  && i <= 11  ) {
        vR = map((int)vReal[i], sp4 , 0, 0, 145), displayBand(1, vR);
      }
      else if (i > 11   && i <= 13  ) {
        vR = map((int)vReal[i], sp4, 0, 0, 145), displayBand(2, vR);
      }
      else if (i > 13  && i <= 15  ) {
        vR = map((int)vReal[i], sp4, 0, 0, 145), displayBand(3, vR);
      }
      else if (i > 15   && i <= 17  ) {
        vR = map((int)vReal[i], sp4, 0, 0, 145), displayBand(4, vR);
      }
      else if (i > 17   && i <= 19 ) {
        vR = map((int)vReal[i], sp4, 0, 0, 145), displayBand(5, vR);
      }
      else if (i > 19  && i <= 21 ) {
        vR = map((int)vReal[i], sp4, 0, 0, 145), displayBand(6, vR);
      }
      else if (i > 21  && i <= 25 ) {
        vR = map((int)vReal[i], sp4, 0, 0, 145), displayBand(7, vR);
      }
      else if (i > 25  && i <= 36 ) {
        vR = map((int)vReal[i], sp4, 0, 0, 145), displayBand(8, vR);
      }
      else if (i > 36  && i <= 50 ) {
        vR = map((int)vReal[i], sp4, 0, 0, 145), displayBand(9, vR);
      }
      else if (i > 50  && i <= 69 ) {
        vR = map((int)vReal[i], sp4, 0, 0, 145), displayBand(10, vR);
      }
      else if (i > 69  && i <= 97 ) {
        vR = map((int)vReal[i], sp4, 0, 0, 145), displayBand(11, vR);
      }
      else if (i > 97  && i <= 135 ) {
        vR = map((int)vReal[i], sp4, 0, 0, 145), displayBand(12, vR);
      }
      else if (i > 135  && i <= 189 ) {
        vR = map((int)vReal[i], sp4, 0, 0, 145), displayBand(13, vR);
      }
      else if (i > 189  && i <= 264 ) {
        vR = map((int)vReal[i], sp4, 0, 0, 145), displayBand(14, vR);
      }
      else if (i > 264   ) {
        vR = map((int)vReal[i], sp4, 0, 0, 145), displayBand(15, vR);
      }
    }
  }
}

void handleRoot() {
  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
}

void handleSpe2() {
  handleRoot();
  sp2 = server.arg("Speed2").toInt();
}

void handleSpe3() {
  handleRoot();
  sp3 = server.arg("Speed3").toInt();
}

void handleSpe4() {
  handleRoot();
  sp4 = 2000 * server.arg("Speed4").toInt();
}

void handleSpe5() {
  handleRoot();
  sp5 = server.arg("Speed5").toInt();
}

void handleWifi() {
  String text = server.arg("usname");
  int len = text.length() + 1;
  text.toCharArray(ssid, len);
  text = server.arg("pssw");
  len = text.length() + 1;
  text.toCharArray(pass, len);
  handleRoot();
  getWifi();
}

void handleRestesp() {
  handleRoot();
  delay(4000);
  resetFunc();
}

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);
  analogReadResolution(10); 
  vga.init(vga.MODE400x300, redPin, greenPin, bluePin, hsyncPin, vsyncPin);  // Working with TTGO Board

  WiFi.softAP(Apssid, Appassword);
  vga.setFont(CodePage437_9x16);
  vga.setTextColor(myCOLORS[2]);
  //vga.setCursor(15, 170);
  vga.setCursor(15, 75);
  vga.print("Ssid: Esp32-VGA IP: http://192.168.4.1");
  delay(5000);
  server.on("/", handleRoot);
  server.on("/MesSpeed2", handleSpe2);
  server.on("/MesSpeed3", handleSpe3);
  server.on("/MesSpeed4", handleSpe4);
  server.on("/MesSpeed5", handleSpe5);
  server.on("/Mywifi", handleWifi);
  server.on("/restesp", handleRestesp);
  server.begin();
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQ));
}

void loop() {
  server.handleClient();

  if (millis() - lastTime >= 28) {
    getdata();
    vga.show();
    lastTime = millis();
  } 
  //====================================================
  if (millis() - lastblack >= sp5) {
    getblack();
    lastblack = millis();
  }
  //====================================================
  if (millis() - lastdoT >= sp3) {
    getdoT();
    lastdoT = millis();
  }
  //====================================================
}
