/**
   Mark Griffith      ccid:mgriffit    Sec:A1
   Jordan Lane				ccid:jlane			 Sec:A2
   Brad Ofrim				  ccid:ofrim			 Sec:A2  
*/

#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include <SD.h>
#include <stdlib.h>


//Defined as speed of bike, but this varaible influences the delay.
//Therefore variable speed is inversely related to the actual speed
int speed;
int jump_counter_1;
int jump_counter_2;
int obstacles;
int storage_1;
int storage_2;
int obstacle1_x;
int obstacle1_y;
int obstacle2_x;
int obstacle2_y;
int obstacle3_x;
int obstacle3_y;
int obstacle4_x;
int obstacle4_y;
int obstacle5_x;
int obstacle5_y;
int obstacle6_x;
int obstacle6_y;
int obstacle7_x;
int obstacle7_y;

//Hidden selection for playing against an ai
bool bot_mode;


// Display pins:
// standard U of A library settings, assuming Atmel Mega SPI pins
#define SD_CS    5 // Chip select line for SD card
#define TFT_CS   6 // Chip select line for TFT display
#define TFT_DC   7 // Data/command line for TFT
#define TFT_RST  8 // Reset Line for TFT (or connect to +5V)

//initialize the screen
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,TFT_DC,TFT_RST);

const int VERT_1 = 0; // vertical movement
const int HORIZ_1 = 1; // horizontal movement
const int SEL_1 = 11; // analog press
const int VERT_2 = 2; // vertical movement
const int HORIZ_2 = 3; // horizontal movement
const int SEL_2 = 10; // analog press

/** Stuff for Bike 1 */
int cursor_x1, cursor_y1;
int vertical_1;
int horizontal_1;
int select_1 = digitalRead(SEL_1);
int init_vert_1, init_horiz_1;
uint32_t bike_color_1;
//~ PlayerPos Player1; // dynamic array
/***********************/

/** Stuff for Bike 2 */
int cursor_x2, cursor_y2;
int vertical_2;
int horizontal_2;
int select_2 = digitalRead(SEL_2);
int init_vert_2, init_horiz_2;
uint32_t bike_color_2;
//~ PlayerPos Player2; // dynamic array
/***********************/

int vertical_initial;
int horizontal_initial;
char names[3][20];
int selection; // user selection
int old_selection; // old selection
char option_names[3][20];
int cursor_size = 2;

	int randomNum(){// Generate a ramdom number
		int randomValue = 0;
		int randomBit;
		for(int i = 0; i <= 8; i++){
			// Read a random value then take only the least significant bit
			randomBit = analogRead(5)%2;

			// Shift the previous bit over then add the new bit
			randomValue = (randomValue << 1)+randomBit;

			// Wait to allow for the analog value to change
			delay(5);
		}
		return randomValue;
	}

int move_bike (int direction, int bike, uint32_t bike_color) {
	while (true) {

		int vertical, horizontal, select, init_vert, init_horiz, cursor_x, cursor_y;

		vertical_1 = analogRead(VERT_1);
		vertical_2 = analogRead(VERT_2);
		horizontal_1 = analogRead(HORIZ_1);
		horizontal_2 = analogRead(HORIZ_2);

		if (bike == 1) {

			cursor_x = cursor_x1;
			cursor_y = cursor_y1;
			vertical = vertical_1;
			horizontal = horizontal_1;
			init_horiz = init_horiz_1;
			init_vert = init_vert_1;
			select = digitalRead(SEL_1);

			// Jump counter will keep counting up BUT when it gets to
			// 6 it will reset if the joystick button is pressed.
			// When the jump counter is < 6 the bike's path will not be drawn and it's
			// position will not be tracked or checked for collisions.
			// To prevent the user from being able to hold down the button, storage counts up
			// as the joystick button is held down, and so the jump counter will not reset to 0
			// unless storage is 0.
			if (select != 1 && jump_counter_1 >= 6 && storage_1 == 0) {
				jump_counter_1 = 0;
				storage_1 = 0;
			}
			if (select != 1) {
				storage_1 += 1;
			}
			if (select == 1) {
				storage_1 = 0;
			}
			if (!(jump_counter_1 < 6)) {
				tft.fillRect(cursor_x1, cursor_y1, cursor_size, cursor_size, bike_color); // draw bike
			}

			// Right
			if (direction == 0) {
				cursor_x1 = cursor_x1 + cursor_size;
			}
			// Down
			if (direction == 1) {
				cursor_y1 = cursor_y1 + cursor_size;
			}
			// Left
			if (direction == 2) {
				cursor_x1 = cursor_x1 - cursor_size;
			}
			// Up
			if (direction == 3) {
				cursor_y1 = cursor_y1 - cursor_size;
			}
		}

	if (bike == 2) {

		cursor_x = cursor_x2;
		cursor_y = cursor_y2;
		vertical = vertical_2;
		horizontal = horizontal_2;
		init_horiz = init_horiz_2;
		init_vert = init_vert_2;
		select = digitalRead(SEL_2);

		if (select != 1 && jump_counter_2 && storage_2 == 0) {
			jump_counter_2 = 0;
			storage_2 = 0;
		}
		if (select != 1) {
			storage_2 += 1;
		}
		if (select == 1) {
			storage_2 = 0;
		}
		if (!(jump_counter_2 < 6)) {
			tft.fillRect(cursor_x2, cursor_y2, cursor_size, cursor_size, bike_color);
		}

		// Right
		if (direction == 0) {
			cursor_x2 = cursor_x2 + cursor_size;
		}
		// Down
		if (direction == 1) {
			cursor_y2 = cursor_y2 + cursor_size;
		}
		// Left
		if (direction == 2) {
			cursor_x2 = cursor_x2 - cursor_size;
		}
		// Up
		if (direction == 3) {
			cursor_y2 = cursor_y2 - cursor_size;
		}
	}

	delay(speed+10);

	// HITS THE WALL
	// return 5
	if (cursor_x >= 125) {
		return 5;
	}
	if (cursor_x <= 2) { /*******************************************************************************************************changed this from 0 */
		return 5;
	}
	if (cursor_y <= 0) {
		return 5;
	}
	if (cursor_y >= 160) {
		return 5;
	}

	if (bike==1) {
		jump_counter_1 += 1; // count up by one
	}
	if (bike==1) {
		jump_counter_2 += 1; // count up by one
	}

	// STATIC POSITIONING
	if (vertical <= init_vert+20 && vertical >= init_vert-20 && horizontal <= init_horiz+20 && horizontal >= init_horiz-20) {
		return direction; // move in the same direction if joystick is static
	}
	// MOVE RIGHT
	if (horizontal > init_horiz+15 && vertical > (1023-horizontal) && vertical < horizontal) {
		// if direction is left, keep going left please
		if (direction == 2) {
			return direction;
		}
		else {return 0;}
	}
	// MOVE DOWN
	if (vertical > init_vert+15 && horizontal <= vertical && horizontal >= (1023-vertical) ) {
		if (direction == 3) {
			return direction;
		}
		else {return 1;}
	}
	// MOVE LEFT
	if (horizontal < init_horiz-15 && vertical < (1023-horizontal) && vertical > horizontal) {
		if (direction == 0) {
			return direction;
		}
		else {return 2;}
	}
	// MOVE UP
	if (vertical < init_vert-15 && horizontal >= vertical && horizontal <= (1023-vertical)) {
		if (direction == 1) {
			return direction;
		}
		else {return 3;}
	}
	else {return direction;}
	}
}

