/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x32 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/
#pragma once
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "TimeCheck.h"
#include <string>
#define OLED_RESET 4
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
using namespace std;
class DisplayManager{
private:
	Adafruit_SSD1306* display;
	int distance;
	int temp;
	int body_temp;
	int humid;
	int emergency;
	TimeCheck updateTime;
	int display_type;
	bool IsEmergency;
public:
	DisplayManager();
	void init(Adafruit_SSD1306* p_display);
	void update(int _dis,int _temp,int _body,int _humid,int _emg);
	void setDisplay(char* _title, char* _unit,int _value);
};