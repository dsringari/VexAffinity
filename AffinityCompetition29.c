#pragma config(Sensor, in2,    rightLiftPot,   sensorPotentiometer)
#pragma config(Sensor, in3,    leftLiftPot,    sensorPotentiometer)
#pragma config(Sensor, in4,    clawLiftPot,    sensorPotentiometer)
#pragma config(Sensor, in5,    leftMobilePot,  sensorPotentiometer)
#pragma config(Sensor, in6,    rightMobilePot, sensorPotentiometer)
#pragma config(Sensor, in7,    directionPot,   sensorPotentiometer)
#pragma config(Sensor, dgtl1,  LeftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  RightEncoder,   sensorQuadEncoder)
#pragma config(Motor,  port2,           leftDrive,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           claw,          tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           rightMobile,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           leftLift,      tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           rightLift,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           leftMobile,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           clawLift,      tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           rightDrive,    tmotorVex393_MC29, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks
  // running between Autonomous and Driver controlled modes. You will need to
  // manage all user created tasks if set to false.
  bStopTasksBetweenModes = true;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
float wheelRadius = 2;
float turnRadius = 5.4;
bool shouldWait = false;
bool shouldWait2 = false;
bool shouldWait3 = false;

// -------- CALCULATIONS ---------

float rotsFor(float degrees) {
	float wheelCircumference = 2 * PI * wheelRadius;
	float turnCircumference = 2 * PI * turnRadius;

	float degRatio = abs(degrees) / 360;
	float turnDist = degRatio * turnCircumference;
	return turnDist / wheelCircumference;
}

float inchesToRots(float inches) {
	float wheelCircumference = 2 * PI * wheelRadius;
	return abs(inches) / wheelCircumference;
}

// -------- ARM CONTROLS ---------

task openLift() {
	while(SensorValue[rightLiftPot] > 3010 && SensorValue[leftLiftPot] > 520){
		motor[rightLift] = 90;
		motor[leftLift] = 90;
	}
	motor[rightLift] = 0;
	motor[leftLift] = 0;
	shouldWait2 = false;
	EndTimeSlice();
}

void closeLift() {
	while(SensorValue[rightLiftPot] < 4070 && SensorValue[leftLiftPot] < 1370){
		motor[rightLift] = -70;
		motor[leftLift] = -70;
	}
	motor[rightLift] = 0;
	motor[leftLift] = 0;
}

void lowerLiftToStack() {

	while(SensorValue[leftLiftPot] < 1230 && SensorValue[rightLiftPot] > 1260){
		motor[rightLift] = -70;
		motor[leftLift] = -70;
	}
	motor[rightLift] = 0;
	motor[leftLift] = 0;
}

task raiseClawLiftTask() {
	while( SensorValue[clawLiftPot] < 2600){
		motor[clawLift] = 120;
	}
	motor[clawLift] = 0;
	shouldWait3 = false;
	EndTimeSlice();
}

void raiseClawLift() {
	startTask(raiseClawLiftTask);
}

void lowerClawLiftAndGrabSecondCone() {
	motor[claw] = -100;
	while (SensorValue[clawLiftPot] > 620) {
		motor[clawLift] = -60;
	}
	motor[rightLift] = -100;
	motor[leftLift] = -100;
	wait(0.1);
	motor[rightLift] = 0;
	motor[leftLift] = 0;
	motor[claw] = 0;
	motor[clawLift] = 0;
}

void releaseCone() {
	raiseClawLift();
	motor[clawLift] = 30;
	motor[claw] = 100;
	wait(0.5);
	motor[claw] = 0;
	motor[clawLift] = 0;
}

void openMobile(bool hasCone) {
	if (hasCone) {
		motor[leftMobile] = 100;
		motor[rightMobile] = 100;
		wait(1);
		motor[leftMobile] = 0;
		motor[rightMobile] = 0;
		return;
	}

	while (SensorValue[leftMobilePot] > 1580 || SensorValue[rightMobilePot] > 740) {
		motor[leftMobile] = 100;
		motor[rightMobile] = 100;
	}

	motor[rightMobile] = 0;
	motor[leftMobile] = 0;
}

void closeMobile() {
	while (SensorValue[leftMobilePot] < 4060 || SensorValue[rightMobilePot] < 2880) {
		motor[leftMobile] = -100;
		motor[rightMobile] = -100;
	}
	motor[rightMobile] = 0;
	motor[leftMobile] = 0;
}