/********************************************************************/

// using Faster line drawing functions
void draw_grid(){

  uint32_t grid_color = 0x005677;
  for(int i=0; i<=128; i+=16){
		// vertical lines
		tft.drawFastVLine(i, 0, 160, grid_color);
  }
  tft.drawFastVLine(127, 0, 160, grid_color);
  for(int i=0; i<=160; i+=16){
		// horizontal lines
		tft.drawFastHLine(0, i, 128, grid_color);
  }
    tft.drawFastHLine(0, 159, 128, grid_color);

}

int draw_bike_1() {

    // initial values of the joystick
    init_vert_1 = analogRead(VERT_1);
    init_horiz_1 = analogRead(HORIZ_1);

    while(true) {

        delay(500);

        cursor_x1 = 64; // start here so it can get to x=1, looks better... have to change rightmost boundaries then so you can't control off the screen
        cursor_y1 = 40;

        while (true) {

            int vertical_1 = analogRead(VERT_1);
            int horizontal_1 = analogRead(HORIZ_1);

            return 1;// Go down
        }
    }
}

int draw_bike_2() {

    // initial values of the joystick
    init_vert_2 = analogRead(VERT_2);
    init_horiz_2 = analogRead(HORIZ_2);

    while(true) {

        delay(500);

        cursor_x2 = 64;
        cursor_y2 = 124;

        while (true) {

            int vertical_2 = analogRead(VERT_2);
            int horizontal_2 = analogRead(HORIZ_2);

           return 3;// Go up
        }
    }
}

/********************************************************************/
void update_color_selection(int bike){

	// replace old selection
    tft.drawRect(35+(20*old_selection),61+(37*bike),14,14,0x0000);

    // draw new selection
	tft.drawRect(35+(20*selection),61+(37*bike),14,14,0xFFFF);

}

void color_screen(){
	//Initallize Screen
	tft.fillScreen(0);
	tft.setTextWrap(false);
	tft.setTextColor(0x9656,0x0000);

	tft.setCursor(29,32);
	tft.print("Select Color");

	tft.drawLine(28,41, 99, 41, 0x9656);

	tft.setTextColor(0x5656, 0x0000);
	tft.setCursor(46,48);
	tft.print("Bike 1");

	//1st Bike Colour options
	tft.fillRect(37,63,10,10,0xF81F);
	tft.fillRect(57,63,10,10,0xF800);
	tft.fillRect(77,63,10,10,0x07E0);
	tft.drawRect(35,61,14,14,0xFFFF);

	tft.setCursor(46,85);
	tft.print("Bike 2");

	//2nd Bike Colour options
	tft.fillRect(37,100,10,10,0xF628);
	tft.fillRect(57,100,10,10,0xFFFF);
	tft.fillRect(77,100,10,10,0x9191);

	selection = 0;
	//Bike 1
	while(true){

        int horizontal_1 = analogRead(HORIZ_1);   // will be 0-1023

        // selection changed
        old_selection = selection;

        //If User moves cursor to right, increse selection, update cursor
        if (horizontal_1 > horizontal_initial+30) {
			selection = selection + 1;
			if (selection > 2) {
				selection = 2;
            }
			update_color_selection(0);
			delay(250);
		}

        //If user moves cursor to left, decrease selection, update cursor
		else if (horizontal_1 < horizontal_initial-30) {
			selection = selection - 1;
			if (selection < 0) {
				selection = 0;
            }
            update_color_selection(0);
            delay(250);
		}

		if (digitalRead(SEL_1)==0){
			switch (selection) {
				 case 0:
					bike_color_1 = 0xF81F;//Assign player 1 to first color
					break;
				case 1:
					bike_color_1 = 0xF800;//Assign player 1 to second color
					break;
				case 2:
					bike_color_1 = 0x07E0;//Assign player 1 to third color
					break;
			}

			//Change to Bike 2 selection
			selection = 0;
			tft.drawRect(35,61,14,14,0x0000);
			tft.drawRect(55,61,14,14,0x0000);
			tft.drawRect(75,61,14,14,0x0000);
			tft.drawRect(35,98,14,14,0xFFFF);

			delay(250);

			//Bike 2
			while(true){

				int horizontal_1 = analogRead(HORIZ_1);
				old_selection = selection;

				//If User moves cursor to right, increase selection, update cursor
				if (horizontal_1 > horizontal_initial+30) {
					selection = selection + 1;
					if (selection > 2) {
						selection = 2;
					}
					update_color_selection(1);
					delay(200);
				}

                //If User moves cursor to left, decrease selection, update cursor
				else if (horizontal_1 < horizontal_initial-30) {
					selection = selection - 1;
					if (selection < 0) {
						selection = 0;
					}
					update_color_selection(1);
					delay(200);
				}

				if (digitalRead(SEL_1)==0){
					switch (selection) {
						case 0:
							bike_color_2 = 0xF628;//Assign player 2 to first color
							return;
						case 1:
							bike_color_2 = 0xFFFF;//Assign player 2 to second color
							return;
						case 2:
							bike_color_2 = 0x9191;//Assign player 2 to third color
							return;
					}

				}

			}
		}


	}
}

