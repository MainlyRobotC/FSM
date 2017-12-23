#pragma config(Sensor, port3,  color,          sensorVexIQ_ColorGrayscale)
#pragma config(Sensor, port4,  gyro,           sensorVexIQ_Gyro)
#pragma config(Sensor, port5,  scanner,        sensorVexIQ_Color12Color)
#pragma config(Motor,  motor1,           ,             tmotorVexIQ, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motor6,           ,             tmotorVexIQ, PIDControl, reversed, driveRight, encoder)
#pragma config(Motor,  motor10,         arm,           tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor11,         claw,          tmotorVexIQ, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//use the DELUXE CLAWBOT in starting position FAR

enum State //different moves
{
	turn1,
	moveback1,
	liftBar1,
	turn2,
	ramp1,
	moveForward1,
	exlowerArm,
	turn3,
	exraiseArm,
	ramp2,
	moveForward2,
	exTurn1,
	exArmL,
	ramp3,
	exForward1,
	exBackward1,
	exTurn2,
	exArmU,
	ramp4,
	exForward2,
	exBackward2,
	exTurn3,
	ramp5,
	exForward3,
	turn4,
	ramp6,
	moveForward3,
	liftBar2,
	turn5,
	moveback2,
	lowerBar1,
	ramp7,
	moveForward4,
	moveback3,
	raiseBar1,
	turn6,
	ramp8,
	moveForward5,
	turn7,
	ramp9,
	moveForward6,
	moveback4,
	lowerBar2,
	ramp10,
	moveForward7,
	moveback5,
	liftBar3,
	ramp11,
	moveForward8,
	moveback6,
	turn8,
	lowerBar3,
	ramp12,
	moveForward9,
	moveback7,
	liftBar4,
	turn9,
	ramp13,
	moveForward10,
	moveback8,
	turn10,
	ramp14,
	moveforward11,
	turn11,
	lowerBar4,
	ramp15,
	moveforward12,
	moveback9,
	turn12,
	ramp16,
	moveforward13,
	turn13,
	ramp17,
	moveforward14,
	moveback10,
	turn14,
	ramp18,
	moveforward15,
	moveback11,
	end
};

State currentState;
int count = 0;
bool ramp = false;

void moveArm(State nextState, int power, int degrees) //function for moving the robot arm
{
	motor[arm] = power;
	if(abs(nMotorEncoder[arm]) >= degrees)
	{
		nMotorEncoder[arm] = 0;
		motor[arm] = 0;
		currentState = nextState;
	}
}

void moveLine(int lines, int power, State nextState) //used to move a certain number of black lines
{
	motor[motor1] = power;
	motor[motor6] = power;
	if(getColorGrayscale(color) <= 50)
	{
		count++;
	}
	if(count >= lines)
	{
		motor[motor1] = 0;
		motor[motor6] = 0;
		currentState = nextState;
		count = 0;
	}
}

void lowerScore(State nextState) //score on lower horizontal goal
{
	motor[arm] = 50;
	if(abs(nMotorEncoder[arm]) >= 200)
	{
		motor[arm] = 0;
		nMotorEncoder[arm] = 0;
		currentState = nextState;
	}
}

void upperScore(State nextState)
{
	motor[arm] = 50;
	if(abs(nMotorEncoder[arm]) >= 400)
	{
		currentState = nextState;
		nMotorEncoder[arm] = 0;
	}
}

void degrees(State nextState, int degrees, int direction) //used to move a certain number of degrees
{
	motor[motor1] = direction * (degrees + 20 - nMotorEncoder[motor1]);
	motor[motor6] = direction * (degrees + 20 - nMotorEncoder[motor1]);
	if(abs(nMotorEncoder[motor1]) >= degrees - 100)
	{
		nMotorEncoder[motor1] = 0;
		nMotorEncoder[motor6] = 0;
		motor[motor1] = 0;
		motor[motor6] = 0;
		currentState = nextState;
	}
}

