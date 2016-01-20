Brad Ofrim					 
Mark Griffith 	
Jordan Lane	

Notes:
On the Start Menu, hold down the Joystick button for ~5 seconds for 1P mode
User can jump by pressing the joystick button at any point during gameplay
The Options menu allows user to toggle obstacles, as well as change the speed and color 
of the bikes

Accessories:
* Arduino Mega Board (AMG)
* USB connection for Arduino
* Adafruit TFT LCD Screen (160x128)
* 2 Joysticks
* POTENTIOMETER
* Wiring


Wiring Instructions:
* 5V ----> Bread Board Positive Bus
* GND ----> Bread Board GND Bus

* LCD Screen
  1. GND ----> Bread Board GND Bus
  2. VCC ----> Bread Board Positive Bus 
  3. RESET ----> Digital Pin 8
  4. D/C ----> Digital Pin 7
  5. CARD_CS ----> Digital Pin 5
  6. TFT_CS ----> Digital Pin 6
  7. MOSI ----> Digital Pin 51
  8. SCK ----> Digital Pin 52
  9. MISO ----> Digital Pin 50
  10. LITE ----> Bread Board Positive Bus
  
* Joystick (Player 1)
  1. VCC ----> Bread Board Positive Bus
  2. VERT ----> Analog Pin 0
  3. HORZ ----> Analog Pin 1
  4. SEL ----> Digital Pin 10
  5. GND ----> Bread Board GND Bus
  
* Joystick (Player 2)
  1. VCC ----> Bread Board Positive Bus
  2. VERT ----> Analog Pin 2
  3. HORZ ----> Analog Pin 3
  4. SEL ----> Digital Pin 11
  5. GND ----> Bread Board GND Bus


Uploading Instructions (Ubuntu):
* Make sure the tron.cpp file is in the same directory as the appropriate 
  Makefile (both are included in the zip file)
* Connect your Arduino to to your PC via USB
* Use 'arduino-sport-select' in terminal to choose the correct port
* To compile the code type 'make' in terminal and then 'make upload' to
upload the program (this must be done after selecting the correct port)
* If unexpected errors occur, try unplugging the arduino and plugging 
it back in. If this doesn't work, type 'make clean' into terminal and 
then try uploading again.