void speed_screen(){

	//Initalize Screen
	tft.fillScreen(0);
	tft.setTextWrap(false);

	//Display "Select Speed"
	tft.setCursor(29,32);
	tft.setTextColor(0x5656,0x0000);
	tft.print("Select Speed");

	//Display "Press Analog to Return"
	tft.setCursor(29,120);
	tft.print("Press Analog");
	tft.setCursor(35, 128);
	tft.print("To Return");
	delay(250);

	uint32_t color = 0x00FF66;

	//Initalize Selection,
	selection = ((100-speed)/10)-1;

	//Initalize graphic on Screen
	tft.drawRect(13,65,102,30,0xFFFF);
	tft.fillRect(14,66,(selection*10)+10,28,0x07E0);

	//Display Speed to User
	tft.setCursor(38,100);
	tft.print("Speed");
	tft.setCursor(74,100);
	tft.print(100-speed);

	while(true){

		Serial.println(selection);
		Serial.println(100-speed);

		//If user presses the analog screen, return to the menu
		if (digitalRead(SEL_1) == 0){
			break;
		}

		//Display Screen Borders
		tft.drawRect(0,0,128,160,color);
		tft.drawRect(5,5,118,150,color+1000);
		color++;
		delay(50);

		//Read analog values
		int horizontal_1 = analogRead(HORIZ_1);

		//If user moves the cursor to the right, increase selection,
		//Increase "how full" the display bar is
		if (horizontal_1 > horizontal_initial+30) {
				selection = selection + 1;
				speed = speed - 10;

				if (selection > 9) {
					selection = 9;
					speed = 0;
				}

				//Update Graphic Bar
				tft.fillRect(14+(10*selection),66,10,28,0x07E0);
				tft.fillRect(70,100,25,10,0x0000);
				tft.setCursor(74,100);
				tft.print(100-speed);

				delay(50);


		}
		//If user moves the cursor to the left, decrease selection,
		//Decrease "how full" the display bar is
		else if (horizontal_1 < horizontal_initial-30) {
				selection = selection - 1;
				speed = speed + 10;

				if (selection < 0) {
					selection = -1;
					//If we set selection = 0, then some of the graphic is still displayed
					//By setting selection = 0, speed is still "100", yet no graphic displayed

					speed = 100;
				}

				//Update Graphic Bar
				tft.fillRect(24+(10*selection),66,100-(10*selection+ 10),28,0x0000);
				tft.fillRect(70,100,25,10,0x0000);
				tft.setCursor(74,100);
				tft.print(100-speed);

				delay(50);
		}

	}

}

void print_names() {
    tft.fillScreen(0); // clear screen with black
    tft.setTextWrap(false); // no wrap

	//Print Logo

	//T
	tft.fillRect(17,25,21,6, 0x7777);
	tft.fillRect(23,31,9,18, 0x7777);

	tft.drawLine(17,25,38,25,0xF81F);
	tft.drawLine(17,25,17,31,0xF81F);
	tft.drawLine(17,31,23,31,0xF81F);
	tft.drawLine(23,31,23,49,0xF81F);
	tft.drawLine(23,49,32,49,0xF81F);
	tft.drawLine(32,49,32,31,0xF81F);
	tft.drawLine(32,31,38,31,0xF81F);
	tft.drawLine(38,31,38,25,0xF81F);

	//R

	//Draw and fill Curve part of R
	tft.fillCircle(53,34,9,  0x7777);
	tft.fillCircle(53,34,3,  0x0000);
	tft.drawCircle(53,34,9,0xF81F);
	tft.drawCircle(53,34,3,0xF81F);
	tft.fillRect(44,25,9,18,0x0000);
	tft.fillRect(50,34,14,14,0x0000);

	//Fill Rectangle Parts of R
	tft.fillRect(41,25,12,6, 0x7777);
	tft.fillRect(56,34,6,3,  0x7777);
	tft.fillRect(38,37,6,12, 0x7777);

	//Fill Awkward Parrallelogram
	int k;
	for(k=0; k<= 9; k++){
	tft.drawLine(44+k,37,56+k,49, 0x7777);
	}

	//Draw Outlines
	tft.drawLine(41,25,53,25,0xF81F);
	tft.drawLine(41,25,41,31,0xF81F);
	tft.drawLine(41,31,53,31,0xF81F);
	tft.drawLine(56,34,56,37,0xF81F);
	tft.drawLine(56,37,62,37,0xF81F);
	tft.drawLine(62,37,62,34,0xF81F);
	tft.drawLine(38,37,38,49,0xF81F);
	tft.drawLine(38,49,44,49,0xF81F);
	tft.drawLine(44,49,44,37,0xF81F);
	tft.drawLine(38,37,53,37,0xF81F);
	tft.drawLine(44,37,56,49,0xF81F);
	tft.drawLine(53,37,65,49,0xF81F);
	tft.drawLine(56,49,65,49,0xF81F);

	//0
	tft.fillCircle(74,37,12, 0x7777);
	tft.fillCircle(74,37,6,  0x0000);
	tft.drawCircle(74,37,12, 0xF81F);
	tft.drawCircle(74,37,6,  0xF81F);

	//N

	tft.fillRect(86,37,6,12, 0x7777);
	tft.fillTriangle(86,25,86,37,98,37,0x7777);
	tft.fillRect(104,25,6,12, 0x7777);
	tft.fillTriangle(98,37,110,37,110,49,0x7777);

	tft.drawLine(86,25,86,49,0xF81F);
	tft.drawLine(86,49,92,49,0xF81F);
	tft.drawLine(92,49,92,37,0xF81F);
	tft.drawLine(92,37,104,37,0xF81F);
	tft.drawLine(104,37,104,25,0xF81F);
	tft.drawLine(104,25,110,25,0xF81F);
	tft.drawLine(110,25,110,49,0xF81F);
	tft.drawLine(110,49,86,25,0xF81F);


    // start printing names
    for (int i = 0; i <= 1; i++) {
        tft.setCursor(35,i*32+72);

        if (i == 0) {
            tft.setTextColor(0xF81F,0xFFFF);	//Start
        }
        else if (i == 1) {
            tft.setTextColor(0x7777,0x0000);	//Options
        }

        tft.print(names[i]);
        tft.print('\n');
    }
}

