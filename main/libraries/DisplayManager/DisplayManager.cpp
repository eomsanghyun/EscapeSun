#include "DisplayManager.h"
#include <string>
using namespace std;
DisplayManager::DisplayManager(){
	updateTime.init();
}
void DisplayManager::init(Adafruit_SSD1306* p_display){
	display = p_display;
	// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display->begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
    // init done
    // Show image buffer on the display hardware.
    // Since the buffer is intialized with an Adafruit splashscreen
    // internally, this will display the splashscreen.
    display->display();
    delay(500);

    // Clear the buffer.
    display->clearDisplay();

    // draw a single pixel
    display->drawPixel(10, 10, WHITE);
    // Show the display buffer on the hardware.
    // NOTE: You _must_ call display after making any drawing commands
    // to make them visible on the display hardware!
    display->display();
    delay(500);
    display->clearDisplay();
    display_type = 0;
    Is_Emergency = false;
}

int getStartPoint(int val,int unit){
	// unit * 8 + 
	int temp_val = val;
	int mod = 10;
	int num_mod = 0;
	while(true){
		temp_val = temp_val - (temp_val % mod);
		num_mod++;
		if(temp_val == 0)break;
		mod = mod * 10;
	}
	// mid point : 64
	int total_num = num_mod + unit;
	float half_num = float(total_num / 2);
	int start_point = 64 - half_num * 8;
	if(start_point < 0){
		start_point = 0;
	}
	return start_point;
}
void DisplayManager::setDisplay(char* _title, char* _unit,int _value){
	// Update Parts
	display->setTextSize(1);
  	display->setTextColor(WHITE);
  	display->setCursor(2,2);
  	display->clearDisplay();
  	display->println(_title);
  	display->drawLine(0, 10, display->width(), 10, WHITE);
  	display->setTextSize(2);
  	int start_x_point = getStartPoint(_value,2); // parameter unit is 2. 2 is Length of this chars -> ' m'  
  	display->setCursor(start_x_point,16);
  	display->print(_value);
  	display->print(_unit); // unit parm -> 2
  	display->display();
  	delay(1);
}
void DisplayManager::update(int _dis,int _temp,int _body,int _humid,int _emg){
	distance = _dis;
	temp = _temp;
	body_temp = _body;
	humid = _humid;
	emergency = _emg;
	if(updateTime.Secondtime() > 3){
		updateTime.resetTime();
		display_type++;
		if(display_type > 4)display_type = 0;
		switch(display_type){
			case 0:{
				setDisplay("Distance"," m",distance);
				break;
			}
			case 1:{
				setDisplay("Temperature"," Cel",temp);
				break;
			}
			case 2:{
				setDisplay("Body Temperature"," Cel",body_temp);
				break;
			}
			case 3:{
				setDisplay("Humidity"," \%",humid);
				break;
			}
			case 4:{
				setDisplay("Emergency Level"," ",emergency);
				break;
			}
		}
		
	}
	// Update Parts
	/*
	display->setTextSize(1);
  	display->setTextColor(WHITE);
  	display->setCursor(2,2);
  	display->clearDisplay();
  	display->println("Distance");
  	display->drawLine(0, 10, display->width(), 10, WHITE);
  	display->setTextSize(2);
  	int start_x_point = getStartPoint(distance,2); // parameter unit is 2. 2 is Length of this chars -> ' m'  
  	display->setCursor(start_x_point,16);
  	display->print(distance);
  	display->print(" m"); // unit parm -> 2
  	display->display();
  	delay(1);
  	*/
}