void turn(State nextState, int direction, int power, int turnAngle) // left turn = 0, right turn  = 1
{

	if(direction == 1)
	{
		motor[motor1] = power;
		motor[motor6] = -1*power;
	}
	else
	{
		motor[motor1] = -1*power;
		motor[motor6] = power;
	}
	if(abs(SensorValue[gyro]) >= turnAngle)
	{
		motor[motor1] = 0;
		motor[motor6] = 0;
		currentState = nextState;
		resetGyro(gyro);
		nMotorEncoder[motor1] = 0;
		nMotorEncoder[motor6] = 0;
	}
}

void rampUp( int direction, State nextState)
{
	ramp = false;
	motor[motor1] = direction * (abs(nMotorEncoder[motor6]) + 20);
	motor[motor6] = direction * (abs(nMotorEncoder[motor6]) + 20);
	if(motor[motor1] >= 100)
	{
		nMotorEncoder[motor6] = 0;
		nMotorEncoder[motor1] = 0;
		//motor[motor1] = 0;
		//motor[motor6] = 0;
		currentState = nextState;
		ramp = true;
	}
}

task main()
{
	setColorMode(color, colorTypeGrayscale_Reflected);
	currentState = turn1;
	nMotorEncoder[arm] = 0;
	nMotorEncoder[motor1] = 0;
	nMotorEncoder[motor6] = 0;
	while(true)
	{
		switch(currentState)
		{
		case turn1: //intake first blue ring
			turn(moveback1, 1, 100, 90);
			break;

		case moveback1: //setting up to score
			degrees(liftBar1, 40, -1);
			break;

		case liftBar1:
			lowerScore(turn2);
			break;

		case turn2:
			turn(ramp1, 0, 50, 45);
			break;

		case ramp1:
			rampUp(1, moveForward1);
			break;

		case moveForward1: //scoring
			degrees(exlowerArm, 250, 1);
			break;

		case exlowerArm:
			moveArm(turn3, -75, 75);
			break;

		case turn3: //moving to bonus post
			turn(exraiseArm, 0, 50, 90);
			break;

		case exraiseArm:
			moveArm(ramp2, 75, 75);
			break;

		case ramp2:
			rampUp(1, moveForward2);
			break;

		case moveForward2:
			degrees(exTurn1, 600, 1);
			break;

		case exTurn1:
			turn(exArmL, 1, 50, 90);  //move to intske extra blue ring
			break;

		case exArmL:
			moveArm(ramp3, -75, 199);
			break;

		case ramp3:
			rampUp(1, exForward1);
			break;

		case exForward1:
			degrees(exBackward1, 450, 1); //intake extra blue ring
			break;

		case exBackward1:
			degrees(exTurn2, 450, -1);
			break;

		case exTurn2:
			turn(exArmU, 1, 50, 45);
			break;

		case exArmU:
			lowerScore(ramp4);
			break;

		case ramp4:
			rampUp(1, exForward2);
			break;

		case exForward2:
			degrees(exBackward2, 240, 1); //outtake extra blue ring
			break;

		case exBackward2:
			degrees(exTurn3, 400, -1);
			break;

		case exTurn3:
			turn(ramp5, 0, 50, 133);
			break;

		case ramp5:
			rampUp(1, exForward3);
			break;

		case exForward3:
			degrees(turn4, 640, 1);
			break;

		case turn4:
			turn(ramp6, 1, 50, 45);
			break;

		case ramp6:
			rampUp(1, moveForward3);
			break;

		case moveForward3:
			degrees(liftBar2, 160, 1);
			break;

		case liftBar2:
			moveArm(turn5, 75, 120);
			break;

		case turn5:
			turn(moveback2, 0, 50, 89);
			break;

		case moveback2:
			degrees(lowerBar1, 320, -1);
			break;

		case lowerBar1:
			moveArm(ramp7, -75, 245);
			break;

		case ramp7:
			rampUp(1, moveForward4);
			break;

		case moveForward4: //intake blue ring from bonus post
			degrees(moveback3, 160, 1);
			break;

		case moveback3: //getting ring off post
			degrees(raiseBar1, 50, -1);
			break;

		case raiseBar1: //setting to score ring
			moveArm(turn6, 150, 100);
			break;

		case turn6:
			turn(ramp8, 0, 50, 115);
			break;

		case ramp8:
			rampUp(1, moveForward5);
			break;

		case moveForward5:
			degrees(turn7, 700, 1);
			break;

		case turn7:
			turn(ramp9, 0, 50, 67);
			break;

		case ramp9:
			rampUp(1, moveForward6);
			break;

		case moveForward6: //scoring ring
			degrees(moveback4, 420, 1);
			break;

		case moveback4: //setting to intake first green ring
			degrees(lowerBar2, 300, -1);
			break;

		case lowerBar2:
			moveArm(ramp10, -75, 160);
			break;

		case ramp10:
			rampUp(1, moveForward7);
			break;

		case moveForward7: //intake ring
			degrees(moveback5, 350, 1);
			break;

		case moveback5:
			degrees(liftBar3, 270, -1);
			break;

		case liftBar3:
			moveArm(ramp11, 75, 380);
			break;

		case ramp11:
			rampUp(1, moveForward8);
			break;

		case moveForward8: //score ring
			degrees(moveback6, 260, 1);
			break;

		case moveback6: //setting to intake second green ring
			degrees(turn8, 300, -1);
			break;

		case turn8:
			turn(lowerBar3, 1, 50, 36);
			break;

		case lowerBar3:
			moveArm(ramp12, -75, 395);
			break;

		case ramp12:
			rampUp(1, moveForward9);
			break;

		case moveForward9:
			degrees(moveback7, 400, 1); //intake second green ring
			break;

		case moveback7:
			degrees(liftBar4, 300, -1);
			break;

		case liftBar4:
			moveArm(turn9, 75, 400);
			break;

		case turn9:
			turn(ramp13, 0, 50, 50); //score second green ring
			break;

		case ramp13:
			rampUp(1, moveForward10);
			break;

		case moveForward10:
			degrees(moveback8, 75, 1);// setting to knock over the bonus tray
			break;

		case moveback8:
			degrees(turn10, 150, -1);
			break;

		case turn10:
			turn(ramp14, 0, 50, 130);
			break;

		case ramp14:
			rampUp(1, moveforward11);
			break;

		case moveforward11:
			degrees(turn11, 1300, 1);
			break;

		case turn11:
			turn(lowerBar4, 1, 50, 50);
			break;

		case lowerBar4:
			moveArm(ramp15, -50, 400);
			break;

		case ramp15:
			rampUp(1, moveforward12);
			break;

		case moveforward12:
			degrees(moveback9, 2600, 1); //knock over left side of bonus tray
			break;

		case moveback9:
			degrees(turn12, 100, -1); // setting to knock over right side of bonus tray
			break;

		case turn12:
			turn(ramp16, 1, 100, 90);
			break;

		case ramp16:
			rampUp(1, moveforward13);
			break;

		case moveforward13:
			degrees(turn13, 1150, 1);
			break;

		case turn13:
			turn(ramp17, 0, 100, 90);
			break;

		case ramp17:
			rampUp(1, moveforward14);
			break;

		case moveforward14:
			degrees(moveback10, 500, 1); //knock over right side of bonus tray
			break;

		case moveback10:
			degrees(turn14, 1200, -1);
			break;

		case turn14:
			turn(ramp18, 1, 100, 90);
			break;

		case ramp18:
			rampUp(1, moveforward15);
			break;

		case moveforward15:
			degrees(moveback11, 500, -1);
			break;

		case moveback11:
			degrees(end, 1500, 1);
			break;

			// FINAL SCORE: SHOULD BE 72, + OR -, 1 - 7
			//Remember to add another green ring to score
		}
	}
}