void print_options() {

	//Initalize Colour Screen
	tft.fillScreen(0);
	tft.setTextWrap(false);

	//Display List of options
	for (int i = 0; i <=3; i++) {
		tft.setCursor(25, i*30+30);

		// Colour
		 if (i == 0) {
            tft.setTextColor(0xF81F,0xFFFF);
        }

        // Speed
        else if (i == 1) {
            tft.setTextColor(0x7777,0x0000);
        }

        // Objects
        else if (i == 2) {
            tft.setTextColor(0x7777,0x0000);
        }

        //Back
        else if (i == 3) {
            tft.setTextColor(0x7777,0x0000);
        }

        tft.print(option_names[i]);
        tft.print('\n');
	}

	tft.setCursor(88,90);
	if(obstacles == 0){
		tft.print("Off");
	}
	else{
		tft.print("On");
	}
}

void update_names() {

        // replace old selection text
        tft.setCursor(35,old_selection*32+72);
        tft.setTextColor(0x7777,0x0000);
        tft.print(names[old_selection]);

        // print new selection text
        tft.setCursor(35,selection*32+72);
        tft.setTextColor(0xF81F,0xFFFF);
        tft.print(names[selection]);
}

void update_names_options() {

        // replace old selection text
        tft.setCursor(25,old_selection*30+30);
        tft.setTextColor(0x7777,0x0000);
        tft.print(option_names[old_selection]);

        // print new selection text
        tft.setCursor(25,selection*30+30);
        tft.setTextColor(0xF81F,0xFFFF);
        tft.print(option_names[selection]);

}

int option_menu() {

	//Options
	strcpy(option_names[0], "Change Colour");
	strcpy(option_names[1], "Change Speed");
	strcpy(option_names[2], "Obstacles:");
	strcpy(option_names[3], "Back");

	//Initalize
	selection = 0;
	print_options();
	delay(250);

	uint32_t color = 0x00FF66;

	while(true){

		//If user presses the analog stick...
		if (digitalRead(SEL_1)==0){

			//Check which option they are selecting, and go to corresponding choice
			switch (selection){
				case 0:
					color_screen(); //Select Color Screen
					option_menu();
					break;
				case 1:
					speed_screen(); //Select Speed Screen
					option_menu();
					break;
				case 2:
					if(obstacles == 0){
						obstacles = 1;
						tft.fillRect(88,90,30,10,0x0000);
						tft.setCursor(88,90);
						tft.setTextColor(0x7777,0x0000);
						tft.print("On");
						delay(200);

					}
					else if(obstacles == 1){
						obstacles = 0;
						tft.fillRect(88,90,30,10,0x0000);
						tft.setCursor(88,90);
						tft.setTextColor(0x7777,0x0000);
						tft.print("Off");
						Serial.println(obstacles);
						delay(200);
					}


					break;
				case 3:
					return 0;

			}

		}

		//Screen Borders
		tft.drawRect(0,0,128,160,color);
		tft.drawRect(5,5,118,150,color+1000);

		/** use drawRect for a border
		and setTextSize for title */

		vertical_1 = analogRead(VERT_1);      // will be 0-1023

		// selection changed
		old_selection = selection;
		//~ Serial.print("selection: ");Serial.println(selection);

		//Moves cursor down, increases selection, and updates visuals
		if (vertical_1 > vertical_initial+40) {
        selection = selection + 1;

			if (selection > 3) {
				selection = 3;
			}
            else{
			delay(200);
            update_names_options();
			}
        }

		//Moves cursor up, decreases selection, and updates visuals
		else if (vertical_1 < vertical_initial-40) {
        selection = selection - 1;

            if (selection < 0) {
				selection = 0;
            }
            else{
            delay(200);
            update_names_options();
			}
        }
                //Borders change colour
                color++;
	}

}

