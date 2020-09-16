// Including the libraries we want VVV
#include <Arduino.h>
#include <PS2X_lib.h>
#include <Adafruit_PWMServoDriver.h>

/*
 * Author: 		0choa
 * Date:   		3.30.20
 * Description: This program was loaded onto Frobo,
 * 				a 4BL + claw robot that competed in
 * 				The MORDOR-RING-DROP battle! (Dun dun dun!)
 */

// Forward declaration for our autonomous routine function
void autoRoutine();

// ~ ~ ~ ~ ~ Important variables! ~ ~ ~ ~ ~ //

// Section 1: Drivetrain variables

int speed1;				// Holds the PWM signal read from controller

// how we want it to be: (This is from StingTM)
byte leftBackward = 2;
byte leftForward = 3;
byte leftPWM = 4;
byte rightForward = 5;
byte rightBackward = 6;
byte rightPWM = 7;

// OLD Frobo pins (improper)
/*
byte leftBackward = 6;	// Left backwards pin #
byte leftForward = 5;	// Left forwards pin #
byte leftPWM = 7;		// Left PWM pin #
byte rightBackward = 2;	// Right backwards pin #
byte rightForward = 3;	// Right forwards pin #
byte rightPWM = 4;		// Right PWM pin #
*/

// Section 2: Arm variables

byte leftArmUp = 8;		// Left arm up pin #
byte leftArmDown = 9;	// Left arm down pin #
byte leftArmPWM = 10;	// Left arm PWM pin #
byte rightArmUp = 11;	// Right arm up pin #
byte rightArmDown = 12;	// Right arm down pin #
byte rightArmPWM = 13;	// Right arm PWM pin #

// Section 3: Claw variables

byte servo = 1;			// Claw port # on servo driver board

// Declaring and initializing an object, called pwm,
// of type Adafruit_PWMServoDriver
Adafruit_PWMServoDriver servoDriverBoard = Adafruit_PWMServoDriver();

// Section 4: PS2 controller variables

// Breakout board pins
const byte PS2_DAT = 51;
const byte PS2_CMD = 50;
const byte PS2_ATT = 53;
const byte PS2_CLK = 52;

// These store information on our PS2 controller
bool pressures = false;
bool rumble = false;

PS2X controller;	// Declaring an object, called controller, of type PS2X

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ //

// The setup function runs once,
// as soon as the 'duino is turned on!
void setup() {

	delay(300); // Giving the controller time to start up

	// Setting up the PS2 controller
	controller.config_gamepad(PS2_CLK, PS2_CMD, PS2_ATT, PS2_DAT, pressures, rumble);

	// Configuring all drivetrain pins to be
	// of type OUTPUT
	pinMode(leftBackward, OUTPUT);
	pinMode(leftForward, OUTPUT);
	pinMode(leftPWM, OUTPUT);
	pinMode(rightForward, OUTPUT);
	pinMode(rightBackward, OUTPUT);
	pinMode(rightPWM, OUTPUT);

	// Configuring all arm pins to be
	// of type OUTPUT
	pinMode(leftArmUp, OUTPUT);
	pinMode(leftArmDown, OUTPUT);
	pinMode(leftArmPWM, OUTPUT);
	pinMode(rightArmUp, OUTPUT);
	pinMode(rightArmDown, OUTPUT);
	pinMode(rightArmPWM, OUTPUT);

	// Configuring the servo driver board
	servoDriverBoard.begin();
	servoDriverBoard.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

	delay(10);

	// Now let's call the autoRoutine function!
	autoRoutine();

	delay(500);
}