// -------- TURNING ---------

float turnDegrees = 0;
float turnSpeed = 0;

task turnME() {
	float rots = rotsFor(turnDegrees);
	float rotDegrees = rots * 360;

	SensorValue[LeftEncoder] = 0;
	SensorValue[RightEncoder] = 0;

	if (turnDegrees > 0) {
		motor[leftDrive] = turnSpeed;
		motor[rightDrive] = -turnSpeed;
	} else {
		motor[leftDrive] = -turnSpeed;
		motor[rightDrive] = turnSpeed;
	}


	// TODO: Test which turn method is more accurate



	// Normal Turn Method
	while (abs(SensorValue[LeftEncoder]) < rotDegrees || abs(SensorValue[RightEncoder]) < rotDegrees) {
		if (abs(SensorValue[LeftEncoder]) >= rotDegrees && abs(SensorValue[RightEncoder]) >= rotDegrees){
			motor[leftDrive] = 0;
			motor[rightDrive] = 0;
		}
	}



	/*
	// Turning with self-correction
	while (abs(SensorValue[LeftEncoder]) < rotDegrees) {
		// If the left side is farther ahead, speed up the right side
		if (abs(SensorValue[LeftEncoder]) > abs(SensorValue[RightEncoder])) {
			motor[RightDrive] = (turnSpeed + 10) * rightDirection;
			motor[LeftDrive] = turnSpeed * leftDirection;
		}

		// If the right side is farther ahead, speed up the left side
		if (abs(SensorValue[LeftEncoder]) < abs(SensorValue[RightEncoder])) {
			motor[RightDrive] = turnSpeed * rightDirection;
			motor[LeftDrive] = (turnSpeed + 10) * leftDirection;
		}

		// If they are both at equal distances, drive straight ahead.
		if (abs(SensorValue[LeftEncoder]) == abs(SensorValue[RightEncoder])) {
			motor[RightDrive] = turnSpeed * rightDirection;
			motor[LeftDrive] = turnSpeed * leftDirection;
		}
	}
	*/

	shouldWait = false;
	EndTimeSlice();
}

void turn(float deg, float speed){
	turnDegrees = deg;
	turnSpeed = speed;
	startTask(turnME);
}

// -------- MOVING ---------
// Note: DO NOT RUN AT MAX SPEED, RUNNING AT MAX SPEED WILL NOT ALLOW THE ROBOT TO STRAIGHTEN ITSELF.
// You should run this at a maximum of 110.

float moveInches = 0;
float moveSpeed = 0;

task moveME() {
	float rots = inchesToRots(moveInches);
	float adjustment = 1;
	float rotDegrees = rots * 360* adjustment;

	SensorValue[LeftEncoder] = 0;
	SensorValue[RightEncoder] = 0;

	// Either 1 or -1 depending on given inches
	int direction;
	if (moveInches > 0) {
		direction = 1;
		} else {
		direction = -1;
	}

	while(abs(SensorValue[LeftEncoder]) < rotDegrees) {
		motor[rightDrive] = moveSpeed * direction;
		motor[leftDrive] = moveSpeed * direction;
	}

	/*

	// Auto-Straigtening
	while (abs(SensorValue[LeftEncoder]) < rotDegrees) {
		// If the left side is farther ahead, speed up the right side
		if (abs(SensorValue[LeftEncoder]) > abs(SensorValue[RightEncoder])) {
			motor[rightDrive] = (moveSpeed + 10) * direction;
			motor[leftDrive] = moveSpeed * direction;
		}

		// If the right side is farther ahead, speed up the left side
		if (abs(SensorValue[LeftEncoder]) < abs(SensorValue[RightEncoder])) {
			motor[rightDrive] = moveSpeed * direction;
			motor[leftDrive] = (moveSpeed + 10) * direction;
		}

		// If they are both at equal distances, drive straight ahead.
		if (abs(SensorValue[LeftEncoder]) == abs(SensorValue[RightEncoder])) {
			motor[rightDrive] = moveSpeed * direction;
			motor[leftDrive] = moveSpeed * direction;
		}
	}
	*/

	// Stop Motors
	motor[leftDrive] = 0;
	motor[rightDrive] = 0;
	// Let the main task know that we can move ahead
	shouldWait = false;
	EndTimeSlice();
}