void start_menu() {
	//Start Menu Options
    strcpy(names[0], "Start Game");
    strcpy(names[1], " Options  ");

	//Initalize
    selection = 0;
    print_names();
    delay(250);
    uint32_t color = 0x00FF66;



    while(true) {



			//Check if user has made a selection
            if (digitalRead(SEL_1) == 0) {


							int timer = 0;

							//Activate one player mode
							while(digitalRead(SEL_1) == 0 && timer < 1001){
								timer = timer + 1;
								delay(1);
							}
							if(timer > 999){
								bot_mode = 1;
							}
							else{
								bot_mode = 0;
							}

				switch (selection){
					//If user selects "start game", terminate start menu
					case 0:
						return;

					//If user selects "options" display option menu
					case 1:
						option_menu();


					//Upon exiting the option menu, reset the start menu
						print_names();
						delay(250);
						selection = 0;
						break;
				}

			}

			//Screen Borders (change colour over time)
            tft.drawRect(0,0,128,160,color);
            tft.drawRect(5,5,118,150,color+1000);

            /** use drawRect for a border
                and setTextSize for title */

				//Read Input values from 1st analog stick
                vertical_1 = analogRead(VERT_1);      // will be 0-1023
                horizontal_1 = analogRead(HORIZ_1);   // will be 0-1023

                // selection changed
                old_selection = selection;

				//Move cursor down, increase selection, and update visuals
                if (vertical_1 > vertical_initial+30) {
                    selection = selection + 1;
                    if (selection > 1) {
                        selection = 1;
                    }
                    else{
					update_names();
					}
                }

                //Move cursor up, decrease selection, and update visuals
                else if (vertical_1 < vertical_initial-30) {
					selection = selection - 1;
					if (selection < 0) {
						selection = 0;
					}
					else{
                    update_names();
					}
                }

                color++;
            }
    return;
}

/********************************************************************/

int location[4][80] = {};//Two dimentional array holding 4 columns and 80 rows

int check_for_crash(int x_in, int y_in, int player){
  //These variables are used to keep track of a cursor on a screen
  //An array of bits is created to track locations where no cursor has been yet
  x_in = x_in/2;
  y_in = y_in/2;

  typedef struct {
    int row;//Track the y position; 0-80
    int column;//Track x position; 0-3
    int position;//Where the x is located within column
  } cursor;

  cursor Cursor; //Initialize two cursors

  /*
    When a cursor moves the x,y coordinates must be checked against all previous
  values of x,y coordinates. This process becomes slower as more coordinates are
  added. We know how many possible locations the cursor could be since it is
  constrained to the edges of the screen. Initializing a two dimentional array
  (a matrix) allows location comparison to be completed in O(1) since only one
  value is compared regardless of how many locations the cursors have already
  been at. The array initially contains zeros so the value of zero is treated as
  a location that the cursor has not yet been located. To reduce memory, one
  location in the matrix is represented with only one bit. This is done by storing
  sequences of 16 locations in  rows as one integer.
    The pros of this method of location comparisons is that it is very fast.
    The downside of this method is that it requires a fairly significant chunk of
  memory regardless of how many locations the cursor has been:
      16 bits/row * 80 rows/column * 4 columns = 5120 bits = 640 bytes
  This much memory is however well within the capacity of an Arduino
  */

  //Find location in matrix
    //Given a x location from 0 to 64 find the column in question
  Cursor.column = x_in/16;
    //Using the x location, find the position within the column and create a value
    //to represent it
  Cursor.position = 1 << (x_in%16);
    //Given a y location from 0 to 80 the row is given by that value
  Cursor.row = y_in;
  //Check value at that location
    //A simple bitwise or operation on the whole row value evaluates to one if
    //there is a collision otherwise it evaluates to 0;
    //If value is one a collision has occured: handle event
      //Return a value that represents a collision
  //If value is zero update value: continue
    //A bitwise and is used to update the value of the whole row
    //Return a value representing no collision

  if(location[Cursor.column][Cursor.row] & Cursor.position){//Collision
    //return collison value (4 for player 1, 5 for player 2);
    Serial.println("Crash");
    //Reset the array to {0}
    for(int i=0; i<80; i++){
      for(int j=0; j<4; j++){
        location[j][i] = 0;
      }
    }
    return 3 + player;
  }
  else{
    // Update matrix
    location[Cursor.column][Cursor.row] =
    (location[Cursor.column][Cursor.row] | Cursor.position);
    //~ Serial.println(Cursor.position, BIN);
    //return pass value;
    return 0;
  }
}

void announce_winner (int winner, uint32_t linecolor) {

	//Initalize Screen
	tft.fillScreen(0x000000);
	tft.setCursor(30, 50);
	tft.setTextColor(0xFFFFFF);
	tft.setTextSize(2);
	tft.setTextWrap(true);

	//Player
	delay(200);
	tft.drawLine(0,58,29,58,linecolor);
	tft.print("Player");
	tft.drawLine(101,58,128,58,linecolor);

	delay(200);

	//The Winning Player's Number
	tft.setCursor(58, 70);
	tft.drawLine(0,78,57,78,linecolor);
	tft.print(winner);
	tft.drawLine(66,78,128,78,linecolor);

	delay(200);

	//Wins
	tft.setCursor(40, 88);
	tft.drawLine(0,96,39,96,linecolor);
	tft.print("Wins");
	tft.drawLine(84,96,128,96,linecolor);

}

void death_animation_1(int player) { // Crashing Animation
	int color_crash;
	int color_hit;
	int rando;
	tft.fillScreen(0x000000); // background color
	tft.setTextColor(0xFFFFFF);
	tft.setTextSize(2);
	tft.setTextWrap(true);
	for (int i=0;i<=90;++i) {
		if (!digitalRead(SEL_1)) {
			break;
		} 
		rando = randomNum()%160;
		tft.drawFastVLine(rando,0,i,0xF800);
		if (i==18) {
			tft.setCursor(30, 140);
			tft	.print("C");
		}
		if (i==36) {
			tft.setCursor(45, 140);
			tft.print("R");
		}
		if (i==54) {
			tft.setCursor(60, 140);
			tft.print("A");
		}
		if (i==72) {
			tft.setCursor(75, 140);
			tft.print("S");
		}
		if (i==90) {
			tft.setCursor(90, 140);
			tft.print("H");
		}
	}
	delay(400);
	tft.setTextSize(1);
}