// Everything inside the loop function runs
// over and over again, until you turn off yo bot
void loop() {

	// Read the controller
	// Note for macro-ers: this method has to be called at least once a second
	//					   for teleop to work properly.
	controller.read_gamepad();

	// If left joystick is pushed forwards
	if (controller.Analog(PSS_LY) >= 0 && controller.Analog(PSS_LY) <= 120){ //real center value is 128, but around 140 is needed because controller is HIGHLY sensitive

	  speed1 = map(controller.Analog(PSS_LY), 0, 120, 255, 0);// create a set range for values and set output values respectively
	  digitalWrite(leftPWM, HIGH);
	  analogWrite(leftForward, speed1);

	// Otherwise, if left joystick is pulled backwards
	} else if(controller.Analog(PSS_LY) >= 136 && controller.Analog(PSS_LY) <= 255) { //Same as above

	  speed1 = map(controller.Analog(PSS_LY), 136, 255, 0, 255);
	  digitalWrite(leftPWM, HIGH);
	  analogWrite(leftBackward, speed1);

	// Otherwise, the left joystick must be in the "dead zone"
	} else {
	  analogWrite(leftForward, 0);
	  analogWrite(leftBackward, 0);
	}

	// If right joystick is pushed forwards
	if (controller.Analog(PSS_RY) >= 0 && controller.Analog(PSS_RY) <= 120) { //real center value is 128, but around 140 is needed because controller is HIGHLY sensitive

	  speed1 = map(controller.Analog(PSS_RY), 0, 120, 255, 0);// create a set range for values and set output values respectively
	  digitalWrite(rightPWM, HIGH);
	  analogWrite(rightForward, speed1);

	// Otherwise, if right joystick is pulled backwards
	} else if(controller.Analog(PSS_RY) >= 136 && controller.Analog(PSS_RY) <= 255) { //Same as above

	  speed1 = map(controller.Analog(PSS_RY), 136, 255, 0 , 255);
	  digitalWrite(rightPWM, HIGH);
	  analogWrite(rightBackward, speed1);

	// Otherwise, the right joystick must be in the "dead zone"
	} else {
	  analogWrite(rightForward, 0);
	  analogWrite(rightBackward, 0);
	}

	// If the upper d-pad is pressed
	if(controller.Button(PSB_PAD_UP)) {      //will be TRUE as long as button is pressed

		// Raise dat arm!
		digitalWrite(leftArmDown, LOW);
		digitalWrite(leftArmUp, HIGH);
		digitalWrite(rightArmUp, HIGH);
		digitalWrite(rightArmDown, LOW);
		analogWrite(leftArmPWM, 200);
		analogWrite(rightArmPWM, 200);

	// Otherwise, if the lower d-pad is pressed
	} else if(controller.Button(PSB_PAD_DOWN)) {

		// Lower dat arm!
		digitalWrite(leftArmDown, HIGH);
		digitalWrite(leftArmUp, LOW);
		digitalWrite(rightArmUp, LOW);
		digitalWrite(rightArmDown, HIGH);
		analogWrite(leftArmPWM, 110);
		analogWrite(rightArmPWM, 110);

	// Otherwise . . .
	} else {

		// Give the "Up" pins juuust enough power
		// to hold the arm's position against dat g force
		digitalWrite(leftArmDown, LOW);
		digitalWrite(leftArmUp, HIGH);
		digitalWrite(rightArmUp, HIGH);
		digitalWrite(rightArmDown, LOW);
		analogWrite(leftArmPWM, 30);
		analogWrite(rightArmPWM, 30);
	}

	// If the left bumper is pressed
	if(controller.Button(PSB_L1)) {      //will be TRUE as long as button is pressed
		servoDriverBoard.setPWM(servo, 0, 400);		// Close claw

	// Otherwise, if the right bumper is pressed
	} else if(controller.Button(PSB_R1)) {
		servoDriverBoard.setPWM(servo, 0, 300);		// Open claw

	// Stop dat servo!
	} else {
		servoDriverBoard.setPWM(servo, 0, 0);
	}

	// Small delay before re-loopin'
	delay(50);
}

// // // // // // // // // // // // //

// The autoRoutine function takes no args
// and returns no values. It is called once,
// when it's time for autonomous in the setup function.
void autoRoutine(){

	delay(10);


	// Pick up noodle
	servoDriverBoard.setPWM(servo, 0, 400);
	delay(800);
	servoDriverBoard.setPWM(servo, 0, 0);

	delay(10);

	// Raise arm all da wayyy
	digitalWrite(leftArmDown, LOW);
	digitalWrite(leftArmUp, HIGH);
	digitalWrite(rightArmUp, HIGH);
	digitalWrite(rightArmDown, LOW);
	analogWrite(leftArmPWM, 255);
	analogWrite(rightArmPWM, 255);
	delay(870);
	analogWrite(leftArmPWM, 30);
	analogWrite(rightArmPWM, 30);

	delay(10);

	// Drive forward
	digitalWrite(leftForward, HIGH);
	digitalWrite(leftBackward, LOW);
	digitalWrite(rightForward, HIGH);
	digitalWrite(rightBackward, LOW);
	analogWrite(leftPWM, 180);
	analogWrite(rightPWM, 180);
	delay(2710); // This value may be different for u :)
	analogWrite(leftPWM, 0);
	analogWrite(rightPWM, 0);

	delay(10);

	// Release the noodle
	servoDriverBoard.setPWM(servo, 0, 300);
	delay(500);
	servoDriverBoard.setPWM(servo, 0, 0);

/* not sure why this is in here...
	delay(10);

	digitalWrite(leftArmB, LOW);
	digitalWrite(leftArmA, HIGH);
	digitalWrite(rightArmA, HIGH);
	digitalWrite(rightArmB, LOW);
	analogWrite(PWML, 255);
	analogWrite(PWMR, 255);
	delay(200);
	analogWrite(PWML, 30);
	analogWrite(PWMR, 30);
*/
	delay(10);

	// Back up!!!
	digitalWrite(leftForward, LOW);
	digitalWrite(leftBackward, HIGH);
	digitalWrite(rightForward, LOW);
	digitalWrite(rightBackward, HIGH);
	analogWrite(leftPWM, 150);
	analogWrite(rightPWM, 150);
	delay(1100);
	analogWrite(leftPWM, 0);
	analogWrite(rightPWM, 0);

	delay(10);

	// Lower arm
	digitalWrite(leftArmUp, LOW);
	digitalWrite(leftArmDown, HIGH);
	digitalWrite(rightArmDown, HIGH);
	digitalWrite(rightArmUp, LOW);
	analogWrite(leftArmPWM, 100);
	analogWrite(rightArmPWM, 100);
	delay(900); //DIFFERENCE
	analogWrite(leftArmPWM, 0);
	analogWrite(rightArmPWM, 0);

	delay(1000);
}