void move(float inches, float speed) {
	moveInches = inches;
	moveSpeed = speed;
	startTask(moveME);
}

task autonomous()
{
  // ..........................................................................
  // Insert user code here.
  // ..........................................................................

  // Program Start
	// Raise Lift, Open Up Claw and Mobile

	bool isLeft = SensorValue[directionPot] > 2000;

	// Move to capture mobile goal
	shouldWait = true;
	shouldWait2 = true;
	shouldWait3 = true;
	startTask(openLift);
	move(50, 70); // <- Asynchronous
	raiseClawLift();
	wait(0.2);
	openMobile(false);
	while(shouldWait || shouldWait2 || shouldWait3) {}


	closeMobile();
	lowerLiftToStack();
	releaseCone();
	shouldWait = true;
	move(3, 110);
	lowerClawLiftAndGrabSecondCone();
	while(shouldWait) {}
	motor[rightLift] = 100;
	motor[leftLift] = 100;
	if (isLeft) {
		motor[rightDrive] = -100;
	} else {
		motor[leftDrive] = -100;
	}
	wait(0.1);
	motor[rightDrive] = 0;
	motor[leftDrive] = 0;
	wait(0.1);
	motor[rightLift] = 0;
	motor[leftLift] = 0;

	shouldWait = true;
	move(-42, 110);
	raiseClawLift();
	releaseCone();
	while(shouldWait) {}


	// Turn
	shouldWait = true;
	if (isLeft) {
		turn(175, 80);
	} else {
		turn(-175, 80);
	}
	while(shouldWait) {}

	shouldWait = true;
	move(27,110);
	while(shouldWait) {}

	if (isLeft) {
		motor[leftDrive] = 110;
		motor[rightDrive] = -30;
	} else {
		motor[leftDrive] = -30;
		motor[rightDrive] = 110;
	}
	wait(0.6);
	if (isLeft) {
		motor[rightDrive] = 100;
	} else {
		motor[leftDrive] = 100;
	}
	wait(2);
	motor[leftDrive] = 0;
	motor[rightDrive] = 0;
	shouldWait = true;
	startTask(openLift);
	wait(0.2);
	openMobile(false);
	while(shouldWait){}
	// Move backwards and close the mobile to release the cone
	shouldWait = true;
	move(-15,110); // <- Asynchronous
	closeMobile();
	while(shouldWait) {}
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/


float joyStickCurve(float value) {
	if (value < 0) {
		return -1 * exp(0.03790648643687201 * -value);
	}
	return exp(0.03790648643687201 * value);
}

task usercontrol()
{
  // User control code here, inside the loop

  while(true) {
		// Arcade Controls
		int c1 =  vexRT[Ch3];
		int c3 = -vexRT[Ch1];
		int j3 = joyStickCurve(c3);
		int j1 = joyStickCurve(c1);

		if(((c1>20||c1<-20)||(c3>20||c3<-20))){
			motor[rightDrive] = (j3 + j1);
			motor[leftDrive] = (j1 - j3);
		}else{
			motor[leftDrive] = 0;
			motor[rightDrive] = 0;
		}

		// Mobile Lift
		if (vexRT[Btn6D] || vexRT[Btn5DXmtr2]) {
			motor[leftMobile] = 100;
			motor[rightMobile] = 100;
		} else if (vexRT[Btn5D] || vexRT[Btn5UXmtr2]) {
			motor[leftMobile] = -100;
			motor[rightMobile] = -100;
		} else {
			motor[leftMobile] = 0;
			motor[rightMobile] = 0;
		}

		// Lift
		if (abs(vexRT[Ch3Xmtr2]) > 20) {
			float j3 = joyStickCurve(vexRT[Ch3Xmtr2]);
			motor[leftLift] = j3;
			motor[rightLift] = j3;
		} else {
			motor[leftLift] = 0;
			motor[rightLift] = 0;
		}

		// Claw Lift
		if (abs(vexRT[Ch2Xmtr2]) > 20) {
			float j2 = joyStickCurve(vexRT[Ch2Xmtr2]);
			motor[clawLift] = j2;
		} else {
			motor[clawLift] = 0;
		}

		// Claw
		if (vexRT[Btn6UXmtr2]) {
			motor[claw] = -100;
		} else if (vexRT[Btn6DXmtr2]) {
			motor[claw] = 100;
		} else {
			motor[claw] = 0;
		}


	}

}