// falling off the edge of the map animation (shrinking square)
void death_animation_2 (int player) {
	tft.fillScreen(0x000000); // background color
	int size_fall = 70;
	int position_x_fall = 30;
	int position_y_fall = 50;
	int bike_color;

	if (player == 1) {
		bike_color = bike_color_1;
	}
	if (player == 2) {
		bike_color = bike_color_2;
	}

	for (int i = 0; i <= 10; i++) {
		tft.fillRect(position_x_fall, position_y_fall, size_fall, size_fall, bike_color);
		delay(100);
		tft.fillRect(position_x_fall, position_y_fall, size_fall, size_fall, 0x000000);
		position_x_fall += 3;
		position_y_fall += 4;
		size_fall = size_fall - 7;
	}
}

// checks HOW the player lost
int game_over_check(int bike_mode, /**PlayerPos Player_data*/ int player) {

	if (bike_mode == 4) {
		death_animation_1(player);
		return 4;
	}
	else if (bike_mode == 5) {
    //Reset the array to {0}
		for(int i=0; i<80; i++){
			for(int j=0; j<4; j++){
				location[j][i] = 0;
			}
		}

		//Player has fallen over the edge, therefore display falling animation
		death_animation_2(player);
		return 5;
	}
}


void create_obstacles(){

	//Equation so we always get at least two obstacles
	int obstacle_number = (randomNum() % 6) + 2;

	Serial.print(obstacle_number);

	if(obstacle_number >= 1){

		obstacle1_x = randomNum() % 128;
		obstacle1_y = randomNum() % 160;

		//~ Serial.println("First");
		Serial.println(obstacle1_x);
		Serial.println(obstacle1_y);

		if(obstacle1_x < 60 || obstacle1_x > 64){
			for(int i = 0; i<=1 ; i++){
				for(int j = 0; j<=1 ; j++){
					check_for_crash(obstacle1_x+(2*i), obstacle1_y+(2*j), 0);
				}
			}
			tft.fillRect(obstacle1_x & ~1, obstacle1_y & ~1, 4, 4, 0x7777);
			tft.drawRect(obstacle1_x & ~1, obstacle1_y & ~1, 4, 4, 0xF81F);

		}
		delay(10);
	}

	if(obstacle_number >= 2){

		obstacle2_x = randomNum() % 128;
		obstacle2_y = randomNum() % 160;

		Serial.println(obstacle2_x);
		Serial.println(obstacle2_y);

		if(obstacle2_x < 60 || obstacle2_x > 64){
			for(int i = 0; i<=1 ; i++){
				for(int j = 0; j<=1 ; j++){
					check_for_crash(obstacle2_x+(2*i), obstacle2_y+(2*j), 0);
				}
			}
			tft.fillRect(obstacle2_x & ~1, obstacle2_y & ~1, 4, 4, 0x7777);
			tft.drawRect(obstacle2_x & ~1, obstacle2_y & ~1, 4, 4, 0xF81F);
		}
		delay(10);
	}


	if(obstacle_number >= 3){

		obstacle3_x = randomNum() % 128;
		obstacle3_y = randomNum() % 160;

		Serial.println(obstacle3_x);
		Serial.println(obstacle3_y);

		if(obstacle3_x < 60 || obstacle3_x > 64){
			for(int i = 0; i<=1 ; i++){
				for(int j = 0; j<=1 ; j++){
					check_for_crash(obstacle3_x+(2*i), obstacle3_y+(2*j), 0);
				}
			}
			tft.fillRect(obstacle3_x & ~1, obstacle3_y & ~1, 4, 4, 0x7777);
			tft.drawRect(obstacle3_x & ~1, obstacle3_y & ~1, 4, 4, 0xF81F);
		}

		delay(10);
	}

	if(obstacle_number >= 4){

		obstacle4_x = randomNum() % 128;
		obstacle4_y = randomNum() % 160;

		Serial.println(obstacle4_x);
		Serial.println(obstacle4_y);

		if(obstacle4_x < 58 || obstacle4_x > 64){
			for(int i = 0; i<=2 ; i++){
				for(int j = 0; j<=2 ; j++){
					check_for_crash(obstacle4_x+(2*i), obstacle4_y+(2*j), 0);
				}
			}
			tft.fillRect(obstacle4_x & ~1, obstacle4_y & ~1, 6, 6, 0x7777);
			tft.drawRect(obstacle4_x & ~1, obstacle4_y & ~1, 6, 6, 0xF81F);
		}
		delay(10);
	}

	if(obstacle_number >= 5){

		obstacle5_x = randomNum() % 128;
		obstacle5_y = randomNum() % 160;

		Serial.println(obstacle5_x);
		Serial.println(obstacle5_y);

		if(obstacle5_x < 56 || obstacle5_x > 64){
			for(int i = 0; i<=3 ; i++){
				for(int j = 0; j<=3 ; j++){
					check_for_crash(obstacle5_x+(2*i), obstacle5_y+(2*j), 0);
				}
			}
			tft.fillRect(obstacle5_x & ~1, obstacle5_y & ~1, 8, 8, 0x7777);
			tft.drawRect(obstacle5_x & ~1, obstacle5_y & ~1, 8, 8, 0xF81F);
		}
		delay(10);
	}

	if(obstacle_number >= 6){

		obstacle6_x = randomNum() % 128;
		obstacle6_y = randomNum() % 160;

		Serial.println(obstacle6_x);
		Serial.println(obstacle6_y);

		if(obstacle6_x < 54 || obstacle6_x > 70){
			for(int i = 0; i<=4 ; i++){
				for(int j = 0; j<=4 ; j++){
					check_for_crash(obstacle6_x+(2*i), obstacle6_y+(2*j), 0);
				}
			}
			tft.fillRect(obstacle6_x & ~1, obstacle6_y & ~1, 10, 10, 0x7777);
			tft.drawRect(obstacle6_x & ~1, obstacle6_y & ~1, 10, 10, 0xF81F);
		}
		delay(100);
	}

	if(obstacle_number >= 7){

		obstacle7_x = randomNum() % 128;
		obstacle7_y = randomNum() % 160;

		Serial.println(obstacle3_x);
		Serial.println(obstacle3_y);

		if(obstacle7_x < 52 || obstacle7_x > 66){
			for(int i = 0; i<=5; i++){
				for(int j = 0; j<=5; j++){
					check_for_crash(obstacle7_x+(2*i), obstacle7_y+(2*j), 0);
				}
			}
			tft.fillRect(obstacle7_x & ~1, obstacle7_y & ~1, 12, 12, 0x7777);
			tft.drawRect(obstacle7_x & ~1, obstacle7_y & ~1, 12, 12, 0xF81F);
		}
		delay(10);
	}
}

/********************************************************************/

int distances[4];

//Define a struct to keep track of values for the "bot"
typedef struct bot {
  int x;
  int y;
  int direction;
  int position;
  int row;
  int column;
}bot;

//Initialize a bot
bot Bot;
bot create_bot(){
  //Store initial values
  Bot.x = 64;
  Bot.y = 120;
  Bot.direction = 1;
  return Bot;
}

//Calculate the distances to the nearest object on all four sides of the bot
void dist_to_wall(bot Bot){

  int dist_up     = 0;
  int dist_left   = 0;
  int dist_right  = 0;
  int dist_down   = 0;
  Bot.column = Bot.x /32;
  Bot.row = Bot.y/2;
  Bot.position = 1 << ((Bot.x/2)%16);


  //up
  //Calculate the distance above the bot's location
  while(!(location[Bot.column][Bot.row+dist_up] & Bot.position) &&
        Bot.row-dist_up > 0){
    dist_up += 1;
  }

  //Calculate the distance below the bot's location
  while(!(location[Bot.column][Bot.row+dist_down] & Bot.position) &&
        Bot.row+dist_down < 80){
        dist_down += 1;
  }

  //Calculate the distance to the left of the bot's location
  while(!(location[(Bot.x/2-dist_left)/16][Bot.row] & 1<<((Bot.position+dist_left)%16)) &&
        (Bot.x-dist_left > 0)){
    dist_left += 1;
  }

  //Calculate the distance to the right of the bot's location
  while(!(location[(Bot.x/2+dist_right)/16][Bot.row] & 1<<((Bot.position-dist_right)%16)) &&
        (Bot.x+dist_right < 128)){
    dist_right += 1;
  }

  //Store the locations
  distances[2] = dist_right;
  distances[3] = dist_down;
  distances[0] = dist_left;
  distances[1] = dist_up;
}

//move the players cursor
//Input value represents: 0: left, 1:up, 2:rigth, 3: down
int move(int direction){
  int next_direction = direction;
  vertical_1 = analogRead(VERT_1);
  horizontal_1 = analogRead(HORIZ_1);

  // If the joystick has been moved change the directin of the players motion
  //to represent which way the player is trying to move
  if(abs(vertical_1 - vertical_initial) > 30 || abs(horizontal_1 - horizontal_initial) > 30){
    vertical_1 = vertical_1 - vertical_initial;
    horizontal_1 = horizontal_1 - horizontal_initial;

    if(abs(vertical_1) > abs(horizontal_1)){ //move verticaly
      if(direction%2){
        cursor_y1 += (direction-2)*2;
      }
      else{
        cursor_y1 += (cursor_size)*vertical_1/abs(vertical_1);
        next_direction = 2 + vertical_1/abs(vertical_1);
      }

    }
    else{//move horizontaly
      if((direction+1)%2){
        cursor_x1 += (direction-1)*2;
      }
      else{
        cursor_x1 += (cursor_size)*horizontal_1/abs(horizontal_1);
        next_direction = 1 + horizontal_1/abs(horizontal_1);
      }
    }

  }
  //Continue to move in the same direction
  else{
    if(direction%2){//check for 1 or 3
      cursor_y1 += (cursor_size)*(direction-2)*vertical_1/abs(vertical_1);
    }
    else{
      cursor_x1 += (cursor_size)*(direction-1)*horizontal_1/abs(horizontal_1);
    }
  }
  //Draw the curor image
  tft.fillRect(cursor_x1, cursor_y1, cursor_size, cursor_size, bike_color_1);
  delay(speed); // speed of bike (explained previously)
  return next_direction;
}

//Determine what value in the distances array is the largest and return it
int decide(){
  int move_to = 0;
  for(int i = 0; i < 4; i++){
    if(distances[i] > distances[move_to]){
      move_to = i;
      // Serial.print(distances[i]);
    }
  }
  return move_to;
}

void move_bot(int direction_input){//0: left, 1:up, 2:right, 3:down
  Bot.x = Bot.x + 2*((direction_input-1)%2);
  Bot.y = Bot.y + 2*((direction_input-2)%2);
}

//Frame for the "secret" one player mode
void Tron(){
  //Reset value
  bot_mode = !bot_mode;

  //Set/reset the cursor values
  cursor_x1 = 64;
  cursor_y1 = 32;

  //Define the direction for the player to move
  int direction = 1;

  //Initialize a counter to randomize the motion of the bot
  int counter=0;

  //Initialize a counter to determine when the bot will turn
  int bot_counter = -3;

  //determinethe next course of action for the bot
  int next_direction = direction;

  //Initialize the "bot"
  Bot = create_bot();

  //Set a value to determine when a crash has occured
  int crash_status = 0;//0: no crash, 4: player crash, 5: bot crash

  //Draw scene
  tft.fillScreen(0x000000);
  draw_grid();
  //Game loop
  while(!crash_status){
    dist_to_wall(Bot);
    direction = move_bike(direction,1,bike_color_1);
    Serial.print("direction: ");Serial.println(direction);
    //Draw the bot
    tft.fillRect(Bot.x, Bot.y, cursor_size, cursor_size, bike_color_2);
    if (jump_counter_1 >= 6) {
			// Check for collision
			crash_status = check_for_crash(cursor_x1, cursor_y1, 1) +
			check_for_crash(Bot.x, Bot.y, 2);
		}
    //~ next_direction = decide();
    if(bot_counter>((randomNum())%5)){
      next_direction = decide();
      if(next_direction != (Bot.direction-2)%4){
        Bot.direction = next_direction;
      }
      bot_counter = 0 ;
      counter = (cursor_x1*cursor_y1*direction)%20;
    }
    bot_counter++;
    move_bot(Bot.direction);

    //Ensure both bot and player are on the screen
    if(cursor_x1 <0 || cursor_x1 > 128 || cursor_y1 <0 || cursor_y1 > 160){
			cursor_x1 = 64;
			cursor_y1 = 32;
			Bot = create_bot();
			Serial.print("break");
			for(int i=0; i<80; i++){
				for(int j=0; j<4; j++){
					location[j][i] = 0;
				}
			}	
			death_animation_2(1);
			break;
		}
		if(Bot.x <0 || Bot.x > 128 || Bot.y <0 || Bot.y > 160){
			cursor_x1 = 64;
			cursor_y1 = 32;
			Bot = create_bot();
			Serial.print("break");
			for(int i=0; i<80; i++){
				for(int j=0; j<4; j++){
					location[j][i] = 0;
				}
			}
			death_animation_2(2);
			break;
			}

		if(crash_status){
			death_animation_1(crash_status);
			break;
		}
  }
}

/********************************************************************/

int main() {

	init();

	// set joystick button to input
	pinMode(SEL_1, INPUT);
	pinMode(SEL_2, INPUT);

	Serial.begin(9600);

	// If your TFT's plastic wrap has a Red Tab, use the following:
	//~ tft.initR(INITR_REDTAB);   // initialize a ST7735R chip, red tab
	// If your TFT's plastic wrap has a Green Tab, use the following:
	//tft.initR(INITR_GREENTAB); // initialize a ST7735R chip, green tab
	tft.initR(INITR_BLACKTAB);

	obstacles = 0;
	speed = 50;
	bike_color_1= 0xF81F;
	bike_color_2= 0xF628;

	vertical_initial = analogRead(VERT_1);
	horizontal_initial = analogRead(VERT_1);

  while(true) {
	// reset selects to HIGH
	digitalWrite(SEL_1, HIGH);
	digitalWrite(SEL_2, HIGH);

	//Display the Start Menu to the User
	start_menu();
  if(bot_mode){
    while(true){
			jump_counter_1 = 6;
      Tron(); // play against the bot
      start_menu();
      if(!bot_mode){break;}
    }
  }

	//Define inital states of Bikes
	int collision = 0;
	int collision2 = 0;
	int game_over_1 = 0;
	int game_over_2 = 0;

	tft.fillScreen(0x000000); // background color
	int bike_mode_1 = 0;
	int bike_mode_2 = 0;

	//Draw the background gird
	draw_grid();

	if(obstacles == 1){
		create_obstacles();

	}

	// initialize jump_counter
	jump_counter_1 = 6;
	jump_counter_2 = 6;

	bike_mode_1 = draw_bike_1();
	bike_mode_2 = draw_bike_2();

	int start_time = millis();

		while (true) {
			//Define 1st bike's state based on analog input
			switch (bike_mode_1) {
				case 0:
					bike_mode_1 = move_bike(0,1,bike_color_1);
					break;
				case 1:
					bike_mode_1 = move_bike(1,1,bike_color_1);
					break;
				case 2:
					bike_mode_1 = move_bike(2,1,bike_color_1);
					break;
				case 3:
					bike_mode_1 = move_bike(3,1,bike_color_1);
					break;
			}

			//Define 2nd bike's state based on analog input
			switch (bike_mode_2) {
				case 0:
					bike_mode_2 = move_bike(0,2,bike_color_2);
					break;
				case 1:
					bike_mode_2 = move_bike(1,2,bike_color_2);
					break;
				case 2:
					bike_mode_2 = move_bike(2,2,bike_color_2);
					break;
				case 3:
					bike_mode_2 = move_bike(3,2,bike_color_2);
					break;
			}

			int time_bef = millis();


			//~ Serial.print("collision: ");Serial.println(collision);
			//~ Serial.print("collision 2: ");Serial.println(collision2);


			//Check if either bike has crashed
			if (jump_counter_1 >= 6) {
				collision = check_for_crash(cursor_x1, cursor_y1, 1);
			}
			if (jump_counter_2 >= 6) {
				collision2 = check_for_crash(cursor_x2, cursor_y2, 2);
			}

			int time_after = millis();
			Serial.println();
			//~ Serial.print("Time Taking To Check Collisions: ");
			//~ Serial.println(time_after-time_bef);
			//~ Serial.println();
			if (collision == 4) { // bike 1 has crashed
				bike_mode_1 = 4;
			}

			if (collision2 == 5) { // bike 2 has crashed
				bike_mode_2 = 4;
			}


			/** bike_mode = 4 if bike hits a path
					bike_mode = 5 if bike falls off edge */

		 // game_over_check will assign 4 or 5, depending on how the bike has crashed
		 int game_over_1 = game_over_check(bike_mode_1, /*Player1,*/ 1);
		 	if (game_over_1 == 4 || game_over_1 == 5) {
			announce_winner(2, bike_color_2);
			tft.setTextSize(1);

			delay(1000);
			break;
			}

		 /** CHANGED THE ORDER HERE SO BOTH DEATH ANIMATIONS DON'T PLAY SIMULTANEOUSLY */
		 int game_over_2 = game_over_check(bike_mode_2, /*Player2,*/ 2);
			if (game_over_2 == 4 || game_over_2 == 5) {
				announce_winner(1, bike_color_1);
				tft.setTextSize(1);

				delay(1000);
				break;
			}
		}
		int end_time = millis();
		Serial.print("Time Taken until Game End: ");
		Serial.println(end_time-start_time);
	}

	Serial.end();
	return 0;
}
